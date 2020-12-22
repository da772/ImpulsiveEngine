#include "gepch.h"

#include "Public/Core/Application/Input.h"
#include "Public/Core/Application/Window.h"

#ifdef GE_WINDOW_API_GLFW
#include "Public/Platform/Window/GLFW/GLFW_Input.h"
#endif

#ifdef GE_WINDOW_API_MOBILE
#include "Public/Platform/Window/Mobile/Mobile_Input.h"
#endif

#include "Public/Core/Application/Application.h"

namespace GEngine {

	Input* Input::s_Instance = nullptr;

	bool Input::IsKeyPressed(int keycode)
	{
		if (!Application::InputEnabled()) return false;
		return s_Instance->IsKeyPressedImpl(keycode); 
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		if (!Application::InputEnabled()) return false;
		return s_Instance->IsMouseButtonPressedImpl(button);
	}

	float Input::GetMouseX()
	{
		if (!Application::InputEnabled()) return 0;
		return s_Instance->GetMouseXImpl() - Application::GetViewPortOffsetX();
	}

	float Input::GetMouseY()
	{
		if (!Application::InputEnabled()) return 0;
		return s_Instance->GetMouseYImpl() - Application::GetViewPortOffsetY();
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		if (!Application::InputEnabled()) return { 0.f,0.f };
		std::pair<float, float> pair = s_Instance->GetMousePositionImpl();
		return { pair.first - Application::GetViewPortOffsetX(), pair.second - Application::GetViewPortOffsetY() };
	}

	void Input::ProcessEvents(const Event& e)
	{
		if (!Application::InputEnabled()) return;
		if (s_Instance != nullptr)
			s_Instance->ProcessEvent(e);
	}

	Input* Input::Create()
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
