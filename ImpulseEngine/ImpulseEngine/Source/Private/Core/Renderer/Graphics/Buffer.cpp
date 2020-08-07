#include "gepch.h"
#include "Core/Renderer/Graphics/Buffer.h"
#include "Core/Renderer/Graphics/GraphicsContext.h"


#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
#include "Core/Platform/Graphics/OpenGL/OpenGL_Buffer.h"
#endif

#ifdef GE_GRAPHICS_API_VULKAN
#include "Core/Platform/Graphics/Vulkan/Vulkan_Buffer.h"
#endif


namespace GEngine {

	VertexBuffer::~VertexBuffer()
	{
		//GE_CORE_DEBUG("DESTROYING VERTEX BUFFER");
	}

	GEngine::VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (GraphicsContext::GetGraphicsApi()) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::OPENGL:
			return new OpenGL_VertexBuffer(vertices, size);
#endif
#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::VULKAN:
			return new Vulkan_VertexBuffer(vertices, size);
#endif
			default:
				GE_CORE_ASSERT(false, "Graphics Api not implemented: {0}", GraphicsContext::GetGraphicsApi());
				return nullptr;
		}
	}

	GEngine::VertexBuffer* VertexBuffer::Create(u32 size)
	{
		switch (GraphicsContext::GetGraphicsApi()) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::OPENGL:
			return new OpenGL_VertexBuffer(size);
#endif
#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::VULKAN:
			//return new Vulkan_VertexBuffer(vertices, size);
#endif
		default:
			GE_CORE_ASSERT(false, "Graphics Api not implemented: {0}", GraphicsContext::GetGraphicsApi());
			return nullptr;
		}
	}

	IndexBuffer::~IndexBuffer()
	{
		//GE_CORE_DEBUG("DESTROYING INDEX BUFFER");
	}

	GEngine::IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (GraphicsContext::GetGraphicsApi()) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::OPENGL:
			return new OpenGL_IndexBuffer(indices, count);
#endif

#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::VULKAN:
			return new Vulkan_IndexBuffer(indices, count);
#endif
		default:
			GE_CORE_ASSERT(false, "Graphics Api not implemented: {0}", GraphicsContext::GetGraphicsApi());
			return nullptr;
		}
	}


}
