#include "gepch.h"
#include "Core/Renderer/Graphics/Buffer.h"
#include "Core/Renderer/Graphics/GraphicsContext.h"


#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
#include "Public/Platform/Graphics/OpenGL/OpenGL_Buffer.h"
#endif

#ifdef GE_GRAPHICS_API_VULKAN
#include "Public/Platform/Graphics/Vulkan/Vulkan_Buffer.h"
#endif

#ifdef GE_GRAPHICS_API_NONE
#include "Public/Platform/Graphics/Server/Empty_Buffer.h"
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
		case GraphicsApi::FGraphicsApi::OPENGL:
			return new OpenGL_VertexBuffer(vertices, size);
#endif
#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::FGraphicsApi::VULKAN:
			return new Vulkan_VertexBuffer(vertices, size);
#endif
#ifdef GE_GRAPHICS_API_NONE
		case GraphicsApi::FGraphicsApi::NONE:
			return new Empty_VertexBuffer(vertices, size);
#endif
			default:
				GE_CORE_ASSERT(false, "Graphics Api not implemented: {0}", (int)GraphicsContext::GetGraphicsApi());
				return nullptr;
		}
	}

	GEngine::VertexBuffer* VertexBuffer::Create(u32 size)
	{
		switch (GraphicsContext::GetGraphicsApi()) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::FGraphicsApi::OPENGL:
			return new OpenGL_VertexBuffer(size);
#endif
#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::FGraphicsApi::VULKAN:
			//return new Vulkan_VertexBuffer(vertices, size);
#endif
#ifdef GE_GRAPHICS_API_NONE
		case GraphicsApi::FGraphicsApi::NONE:
			return new Empty_VertexBuffer(size);
#endif
		default:
			GE_CORE_ASSERT(false, "Graphics Api not implemented: {0}", (int)GraphicsContext::GetGraphicsApi());
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
		case GraphicsApi::FGraphicsApi::OPENGL:
			return new OpenGL_IndexBuffer(indices, count);
#endif

#ifdef GE_GRAPHICS_API_VULKAN
		case GraphicsApi::FGraphicsApi::VULKAN:
			return new Vulkan_IndexBuffer(indices, count);
#endif
#ifdef GE_GRAPHICS_API_NONE
		case GraphicsApi::FGraphicsApi::NONE:
			return new Empty_IndexBuffer(indices, count);
#endif
		default:
			GE_CORE_ASSERT(false, "Graphics Api not implemented: {0}", (int)GraphicsContext::GetGraphicsApi());
			return nullptr;
		}
	}


	GEngine::Ref<GEngine::FrameBuffer> FrameBuffer::Create(int width, int height, int format,const char* texName, const float renderScale )
	{
		switch (GraphicsContext::GetGraphicsApi()) {
#if defined(GE_GRAPHICS_API_OPENGL_3_3) || defined(GE_GRAPHICS_API_OPENGL_ES)
		case GraphicsApi::FGraphicsApi::OPENGL:
			return std::make_shared<OpenGL_FrameBuffer>(width, height, format, texName, renderScale);
#endif
#ifdef GE_GRAPHICS_API_NONE
		case GraphicsApi::FGraphicsApi::NONE:
			return std::make_shared<Empty_FrameBuffer>(width, height, format, texName, renderScale);
#endif

		default:
			GE_CORE_ASSERT(false, "Graphics Api not implemented: {0}", (int)GraphicsContext::GetGraphicsApi());
			return nullptr;
		}
	}

}
