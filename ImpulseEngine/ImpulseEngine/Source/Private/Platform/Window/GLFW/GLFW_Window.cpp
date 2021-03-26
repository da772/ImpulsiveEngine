#include "gepch.h"
#include "Public/Core/Renderer/Graphics/GraphicsContext.h"
#include "Public/Platform/Window/GLFW/GLFW_Window.h"
#include "Public/Core/Events/ApplicationEvent.h"
#include "Public/Core/Events/MouseEvent.h"
#include "Public/Core/Events/KeyEvent.h"
#include "Public/Core/Renderer/RenderCommand.h"
#include "Public/Core/Renderer/Renderer.h"
#include "Public/Core/Application/Application.h"

#ifdef GE_WINDOW_API_GLFW
#include <GLFW/glfw3.h>

namespace GEngine {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		GE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	GLFW_Window::GLFW_Window(const WindowData& props)
	{
	
	}

	GLFW_Window::~GLFW_Window()
	{
		Shutdown();
	}

	void GLFW_Window::Init(const WindowData& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		GE_CORE_INFO("GLFW API: Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);


		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			GE_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			if (GraphicsContext::GetGraphicsApi() == FGraphicsApi::VULKAN) {
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			}
			if (GraphicsContext::GetGraphicsApi() == FGraphicsApi::OPENGL) {
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
				const uint8_t samples = Application::GetRenderSettings().samples;
				if (samples)
					glfwWindowHint(GLFW_SAMPLES, samples);
			}

			s_GLFWInitialized = true;
		}

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		m_Context = Scope<GraphicsContext>(GraphicsContext::Create((const void*)m_Window));

		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (!mode)
			return;

		int monitorX, monitorY;
		glfwGetMonitorPos(monitor, &monitorX, &monitorY);

		int windowWidth, windowHeight;
		glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);

		glfwSetWindowPos(m_Window,
			monitorX + (mode->width - windowWidth) / 2,
			monitorY + (mode->height - windowHeight) / 2);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		
		

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			data.safe_top = 0;
			data.safe_bottom = 0;
			data.safe_left = 0;
			data.safe_right = 0;
			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});

		glfwSetErrorCallback([](int error, const char* description) {
			GE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
		});
        
        
        glfwGetWindowSize(m_Window, (int*)&m_Data.Width, (int*)&m_Data.Height);
        
	}

	void GLFW_Window::GetSafeArea(int* top, int* bottom, int* left, int* right)
	{
		glfwGetFramebufferSize(m_Window, right, top);
		*bottom = 0;
		*left = 0;
	}

	void GLFW_Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	float const GLFW_Window::GetTime() const
	{
		return (float)glfwGetTime();
	}

	void GLFW_Window::OnUpdate(bool m_Minimized)
	{

		glfwPollEvents();
		if (!m_Minimized)
			glfwSwapBuffers(m_Window);
		
	}
    
    void GLFW_Window::GetFrameBufferSize(int* width, int* height) {
        glfwGetFramebufferSize((GLFWwindow*)m_Window,width,height);
    }

    unsigned int GLFW_Window::GetWidth() const {
        int width, height;
        glfwGetWindowSize(m_Window, &width, &height);
        return width;
    }
    unsigned int GLFW_Window::GetHeight() const {
        int width, height;
        glfwGetWindowSize(m_Window, &width, &height);
        return height;
    }

	void GLFW_Window::SetTitle(const std::string& title)
	{
		glfwSetWindowTitle(m_Window, title.c_str());
	}

	void GLFW_Window::SetSize(uint16_t width, uint16_t height)
	{
		glfwSetWindowSize(m_Window, (int)width, (int)height);
	}

	void GLFW_Window::SetVSync(bool enabled)
	{
		if (GraphicsContext::GetGraphicsApi() == GraphicsApi::FGraphicsApi::OPENGL) glfwSwapInterval(enabled);
		m_Context->SetVSync(enabled);
		m_Data.VSync = enabled;
	}

	bool GLFW_Window::IsVSync() const
	{
		return m_Data.VSync;
	}

}
#endif
