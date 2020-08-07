#include "gepch.h"
#include "Public/Core/Platform/Window/GLFW/GLFW_Input.h"

#ifdef GE_WINDOW_API_GLFW
#include "Public/Core/Application/Application.h"
#include <GLFW/glfw3.h>

namespace GEngine {

	bool GLFW_Input::IsKeyPressedImpl(int keycode) {

		auto window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}


	bool GLFW_Input::IsMouseButtonPressedImpl(uint64_t button)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	float GLFW_Input::GetMouseXImpl(uint64_t id)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow());
		auto[x, y] = GetMousePositionImpl();
		return x;
	}

	float GLFW_Input::GetMouseYImpl(uint64_t id)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow());
		auto[x, y] = GetMousePositionImpl();
		return y;
	}

	std::pair<float, float> GLFW_Input::GetMousePositionImpl(uint64_t id)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetApp()->GetWindow()->GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return {(float) xpos,(float) ypos };
	}

}

#endif
