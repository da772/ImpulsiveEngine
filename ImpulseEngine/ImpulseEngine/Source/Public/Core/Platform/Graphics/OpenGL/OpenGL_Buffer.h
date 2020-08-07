#pragma once
#include "Public/Core/Renderer/Graphics/Buffer.h"

namespace GEngine {


	class OpenGL_VertexBuffer : public VertexBuffer {
	public:
		OpenGL_VertexBuffer(float* vertices, uint32_t size);
		OpenGL_VertexBuffer(uint32_t size);
		~OpenGL_VertexBuffer();
		virtual void Bind() override;
		virtual void UnBind() override;
		virtual inline void SetLayout(Ref<BufferLayout> layout) override { m_Layout = layout; };
		virtual inline Ref<BufferLayout> GetLayout() const override { return m_Layout; };
		virtual void SetVertices(float* vertices, uint32_t size) override;
		virtual void SetData(const void* data, u32 size) override;

	private:
		uint32_t m_RendererID = 0;
		Ref<BufferLayout> m_Layout;
	};

	class OpenGL_IndexBuffer : public IndexBuffer {
	public:
		OpenGL_IndexBuffer(uint32_t* indices, uint32_t size);
		~OpenGL_IndexBuffer();
		virtual void Bind() override;
		virtual void UnBind() override;
	private:
		uint32_t m_RendererID = 0;
	};


}