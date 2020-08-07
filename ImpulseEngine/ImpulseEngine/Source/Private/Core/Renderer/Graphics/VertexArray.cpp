#include "gepch.h"
#include "Core/Renderer/Graphics/VertexArray.h"
#include "Core/Renderer/Graphics/GraphicsContext.h"

#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
#include "Core/Platform/Graphics/OpenGL/OpenGL_VertexArray.h"
#endif

#ifdef GE_GRAPHICS_API_VULKAN
#include "Core/Platform/Graphics/Vulkan/Vulkan_VertexArray.h"
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
		case GraphicsApi::OPENGL:
			return new OpenGL_VertexArray();
#endif

#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::VULKAN:
			return new Vulkan_VertexArray();
#endif
		default:
			GE_CORE_ASSERT("Graphics Api not implemented: {0}", GraphicsContext::GetGraphicsApi());
			return nullptr;
		}
	}

}
