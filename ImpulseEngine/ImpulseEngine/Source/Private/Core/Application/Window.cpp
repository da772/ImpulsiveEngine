#include "gepch.h"
#include "Public/Core/Application/Window.h"
#include "Public/Core/Application/Application.h"
#ifdef GE_WINDOW_API_GLFW
#include "Public/Core/Platform/Window/GLFW/GLFW_Window.h"
#endif
#ifdef GE_WINDOW_API_WIN32
#include "Public/Core/Platform/Window/WIN32/WIN32_Window.h"

#endif
#ifdef GE_WINDOW_API_MOBILE
#include "Public/Core/Platform/Window/Mobile/MobileWindow.h"
#endif


namespace GEngine {

	FWindowApi Window::s_WindowApi = FWindowApi::NONE;

	template<typename T>
	Window* Window::_CreateWindow(const WindowData& props)
	{
		Window* window = new T(props);
		window->Init(props);
		return window;
	}


	Window::~Window()
	{
	}

	Window* Window::Create(const WindowData& props)
	{
        Window::s_WindowApi = Application::GetApp()->GetWindowApi();
        switch (Window::s_WindowApi) {
            #ifdef GE_WINDOW_API_GLFW
            case FWindowApi::GLFW:
				return Window::_CreateWindow<GLFW_Window>(props);
            #endif
            #ifdef GE_WINDOW_API_WIN32
            case FWindowApi::WIN32API:
				return  Window::_CreateWindow<WIN32_Window>(props);
            #endif
            #ifdef GE_WINDOW_API_MOBILE
            case FWindowApi::MOBILE:
                return Window::_CreateWindow<MobileWindow>(props);
            #endif
            case FWindowApi::NONE:
            default:
                GE_CORE_ASSERT(false, "Window Api invalid on current platform : {0}",(int) Window::s_WindowApi);
				return nullptr;
        }
		
	}

}
