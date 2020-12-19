#include "gepch.h"
#include "Public/Platform/Graphics/Vulkan/Vulkan_VertexArray.h"


#ifdef GE_GRAPHICS_API_VULKAN

#include "Public/Platform/Graphics/Vulkan/Vulkan_Buffer.h"

namespace GEngine {

	Vulkan_VertexArray::Vulkan_VertexArray()
	{
		Vulkan_GraphicsContext::GenVertexStructure(m_RendererID);
	}

	Vulkan_VertexArray::~Vulkan_VertexArray()
	{
		if (m_RendererID)	
			Vulkan_GraphicsContext::DestroyVertexStructure(m_RendererID);
	}

	void Vulkan_VertexArray::Bind() const
	{
		Vulkan_GraphicsContext::BindVertexStructure(m_RendererID);
	}

	void Vulkan_VertexArray::UnBind() 
	{
		Vulkan_GraphicsContext::DestroyVertexStructure(m_RendererID);
	}

	void Vulkan_VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		Vulkan_VertexBuffer* vb = (Vulkan_VertexBuffer*)vertexBuffer.get();
		Vulkan_GraphicsContext::SetPipelineVertices(m_RendererID, vb->GetVertices());

		// Set layout here

	}

	void Vulkan_VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		Vulkan_IndexBuffer* vb = (Vulkan_IndexBuffer*)indexBuffer.get();
		Vulkan_GraphicsContext::SetPipelineIndices(m_RendererID, vb->GetIndices());
	}

}

#endif

