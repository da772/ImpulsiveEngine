#include "gepch.h"
#ifdef GE_PLATFORM_ANDROID

#include "Public/Platform/Window/Android/android_util.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <android/sensor.h>
#include <android/log.h>


#include "Public/Platform/Window/Android/android_native_app_glue.h"

/*ADS*/
#include "firebase/admob.h"
#include "firebase/admob/types.h"
#include "firebase/app.h"
#include "firebase/future.h"
#include "firebase/admob/rewarded_video.h"

#include "Public/Core/Util/ThreadPool.h"

namespace AndroidUtil {
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Android3.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Android3.NativeActivity", __VA_ARGS__))


	struct saved_state {
		float angle;
		int32_t x;
		int32_t y;
	};


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

	struct engine* m_engine = nullptr;
	
	JNIEnv* nativeEnv = NULL;
	JavaVM* g_JavaVM = NULL;
	JNIEnv* jni_env = NULL;
	JavaVMInitArgs vm_args;
	

	void setEngine(struct engine* en)
	{
		m_engine = en;
	}

	struct filedat LoadFileToMemory(const char* filePath)
	{

		//	LOGW("READING FILE");

			/*
			// Open file
			AAsset* file = AAssetManager_open(_engine->app->activity->assetManager, filePath, AASSET_MODE_BUFFER);
			// Get the file length
			size_t fileLength = AAsset_getLength(file);

			// Allocate memory to read your file
			void* fileContent = malloc(fileLength + 1);

			// Read file
			AAsset_read(file, fileContent, fileLength);

			// Close file
			AAsset_close(file);

			LOGW("FILE READ %d", fileLength);
			*/
			// Open your file
		AAsset* file = AAssetManager_open(m_engine->app->activity->assetManager, filePath, AASSET_MODE_BUFFER);
		// Get the file length
		size_t fileLength = AAsset_getLength(file);

		// Allocate memory to read your file
		void* fileContent = malloc(fileLength + 1);

		// Read your file
		AAsset_read(file, fileContent, fileLength);
		AAsset_close(file);

		struct filedat a = { fileLength, (unsigned char*)fileContent };

		return a;

	}

	void* GetNativeActivity()
	{

		return (void*)m_engine->app->activity;
	}

	void SetRewardUserId_Google(std::string id)
	{
		jni_env = GetJNIEnv();

		jobject nativeActivity = m_engine->app->activity->clazz;
		jclass acl = jni_env->GetObjectClass(nativeActivity);
		jmethodID getClassLoader = jni_env->GetMethodID(acl, "getClassLoader", "()Ljava/lang/ClassLoader;");
		jobject cls = jni_env->CallObjectMethod(nativeActivity, getClassLoader);
		jclass classLoader = jni_env->FindClass("java/lang/ClassLoader");
		jmethodID findClass = jni_env->GetMethodID(classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
		jstring strClassName = jni_env->NewStringUTF("com.helper.JavaInterface");
		jclass androidInterface = (jclass)(jni_env->CallObjectMethod(cls, findClass, strClassName));
		jni_env->DeleteLocalRef(strClassName);
		
		jstring jstr1 = jni_env->NewStringUTF(id.c_str());
		jmethodID mid = jni_env->GetStaticMethodID(androidInterface, "SetRewardAdID", "(Landroid/app/Activity;Ljava/lang/String;)V");
		jni_env->CallStaticVoidMethod(androidInterface, mid, nativeActivity,jstr1);
		jni_env->DeleteLocalRef(jstr1);
		jni_env->DeleteLocalRef(cls);
		CleanJNIEnv();
	}

	struct safeArea {
		int* top, bottom, left, right;
	};

	void GetSafeArea(int* top, int* bottom, int* left, int* right)
	{
		jni_env = GetJNIEnv();
		jobject nativeActivity = m_engine->app->activity->clazz;
		jclass acl = jni_env->GetObjectClass(nativeActivity);
		jmethodID getClassLoader = jni_env->GetMethodID(acl, "getClassLoader", "()Ljava/lang/ClassLoader;");
		jobject cls = jni_env->CallObjectMethod(nativeActivity, getClassLoader);
		jclass classLoader = jni_env->FindClass("java/lang/ClassLoader");
		jmethodID findClass = jni_env->GetMethodID(classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
		jstring strClassName = jni_env->NewStringUTF("com.helper.JavaInterface");
		jclass androidInterface = (jclass)(jni_env->CallObjectMethod(cls, findClass, strClassName));
		jni_env->DeleteLocalRef(strClassName);

		jmethodID mid = jni_env->GetStaticMethodID(androidInterface, "GetSafeArea", "(Landroid/app/Activity;)Ljava/lang/String;");
		jstring val = (jstring)jni_env->CallStaticObjectMethod(androidInterface, mid, nativeActivity);

		const char* _cStr = jni_env->GetStringUTFChars(val, 0);
		uint32_t cSize = (uint32_t)jni_env->GetStringLength(val);
		std::string s(_cStr);
		jni_env->ReleaseStringUTFChars(val, _cStr);
		jni_env->DeleteLocalRef(val);
		jni_env->DeleteLocalRef(cls);
		CleanJNIEnv();
		int counter = 0;
		int* arr[4] = { top, bottom, left, right };
		std::string _s;
		for (char c : s) {
			if (c == ':') {
				*arr[counter] = std::stoi(_s);
				_s = "";
				if (++counter >= 4) break;
				continue;
			}
			else {
				_s += c;
			}
		}

		*top = *arr[0];
		*bottom = *arr[1];
		*left = *arr[2];
		*right = *arr[3];
		GE_CORE_DEBUG("TOP:{0} BOTTOM:{1} LEFT:{2} RIGHT:{3}", *top, *bottom, *left, *right);
	}

	class LoggingRewardedVideoListener
		: public firebase::admob::rewarded_video::Listener {
	public:
		LoggingRewardedVideoListener() {};
		virtual ~LoggingRewardedVideoListener() {};
		void OnRewarded(firebase::admob::rewarded_video::RewardItem reward) override {
			GE_CORE_DEBUG("REWARDING USER WITH: {0},{1} ", reward.amount, reward.reward_type.c_str());
			//firebase::admob::rewarded_video::Destroy();
		}
		void OnPresentationStateChanged(
			firebase::admob::rewarded_video::PresentationState state) override {
			GE_CORE_DEBUG("REWARDED VIDEO HAS CHANGED TO: {0} ", state);
		}
	};

	JNIEnv* GetJNIEnv()
	{
		JavaVM* vm = m_engine->app->activity->vm;
		JNIEnv* env;
		jint result = vm->AttachCurrentThread(&env, nullptr);
		if (result != 0) {
			GE_CORE_ERROR("COULD NOT CREATE JNIENV");
			return nullptr;
		}
		return env;
	}

	void CleanJNIEnv()
	{
		m_engine->app->activity->vm->DetachCurrentThread();
	}

	jobject GetAndroidActivity()
	{
		return m_engine->app->activity->clazz;
	}

}
#endif