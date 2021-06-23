#pragma once

#include "gepch.h"

#include "Public/Core/Core.h"
#include "Public/Core/Events/Event.h"
#include "Public/Core/Renderer/Graphics/GraphicsContext.h"

#ifdef GE_WINDOW_API_SERVER
#ifndef GE_WINDOW_API_DEFAULT
#define GE_WINDOW_API_DEFAULT GEngine::FWindowApi::SERVER;
#endif
#endif
#ifdef GE_WINDOW_API_GLFW
#ifndef GE_WINDOW_API_DEFAULT
#define GE_WINDOW_API_DEFAULT GEngine::FWindowApi::GLFW;
#endif
#endif
#ifdef GE_WINDOW_API_WIN32
#ifndef GE_WINDOW_API_DEFAULT
#define GE_WINDOW_API_DEFAULT GEngine::FWindowApi::WIN32;
#endif
#endif
#ifdef GE_WINDOW_API_MOBILE
#ifndef GE_WINDOW_API_DEFAULT
#define GE_WINDOW_API_DEFAULT GEngine::FWindowApi::MOBILE;
#endif
#endif

namespace GEngine {


	namespace WindowApi { enum class FWindowApi : uint8_t { NONE = 0, GLFW, WIN32API, MOBILE, SERVER }; }

	using namespace WindowApi;

	using EventCallbackFn = std::function<void(Event&)>;

	struct GE_API WindowData
	{
	public:
		WindowData() {};
		WindowData(std::string title, unsigned int width = 1280, unsigned int height = 720) 
		:Title(title), Width(width),Height(height)	{

		}
		std::string Title = "GEngine";
		unsigned int Width = 1280, Height = 720;
		bool VSync;
		int safe_top =0 , safe_bottom = 0, safe_left = 0, safe_right = 0;
		const float GetSafeTopUI() const;
		const float GetSafeBottomUI() const;
		const float GetSafeLeftUI() const;
		const float GetSafeRightUI() const;

		EventCallbackFn EventCallback;
	};


	// Interface representing a desktop system based Window
	class GE_API Window
	{
	public:
		virtual ~Window();

		virtual void OnUpdate(bool m_Minimized) = 0;

		inline const WindowData& GetWindowData() { return m_Data; };

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetTitle(const std::string& title) = 0;

		virtual void SetSize(uint16_t width, uint16_t height) = 0;


        std::vector<std::function<void(void)>> f_list;
        
		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual float const GetTime() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual void Shutdown() = 0;

		virtual void SetWindowHint(int hint, int value) = 0;

		inline virtual void MaximizeWindow() {};
		inline virtual void RestoreWindow() {};
        
        virtual void GetFrameBufferSize(int* width, int* height) = 0;

		virtual void GetSafeArea(int* top, int* bottom, int* left, int* right) = 0;

		virtual void Init(const WindowData& props) = 0;

		

		template<typename T>
		static Window* _CreateWindow(const WindowData& props);

		Scope<GraphicsContext> m_Context;

		virtual void GetSafeAreaUI(float* top, float* bottom, float* left, float* right);

		inline static const FWindowApi GetWindowApi() { return Window::s_WindowApi; }
        
	protected:
		WindowData m_Data;
	private:
        static FWindowApi s_WindowApi;
		
	};

	extern Window* Create_Window(const WindowData& props = WindowData());

}
