#pragma once
#include "Core/Renderer/Graphics/Buffer.h"

namespace GEngine {

#ifdef GE_GRAPHICS_API_VULKAN

	class GE_API Vulkan_VertexBuffer : public VertexBuffer {
	public:
		Vulkan_VertexBuffer(float* vertices, uint32_t size);
		~Vulkan_VertexBuffer();
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual inline void SetLayout(Ref <BufferLayout> layout) override { m_Layout = layout; };
		virtual inline Ref<BufferLayout> GetLayout() const override { return m_Layout; };
		inline const std::vector<float>& GetVertices() { return m_Vertices; }
		virtual void SetVertices(float* vertices, uint32_t size) override {};

		void SetData(const void* data, uint32_t size) override;

	private:
		uint32_t m_RendererID = 0;
		Ref<BufferLayout> m_Layout;
		std::vector<float> m_Vertices;
	};

	class GE_API Vulkan_IndexBuffer : public IndexBuffer {
	public:
		Vulkan_IndexBuffer(uint32_t* indices, uint32_t size);
		~Vulkan_IndexBuffer();
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual inline uint32_t GetCount() const { return m_Count; };
		inline const std::vector<uint32_t>& GetIndices() { return m_Indices; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_Count = 0;
		std::vector<uint32_t> m_Indices;
	};
#endif

}