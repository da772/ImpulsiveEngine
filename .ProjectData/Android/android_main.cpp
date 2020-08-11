/*
* Copyright (C) 2010 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/
#ifdef GE_PLATFORM_ANDROID
#include "gepch.h"
#include <malloc.h>
#include <jni.h>
#include <errno.h>
#include "Public/Core/Platform/Window/Android/android_util.h"
#include "Public/Core/Platform/Window/Mobile/Mobile_Input.h"
#include "GEngine.h"
#include <EntryPoint.h>

#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <android/sensor.h>
#include <android/input.h>

#include <android/log.h>
#include "android_native_app_glue.h"


#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Android3.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Android3.NativeActivity", __VA_ARGS__))

struct saved_state {
	float angle;
	int32_t x;
	int32_t y;
};

bool checkResize = false;
bool ActivityPaused = false;
bool running = false;
bool appCreated = false;
void main_loop();
void SetActivityStatus(bool b);

/**
* Shared state for our app.
*/
struct engine {
	struct android_app* app;

	ASensorManager* sensorManager;
	const ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;

	int animating;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
	struct saved_state state;
};


/**
* Initialize an EGL context for the current display.
*/
static int engine_init_display(struct engine* engine) {
	// initialize OpenGL ES and EGL
	/*
	* Here specify the attributes of the desired configuration.
	* Below, we select an EGLConfig with at least 8 bits per color
	* component compatible with on-screen windows
	*/
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
		EGL_NONE
	};

	
	EGLint w, h, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Here, the application chooses the configuration it desires. In this
	* sample, we have a very simplified selection process, where we pick
	* the first EGLConfig that matches our criteria */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	* As soon as we picked a EGLConfig, we can safely reconfigure the
	* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);

	const int attrib_list[] = {
	   EGL_CONTEXT_CLIENT_VERSION, 3,
	   EGL_NONE
	};
	

	context = eglCreateContext(display, config, EGL_NO_CONTEXT, attrib_list);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}
	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;
	engine->state.angle = 0;
	

	// Initialize GL state.
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	//glEnable(GL_CULL_FACE);

	//glShadeModel(GL_SMOOTH);
	
	//LOGW("%s - %s", engine->app->activity->externalDataPath, engine->app->activity->internalDataPath);

	//const char* fpath = ;

	//GEngine::Ref<GEngine::FileData> data = GEngine::FileSystem::FileDataFromPath(GEngine::Utility::FilePathFromRoot("Content/shaders/TextureShader.glsl"));

	//LOGW("data %s", data->GetDataAsString());

	if (!appCreated) {
		LOGW("CREATING APP");
		App_Create(w, h);
		appCreated = true;
	}
	else {
		LOGW("RELOADING APP");
		App_ReloadGraphics();
	}
	
	return 0;
}

/**
* Just the current frame in the display.
*/
static void engine_draw_frame(struct engine* engine) {
	if (engine->display == NULL) {
		// No display.
		return;
	}

	// Just fill the screen with a color.
	//glClearColor(((float)engine->state.x) / engine->width, engine->state.angle,
	//	((float)engine->state.y) / engine->height, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
	eglSwapBuffers(engine->display, engine->surface);
	App_Draw();
	
}

/**
* Tear down the EGL context currently associated with the display.
*/
static void engine_term_display(struct engine* engine) {

	if (engine->display != EGL_NO_DISPLAY) {
		App_UnloadGraphics();
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT) {
			GE_CORE_DEBUG("DESTROYING EGL CONTEXT");
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE) {
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->animating = 0;
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}


std::unordered_map<int, uint64_t> touch_map;

/**
* Process the next input event.
*/
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
	struct engine* engine = (struct engine*)app->userData;

	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
		uint32_t action = AKeyEvent_getAction(event);
		uint32_t keyCode = AKeyEvent_getKeyCode(event);
		GE_CORE_DEBUG("Action: {0} Keycode: {1}", action, keyCode);
	}

	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		
		
		uint32_t cnt = AMotionEvent_getPointerCount(event);
		for (int i = 0; i < cnt; i++) {
			uint32_t id = AMotionEvent_getPointerId(event, i);
			uint32_t n = AMotionEvent_getAction(event);
			float x = AMotionEvent_getX(event, i);
			float y = AMotionEvent_getY(event, i);
			float pressure = AMotionEvent_getPressure(event, i);
			engine->state.x = x;
			engine->state.y = y;
			uint32_t action = n == 1 ? 2 : n == 2 ? 1 : n == 0 ? 0 : 3;
			if (action != 0) {
				if (touch_map.find(id) != touch_map.end()) {
					uint64_t _id = touch_map[id];
					if (action == 3 || action == 2) {
						touch_map.erase(id);
					}
					GEngine::Mobile_Input_Callback::Touched(_id, action, x, y, pressure);
					continue;
				}
			}
			uint64_t _id = GEngine::Time::GetEpochTimeNS();
			if (touch_map.find(_id) != touch_map.end()) {
				_id++;
			}
			touch_map[id] = _id;
			GEngine::Mobile_Input_Callback::Touched(touch_map[id], action, x, y, pressure);
			continue;
			
			
		}

		return 1;
	}
	
	
	return 0;
}


