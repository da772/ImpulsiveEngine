#include "gepch.h"
#include "Core/Renderer/Graphics/VertexArray.h"
#include "Core/Renderer/Graphics/GraphicsContext.h"

#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
#include "Public/Platform/Graphics/OpenGL/OpenGL_VertexArray.h"
#endif

#ifdef GE_GRAPHICS_API_VULKAN
#include "Public/Platform/Graphics/Vulkan/Vulkan_VertexArray.h"
#endif

#ifdef GE_GRAPHICS_API_NONE
#include "Public/Platform/Graphics/Server/Empty_VertexArray.h"
#endif


namespace GEngine {

	VertexArray::~VertexArray()
	{
		//GE_CORE_DEBUG("DESTROYING VERTEX ARRAY");
	}

	GEngine::VertexArray* VertexArray::Create()
	{
		switch (GraphicsContext::GetGraphicsApi()) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::FGraphicsApi::OPENGL:
			return new OpenGL_VertexArray();
#endif

#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::FGraphicsApi::VULKAN:
			return new Vulkan_VertexArray();
#endif
#ifdef GE_GRAPHICS_API_NONE
		case GraphicsApi::FGraphicsApi::NONE:
			return new Empty_VertexArray();
#endif
		default:
			GE_CORE_ASSERT("Graphics Api not implemented: {0}", (int)GraphicsContext::GetGraphicsApi());
			return nullptr;
		}
	}

}
