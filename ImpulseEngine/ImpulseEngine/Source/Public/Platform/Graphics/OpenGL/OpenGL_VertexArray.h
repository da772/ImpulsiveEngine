#pragma once
#include "Core/Renderer/Graphics/VertexArray.h"

namespace GEngine {

	class OpenGL_VertexArray : public VertexArray {

	public :
		OpenGL_VertexArray();
		virtual ~OpenGL_VertexArray();

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

	};

}