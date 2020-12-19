#include "gepch.h"
#ifdef GE_GRAPHICS_API_VULKAN
#include "Public/Platform/Graphics/Vulkan/Vulkan_RendererApi.h"
#include "Public/Platform/Graphics/Vulkan/Vulkan_GraphicsContext.h"



namespace GEngine {

	void Vulkan_RendererApi::Init()
	{

	}

	void Vulkan_RendererApi::SetClearColor(const Vector4f& color)
	{
		Vulkan_GraphicsContext::SetClearColor(color);
	}

	void Vulkan_RendererApi::Clear()
	{
		//Vulkan_GraphicsContext::Clear();
	}

	void Vulkan_RendererApi::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		Vulkan_GraphicsContext::Resized();
	}

	void Vulkan_RendererApi::DrawIndexed(const Ref<VertexArray>& vertexArray, u32 size)
	{

	}

	void Vulkan_RendererApi::DrawArrays(const Ref<VertexArray>& vertexArray)
	{

	}

}

#endif
