#include "gepch.h"

#include "Public/Core/Application/Input.h"
#include "Public/Core/Application/Window.h"

#ifdef GE_WINDOW_API_GLFW
#include "Public/Core/Platform/Window/GLFW/GLFW_Input.h"
#endif

#ifdef GE_WINDOW_API_MOBILE
#include "Public/Core/Platform/Window/Mobile/Mobile_Input.h"
#endif


namespace GEngine {

	Input* Input::s_Instance = nullptr;

	void Input::ProcessEvents(const Event& e)
	{
		if (s_Instance != nullptr)
			s_Instance->ProcessEvent(e);
	}

	void Input::Create()
	{
		if (s_Instance != nullptr) {
			delete Input::s_Instance;
			s_Instance = nullptr;
		}

		switch (Window::GetWindowApi()) {
#ifdef GE_WINDOW_API_GLFW
		case WindowApi::FWindowApi::GLFW:
			s_Instance = new GLFW_Input();
			break;
#endif
#ifdef GE_WINDOW_API_MOBILE
		case WindowApi::FWindowApi::MOBILE:
			s_Instance = new Mobile_Input();
			break;
#endif
		default:
			GE_CORE_ERROR( "Invalid Input for current Window Api: {0}", (int)Window::GetWindowApi());
			break;
		}




	}

	
}