/**
* Process the next main command.
*/
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	struct engine* engine = (struct engine*)app->userData;
	switch (cmd) {
	case APP_CMD_SAVE_STATE:
		// The system has asked us to save our current state.  Do so.
		engine->app->savedState = malloc(sizeof(struct saved_state));
		*((struct saved_state*)engine->app->savedState) = engine->state;
		engine->app->savedStateSize = sizeof(struct saved_state);
		break;
	case APP_CMD_INIT_WINDOW:
		// The window is being shown, get it ready.
		LOGW("APP_CMD_INIT_WINDOW!");
		if (engine->app->window != NULL) {
			engine_init_display(engine);
			//App_Create(engine->width, engine->height);
			engine_draw_frame(engine);
		}
		break;
	case APP_CMD_TERM_WINDOW:
		// The window is being hidden or closed, clean it up.
		//GE_LOG_WARN("APP ENDING");
		//App_Shutdown();
		engine_term_display(engine);
		break;
	case APP_CMD_GAINED_FOCUS:
		// When our app gains focus, we start monitoring the accelerometer.
		LOGW("GAINING FOCUS!");
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_enableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
			// We'd like to get 60 events per second (in microseconds).
			ASensorEventQueue_setEventRate(engine->sensorEventQueue,
				engine->accelerometerSensor, (1000L / 60) * 1000);
		}
		engine->animating = 1;
		break;
	case APP_CMD_LOST_FOCUS:
		// When our app loses focus, we stop monitoring the accelerometer.
		// This is to avoid consuming battery while not being used.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_disableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
		}
		// Also stop animating.
		engine->animating = 0;
		engine_draw_frame(engine);
		break;
	case APP_CMD_CONFIG_CHANGED:
	{
		checkResize = true;
		break;
	}
	case APP_CMD_PAUSE:
	{
		//ActivityPaused = true;
		break;
	}
	case APP_CMD_RESUME:
	{
		//ActivityPaused = false;
		LOGW("RESUME ACTIVITY GLU");
		//main_loop();
		break;
	}
	
	case APP_CMD_WINDOW_RESIZED:
	{
		break;
	}
		//App_Resize(engine->width, engine->height);
		
	}
}

void* statePtr;
void* enginePtr = nullptr;


void resume_loop(ANativeActivity* a);
void pause_loop(ANativeActivity* a);


//std::mutex activityMutex;

bool GetActivityStatus() {
	//std::lock_guard guard(activityMutex);
	return ActivityPaused;
}

void SetActivityStatus(bool b) {
	//std::lock_guard guard(activityMutex);
	ActivityPaused = b;
	if (b == true)
		running = false;
}

void main_loop() {
	// loop waiting for stuff to do.
	bool bRunning = false;
	{
		bRunning = running;
	}

	if (!bRunning) {
		//std::lock_guard guard(activityMutex);
		running = true;
	}
	else {
		LOGW("MAIN_LOOP STOPPED");
		//return;
	}
	
	engine& engine = *(struct engine*)enginePtr;
	android_app* state = (android_app*)statePtr;
	//state->activity->vm->AttachCurrentThread(&state->activity->env, NULL);
	while (true) {
		// Read all pending events.
		int ident;
		int events;
		struct android_poll_source* source;
		

		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		
		while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
			(void**)&source)) >= 0) {

			// Process this event.
			if (source != NULL) {
				source->process(state, source);
			}

			// If a sensor has data, process it now.
			if (ident == LOOPER_ID_USER) {
				if (engine.accelerometerSensor != NULL) {
					ASensorEvent event;
					while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
						&event, 1) > 0) {
						//LOGI("accelerometer: x=%f y=%f z=%f",
						//	event.acceleration.x, event.acceleration.y,
						//	event.acceleration.z);
					}
				}
				
			}

			// Check if we are exiting.
			if (state->destroyRequested != 0) {
				LOGW("DESTROY REQUESTED!");
				engine_term_display(&engine);
				return;
			}
		}
		

		App_Update();
		engine_draw_frame(&engine);
		if (checkResize) {


			uint32_t w = ANativeWindow_getWidth(engine.app->window);
			uint32_t h = ANativeWindow_getHeight(engine.app->window);
			if (w != engine.width && h != engine.height) {
				checkResize = false;
				engine.width = w;
				engine.height = h;
				LOGW("RESIZED? %d, %d", w, h);
				App_Resize(engine.width, engine.height);
			}

		}
	}
}



void resume_loop(ANativeActivity* a) {
	SetActivityStatus(false);
	LOGW("RESUME LOOP");
	//running = false;
	//main_loop();
	LOGW("LOOP STOPPED?");
}
void pause_loop(ANativeActivity* a) {
	LOGW("PAUSE LOOP");
	SetActivityStatus(true);
	running = false;
}



/**
* This is the main entry point of a native application that is using
* android_native_app_glue.  It runs in its own thread, with its own
* event loop for receiving input events and doing other things.
*/
void android_main(struct android_app* state) {


	GEngine::Mobile_Input_Callback::SetGetViewContext([]() {
		return AndroidUtil::GetAndroidActivity();
	});

	if (enginePtr != nullptr) {
		main_loop();
		return;
	}
	

	struct engine engine;
	statePtr = (void*)state;

	memset(&engine, 0, sizeof(engine));
	
	state->userData = &engine;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;
	engine.app = state;
	

	// Prepare to monitor accelerometer
	engine.sensorManager = ASensorManager_getInstance();
	engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
		ASENSOR_TYPE_ACCELEROMETER);
	engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
		state->looper, LOOPER_ID_USER, NULL, NULL);

	if (state->savedState != NULL) {
		// We are starting with a previous saved state; restore from it.
		engine.state = *(struct saved_state*)state->savedState;
	}
	
	engine.animating = 1;

	enginePtr = (void*)&engine;
	AndroidUtil::setEngine((AndroidUtil::engine*) &engine);
	//engine.app->activity->callbacks->onResume = &resume_loop;
	//engine.app->activity->callbacks->onPause = &pause_loop;
	

	main_loop();


}
#endif