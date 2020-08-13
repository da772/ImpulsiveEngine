#include "gepch.h"
#include "Core/Renderer/Graphics/GraphicsContext.h"
#include "Core/Application/Application.h"
#include "Core/Renderer/RenderCommand.h"
#include "Core/Application/Input.h"

#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
#include "Core/Platform/Graphics/OpenGL/OpenGL_GraphicsContext.h"
#endif

#ifdef GE_GRAPHICS_API_VULKAN
#include "Core/Platform/Graphics/Vulkan/Vulkan_GraphicsContext.h"
#endif

namespace GEngine {
	FGraphicsApi GraphicsContext::s_GraphicsApi = GraphicsApi::FGraphicsApi::NONE;

	GraphicsContext::GraphicsContext()
	{
		Input::Create();
		
	}

	GraphicsContext::~GraphicsContext()
	{
		Renderer::Shutdown();
	}

	template<typename T>
	GraphicsContext* GraphicsContext::_CreateContext(const void* window)
	{
		GraphicsContext* context = new T(window);
		context->Init();
		Renderer::Init();
		return context;
	}


	GraphicsContext* GraphicsContext::Create(const void* window) {
		GraphicsContext::s_GraphicsApi = Application::GetApp()->GetGraphicsApi();
		switch (GraphicsContext::s_GraphicsApi) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::FGraphicsApi::OPENGL: {
			return GraphicsContext::_CreateContext<OpenGL_GraphicsContext>(window);
		}
#endif
#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::FGraphicsApi::VULKAN: {
			return GraphicsContext::_CreateContext<Vulkan_GraphicsContext>(window);
		}
#endif

		default:
			GE_CORE_ASSERT("Invalid graphics api for current platform. {0}",(int) GraphicsContext::s_GraphicsApi);
			return nullptr;
		}

	}

}
