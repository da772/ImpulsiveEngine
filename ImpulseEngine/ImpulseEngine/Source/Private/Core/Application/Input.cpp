#include "gepch.h"

#include "Public/Core/Application/Input.h"
#include "Public/Core/Application/Window.h"
#include "Public/Core/Events/KeyEvent.h"
#include "Public/Core/Events/MouseEvent.h"

#ifdef GE_WINDOW_API_GLFW
#include "Public/Platform/Window/GLFW/GLFW_Input.h"
#endif

#ifdef GE_WINDOW_API_MOBILE
#include "Public/Platform/Window/Mobile/Mobile_Input.h"
#endif

#ifdef GE_WINDOW_API_SERVER
#include "Public/Platform/Window/Server/Server_Input.h"
#endif

#include "Public/Core/Application/Application.h"

namespace GEngine {

	std::unordered_map<int, bool> Input::m_keyEvent;
	std::unordered_map<int, bool> Input::m_mouseEvent;

	Input* Input::s_Instance = nullptr;

	void Input::Flush()
	{
		m_keyEvent.clear();
		m_mouseEvent.clear();
	}

	bool Input::IsKeyPressed(int keycode)
	{
		if (!Application::InputEnabled()) return false;
		const auto& it = m_keyEvent.find(keycode);
		if (it != m_keyEvent.end()) {
			return it->second;
		}
		return false;
	}

	bool Input::IsKeyReleased(int keycode)
	{
		if (!Application::InputEnabled()) return false;
		const auto& it = m_keyEvent.find(keycode);
		if (it != m_keyEvent.end()) {
			return !it->second;
		}
		return false;
	}

	bool Input::IsKeyDown(int keycode)
	{
		if (!Application::InputEnabled()) return false;
		return s_Instance->IsKeyPressedImpl(keycode);
	}

	bool Input::IsMouseButtonDown(int button)
	{
		if (!Application::InputEnabled()) return false;
		return s_Instance->IsMouseButtonPressedImpl(button);
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		if (!Application::InputEnabled()) return false;
		const auto& it = m_mouseEvent.find(button);
		if (it != m_mouseEvent.end()) {
			return it->second;
		}
		return false;
	}

	bool Input::IsMouseButtonReleased(int button)
	{
		if (!Application::InputEnabled()) return false;
		const auto& it = m_mouseEvent.find(button);
		if (it != m_mouseEvent.end()) {
			return !it->second;
		}
		return false;
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
		//m_mouseEvent.clear();
		//m_keyEvent.clear();
		if (!Application::InputEnabled()) return;
		if (s_Instance != nullptr)
			s_Instance->ProcessEvent(e);
		if (e.GetEventType() == EventType::KeyPressed) {
			const KeyPressedEvent& k = (const KeyPressedEvent&)e;
			if (!k.GetRepeatCount())
				m_keyEvent[k.GetKeyCode()] = true;
		}
		else if (e.GetEventType() == EventType::KeyReleased) {
			const KeyReleasedEvent& k = (const KeyReleasedEvent &)e;
			m_keyEvent[k.GetKeyCode()] = false;
		} else if (e.GetEventType() == EventType::MouseButtonPressed) {
			const MouseButtonPressedEvent& k = (const MouseButtonPressedEvent&)e;
			m_mouseEvent[k.GetMouseButton()] = true;
		}
		else if (e.GetEventType() == EventType::MouseButtonReleased) {
			const MouseButtonReleasedEvent& k = (const MouseButtonReleasedEvent&)e;
			m_mouseEvent[k.GetMouseButton()] = false;
		}

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
#ifdef GE_WINDOW_API_SERVER
		case WindowApi::FWindowApi::SERVER:
			s_Instance = new Server_Input();
			break;
#endif
		default:
			GE_CORE_ERROR( "Invalid Input for current Window Api: {0}", (int)Window::GetWindowApi());
			break;
		}


		return s_Instance;



	}


}
