#include "gepch.h"

#include "Public/Core/Platform/Graphics/OpenGL/OpenGL_GraphicsContext.h"

#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
#ifdef GE_GRAPHICS_API_OPENGL_3_3)
#include <GLFW/glfw3.h>
#include <GLFW/deps/linmath.h>
#include <glad/glad.h>
#endif
#if defined (GE_GRAPHICS_API_OPENGL_ES)
#if defined(GE_PLATFORM_ANDROID)
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#else
#include <OpenGLES/ES3/gl.h>
#endif
#endif



#include "glm/glm.hpp"

namespace GEngine {


	OpenGL_GraphicsContext::OpenGL_GraphicsContext(const void* windowHandle)
		: m_WindowHandle((GLFWwindow*)windowHandle), GraphicsContext()
	{
		//GE_CORE_ASSERT(windowHandle, "Window Handle is null!");
	}

    OpenGL_GraphicsContext::~OpenGL_GraphicsContext() {
#if defined(GE_GRAPHICS_API_OPENGL_3_3)
		//glfwMakeContextCurrent(NULL);
		
#endif
    }
    
	void OpenGL_GraphicsContext::Init()
	{
        int status = 1;
        #if defined(GE_GRAPHICS_API_OPENGL_3_3)
		glfwMakeContextCurrent(m_WindowHandle);
		status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GE_CORE_ASSERT(status, "Failed to initialize Glad!");
        #endif
		if (status)
			GE_CORE_INFO("OpenGL Renderer: {0}, {1}, {2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
        
        
		int width = 0, height = 0;
        #if defined(GE_GRAPHICS_API_OPENGL_3_3)
		glfwGetFramebufferSize(m_WindowHandle, &width, &height);
		glViewport(0, 0, width, height);
        glEnable(GL_MULTISAMPLE);
        #endif
		

	}

	void OpenGL_GraphicsContext::SwapBuffers()
	{
		
	}


}

#endif


