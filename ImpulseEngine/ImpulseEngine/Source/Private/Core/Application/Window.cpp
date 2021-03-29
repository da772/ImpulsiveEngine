#include "gepch.h"
#include "Public/Core/Application/Window.h"
#include "Public/Core/Application/Application.h"
#include "Public/Core/Util/GEMath.h"
#include "Public/Core/Application/Input.h"

#ifdef GE_WINDOW_API_GLFW
#include "Public/Platform/Window/GLFW/GLFW_Window.h"
#endif
#ifdef GE_WINDOW_API_WIN32
#include "Public/Platform/Window/WIN32/WIN32_Window.h"

#endif
#ifdef GE_WINDOW_API_MOBILE
#include "Public/Platform/Window/Mobile/MobileWindow.h"
#endif

#ifdef GE_WINDOW_API_SERVER
#include "Public/Platform/Window/Server/Server_Window.h"
#endif


namespace GEngine {
FWindowApi Window::s_WindowApi = GE_WINDOW_API_DEFAULT;


	template<typename T>
	Window* Window::_CreateWindow(const WindowData& props)
	{
		Window* window = new T(props);
		window->Init(props);
		Input::Create();
		return window;
	}


	void Window::GetSafeAreaUI(float* top, float* bottom, float* left, float* right)
	{
		*top = ((float)m_Data.safe_top / (float)m_Data.Height);
		*bottom = ((float)m_Data.safe_bottom/ (float)m_Data.Height);
		*left = ((float)m_Data.safe_left/ (float)m_Data.Width);
		*right = ((float)m_Data.safe_right / (float)m_Data.Width);
	}

	Window::~Window()
	{
	}

	void Window::OnUpdate(bool m_Minimized)
	{

	}

	void Window::GetSafeArea(int* top, int* bottom, int* left, int* right)
	{
		*top = 0;
		*bottom = 0;
		*left = 0;
		*right = 0;
	}
	
	Window* Create_Window(const WindowData& props)
	{
        FWindowApi s_WindowApi = Window::GetWindowApi();
        
        switch (s_WindowApi) {
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
#ifdef GE_WINDOW_API_SERVER
            case FWindowApi::SERVER:
				return Window::_CreateWindow<ServerWindow>(props);
	#endif
			case FWindowApi::NONE:
            default:
                GE_CORE_ASSERT(false, "Window Api invalid on current platform : {0}",(int) s_WindowApi);
				return nullptr;
        }
		
	}
	
	const float WindowData::GetSafeTopUI() const
	{
		return (float)((float)safe_top / (float)Height);
	}

	const float WindowData::GetSafeBottomUI() const
	{
		return (float)((float)safe_bottom / (float)Height);
	}

	const float WindowData::GetSafeLeftUI() const
	{
		return (float)((float)safe_left / (float)Width);
	}

	const float WindowData::GetSafeRightUI() const
	{
		return (float)((float)safe_right / (float)Width);
	}

}
