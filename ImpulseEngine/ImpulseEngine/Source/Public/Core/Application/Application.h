#pragma once

#include "Public/Core/Core.h"
#include "Public/Core/Events/Event.h"
#include "Public/Core/Events/ApplicationEvent.h"

#include "Window.h"

#include "Public/Core/Application/LayerStack.h"
#include "Public/Core/Events/Event.h"
#include "Public/Core/Events/ApplicationEvent.h"
#include "Public/Core/ImGui/ImGuiLayer.h"

#include "Public/Core/Renderer/Renderer.h"
#include "Public/Core/Renderer/Renderer2D.h"
#include "Public/Core/Renderer/RenderCommand.h"

#include "Public/Core/Renderer/Camera.h"
#include "Public/Core/Renderer/RenderSettings.h"




namespace GEngine {

	class Shader;
	class Timestep;
	class CameraController;


	class GE_API Application
	{
	public:
		Application();
		Application(const char* title, int width, int height);
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopOverlay(Layer* layer);

		inline static Application* GetApp() { return Application::s_Instance; };
		static inline Window* GetWindow() { return Application::s_Instance ? Application::s_Instance->m_Window.get() : nullptr; }

		void QueueWindowApi(const FWindowApi& windowApi);
		void QueueGraphicsApi(const FGraphicsApi& graphicsApi);
		inline FWindowApi GetWindowApi() {return Application::s_windowApi;};
		inline FGraphicsApi GetGraphicsApi() { return Application::s_graphicsApi; }


		inline void SetTargetCamera(Camera* camera) { m_Camera = camera;  }
		inline void SetTargetCameraController(CameraController* camera) { m_CameraContoller = camera; }
		inline Camera* GetTargetCamera() { return m_Camera; }
		inline CameraController* GetTargetCameraController() { return m_CameraContoller; }

		static void UnloadGraphics();
		static void ReloadGraphics();
		static void Stop();
		static void Start();

		static int GetWidth();
		static int GetHeight();
		static int GetWindowWidth();
		static int GetWindowHeight();
		static int GetSafeTop();
		static int GetSafeBottom();
		static int GetSafeLeft();
		static int GetSafeRight();
		static float GetSafeTopUI();
		static float GetSafeBottomUI();
		static float GetSafeLeftUI();
		static float GetSafeRightUI();
		static inline bool DebugTools() { return s_debugTools; }

		static const int GetViewPortOffsetX() { return s_Instance->m_viewPortOffset.x; }
		static const int GetViewPortOffsetY() { return s_Instance->m_viewPortOffset.y; }

		static const int GetUIResolutionWidth() { return s_Instance->m_uiResolutionWidth; }
		static const int GetUIResolutionHeight() { return s_Instance->m_uiResolutionHeight; }

		static void SetViewPortOffset(const Vector2f& offset) { s_Instance->m_viewPortOffset = offset; }

		inline static const bool IsGamePaused() { return s_Instance->m_pause; };

		static void Setup();
		void Shutdown();
        void Update(float ts = 0);
        void Draw();
        void Pause();
        void Resume();


		static void PauseGame();
		static void ResumeGame();

		std::unordered_map<std::string, float> profile = {
		{"Layers", 0.f},
		{"FPS", 0.f},
		{"OnUpdate", 0.f},
		{"OnRender", 0.f},
		{"FrameTime", 0.f},
		{"Run", 0.f},
		{"Physics", 0.f}
		};

		inline bool IsRunning() { return m_Running; }
		void EnableImGui(bool b);

		int m_viewPortWidth = 0, m_viewPortHeight = 0;

		static bool InputEnabled();
		static void SetInputEnabled(bool b);

		static inline const struct RenderSettings& GetRenderSettings() { return s_Instance->m_renderSettings; }

		inline void SetRenderScale(const float i) { m_renderSettings.renderScale = i; Renderer::SetRenderScale(m_renderSettings.renderScale); }
		inline void SetRenderSamples(uint8_t i) { m_renderSettings.samples = i; }
		
		std::string AddOnGamePauseCallback(std::function<void(bool)> f);
		inline void RemoveOnGamePauseCallback(std::string s) { m_onPausedCallbacks.erase(s); }


	protected:
		void SetWindowApi(const FWindowApi& windowApi);
		void SetGraphicsApi(const FGraphicsApi& graphicsApi);
		Scope<Window> m_Window;
		inline virtual void OnCleanDirtyApi() {};
		inline virtual void OnUpdate(Timestep timeStep) {};
		Camera* m_Camera = nullptr;
		CameraController* m_CameraContoller = nullptr;
		FWindowApi GetDefaultWindowApi();
		FGraphicsApi GetDefaultGraphicsApi();

		inline void SetUIResolution(const int width, const int height) { m_uiResolutionWidth = width; m_uiResolutionHeight = height;};
		struct RenderSettings m_renderSettings;
		bool m_pause = false;

		double m_LastFrameTime = 0;
		int m_frameCount = 0;
		float m_totalTime = 0;
		float m_fps = 0;
		bool m_loaded = true;
        
		Vector2f m_viewPortOffset = Vector2f(0,0);
		int m_width = 1280, m_height = 720;
		int m_uiResolutionWidth = 0, m_uiResolutionHeight = 0;

		const char* title = "NULL";
		
		static bool s_debugTools;
		bool m_enableInput = true;

		std::unordered_map<std::string, std::function<void(bool)>> m_onPausedCallbacks;

	private:
		bool b_EnableImGui = false;
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		
		
		void LayerSetup();
		void LayerReset();
		void CleanDirtyApi();

		bool b_NewWindowApi = false;
		bool b_NewGraphicsApi = false;
		 
		
		float timer = 0;

		FWindowApi tempWindowApi;
		FGraphicsApi tempGraphicsApi;

		
		Layer* m_ImGuiLayer = nullptr;
		bool m_Running = true, m_Minimized = false;;
		LayerStack m_LayerStack;
		static Application* s_Instance;
		static FWindowApi s_windowApi;
		static FGraphicsApi s_graphicsApi;
		
		

		

	};

	// To be defined in CLIENT
	Application* CreateApplication();

}
