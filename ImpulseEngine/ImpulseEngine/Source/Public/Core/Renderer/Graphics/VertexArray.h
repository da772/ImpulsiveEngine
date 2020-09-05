#pragma once
#include "Public/Core/Renderer/Graphics/Buffer.h"


namespace GEngine {
	class VertexArray {
	public:
		virtual ~VertexArray();

		virtual void Bind() const = 0;
		virtual void UnBind() = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)  = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;
		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() = 0;

		inline uint32_t GetVertexCount() const { return m_VertexCount; }
		inline void SetVertexCount(const uint32_t count) { m_VertexCount = count; }

		static VertexArray* Create();

	protected:
		
		uint32_t m_VertexCount = 0;


	};
}