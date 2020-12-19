#include "gepch.h"
#include "Public/Core/Renderer/RendererApi.h"
#include "Core/Renderer/Graphics/GraphicsContext.h"

#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
#include "Public/Platform/Graphics/OpenGL/OpenGL_RendererApi.h"
#endif

#ifdef GE_GRAPHICS_API_VULKAN
#include "Public/Platform/Graphics/Vulkan/Vulkan_RendererApi.h"
#endif

namespace GEngine {

	unordered_map<int, int> RendererApi::s_BlendFactors;

	GEngine::RendererApi* RendererApi::Create()
	{
		FGraphicsApi api = GraphicsContext::GetGraphicsApi();

		switch (api) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::FGraphicsApi::OPENGL:
			return new OpenGL_RendererApi();
#endif

#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::FGraphicsApi::VULKAN:
			return new Vulkan_RendererApi();
#endif

		default:
			GE_CORE_ASSERT(false, "Invalid Graphics API for RendererApi");
			return nullptr;
		}

	}

}
