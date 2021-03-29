#pragma once

#include "Public/Core/Renderer/Graphics/GraphicsContext.h"

struct GLFWwindow;

namespace GEngine {
	
	class GE_API Empty_GraphicsContext : public GraphicsContext {

	public:
		inline Empty_GraphicsContext(const void* windowHandle) {};
        inline ~Empty_GraphicsContext() {};

		inline virtual void Init() override {};
		inline virtual void SwapBuffers() override {};
		inline virtual void SetVSync(bool vsync) override {};

	};

}
