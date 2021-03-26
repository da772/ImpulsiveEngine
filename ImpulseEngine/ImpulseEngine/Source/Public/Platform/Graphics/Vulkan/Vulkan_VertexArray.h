#pragma once
#include "Core/Renderer/Graphics/VertexArray.h"
#ifdef GE_GRAPHICS_API_VULKAN
#ifdef GE_GRAPHICS_API_VULKAN
#include "Public/Platform/Graphics/Vulkan/Vulkan_GraphicsContext.h"
#endif
namespace GEngine {

	class GE_API Vulkan_VertexArray : public VertexArray {

	public:
		Vulkan_VertexArray();
		virtual ~Vulkan_VertexArray() ;

		virtual void Bind() const override;
		virtual void UnBind() override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual inline const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() override { return m_VertexBuffers; };
		virtual inline const Ref<IndexBuffer>& GetIndexBuffer() override { return m_IndexBuffer; };

	private:
		uint32_t m_RendererID = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;

	private:
#ifdef GE_GRAPHICS_API_VULKAN
		
#endif

	};

}

#endif
