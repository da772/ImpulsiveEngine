#include "gepch.h"
#ifdef GE_WINDOW_API_GLFW
#include "Public/Core/Platform/Window/GLFW/GLFW_Input.h"


#include "Public/COre/Collision/CollisionDetection.h"
#include "Public/Core/Application/Application.h"
#include <GLFW/glfw3.h>
#include "Public/Core/Events/MouseEvent.h"
#include "Public/Core/Events/Event.h"
#include "Public/Core/Util/GEMath.h"
#include "Public/Core/Util/ThreadPool.h"


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

	void GLFW_Input::ProcessEvent(const Event& e)
	{
		if (e.GetEventType() == EventType::MouseMoved) {
			
			const MouseMovedEvent& mouse = (MouseMovedEvent&)e;
			int width, height;
			Application::GetApp()->GetWindow()->GetFrameBufferSize(&width, &height);
				CollisionDetection::InteractionUI(
					GEMath::MapRange(mouse.GetX() / (float)width, 0, 1, -1, 1),
					-GEMath::MapRange(mouse.GetY() / (float)height, 0, 1, -1, 1));
		}
	}

}

#endif
