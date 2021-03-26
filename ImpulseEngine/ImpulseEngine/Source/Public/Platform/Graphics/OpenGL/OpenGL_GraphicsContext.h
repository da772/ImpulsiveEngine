#pragma once

#include "Public/Core/Renderer/Graphics/GraphicsContext.h"

struct GLFWwindow;

namespace GEngine {
	
	class GE_API OpenGL_GraphicsContext : public GraphicsContext {

	public:
		OpenGL_GraphicsContext(const void* windowHandle);
        ~OpenGL_GraphicsContext();

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void SetVSync(bool vsync) override {};

	private:
		GLFWwindow* m_WindowHandle;
	};

}
