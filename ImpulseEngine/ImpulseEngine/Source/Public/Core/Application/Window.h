#pragma once

#include "gepch.h"

#include "Public/Core/Core.h"
#include "Public/Core/Events/Event.h"
#include "Public/Core/Renderer/Graphics/GraphicsContext.h"

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


	namespace WindowApi { enum FWindowApi : uint8_t { NONE = 0, GLFW, WIN32API, MOBILE }; }

	using namespace WindowApi;

	using EventCallbackFn = std::function<void(Event&)>;

	struct WindowData
	{
		WindowData() {};
		WindowData(std::string title, unsigned int width = 1280, unsigned int height = 720) 
		:Title(title), Width(width),Height(height)	{

		}
		std::string Title = "GEngine";
		unsigned int Width = 1280, Height = 720;
		bool VSync;

		EventCallbackFn EventCallback;
	};


	// Interface representing a desktop system based Window
	class GE_API Window
	{
	public:
		virtual ~Window();

		virtual void OnUpdate(bool m_Minimized) = 0;

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

        
        virtual void GetFrameBufferSize(int* width, int* height) = 0;

		virtual void Init(const WindowData& props) = 0;

		static Window* Create(const WindowData& props = WindowData());

		template<typename T>
		static Window* _CreateWindow(const WindowData& props);

		Scope<GraphicsContext> m_Context;

		inline static const FWindowApi GetWindowApi() { return Window::s_WindowApi; }

	private:
		static FWindowApi s_WindowApi;
	};

}
