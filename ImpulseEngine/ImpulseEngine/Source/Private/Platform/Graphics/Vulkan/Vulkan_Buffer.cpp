#include "gepch.h"

#include "Public/Platform/Graphics/Vulkan/Vulkan_Buffer.h"

#ifdef GE_GRAPHICS_API_VULKAN

namespace GEngine {

	Vulkan_VertexBuffer::Vulkan_VertexBuffer(float* vertices, uint32_t size) :
		VertexBuffer(vertices, size)
	{
		m_Vertices = { vertices, vertices +  size };


	}

	Vulkan_VertexBuffer::~Vulkan_VertexBuffer()
	{

	}

	void Vulkan_VertexBuffer::Bind()
	{

	}

	void Vulkan_VertexBuffer::UnBind()
	{

	}

	Vulkan_IndexBuffer::Vulkan_IndexBuffer(uint32_t* indices, uint32_t size)
		: IndexBuffer(indices,  size), m_Indices(indices, indices+ size) 
	{

	}

	Vulkan_IndexBuffer::~Vulkan_IndexBuffer()
	{

	}

	void Vulkan_IndexBuffer::Bind()
	{

	}

	void Vulkan_IndexBuffer::UnBind()
	{

	}

}

#endif


