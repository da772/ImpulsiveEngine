#pragma once
#include "Public/Core/Renderer/Graphics/Buffer.h"

namespace GEngine {


	class GE_API Empty_VertexBuffer : public VertexBuffer {
	public:
		inline Empty_VertexBuffer(float* vertices, uint32_t size) : VertexBuffer(vertices, size) {};
		inline Empty_VertexBuffer(uint32_t size) : VertexBuffer(size) {};
		inline ~Empty_VertexBuffer() {};
		inline virtual void Bind() override {};
		inline virtual void UnBind() override {};
		virtual inline void SetLayout(Ref<BufferLayout> layout) override { };
		virtual inline Ref<BufferLayout> GetLayout() const override { return nullptr; };
		virtual void SetVertices(float* vertices, uint32_t size) override {};
		virtual void SetData(const void* data, u32 size) override {};
	};

	class GE_API Empty_IndexBuffer : public IndexBuffer {
	public:
		inline Empty_IndexBuffer(uint32_t* indices, uint32_t size) : IndexBuffer(indices, size) {};
		inline ~Empty_IndexBuffer() {};
		inline virtual void Bind() override {};
		inline virtual void UnBind() override {};
	};

	class GE_API Empty_FrameBuffer : public FrameBuffer {

	public:
		inline Empty_FrameBuffer(int width, int height, int format, const char* texName = "", const float renderScale= 1.f) : FrameBuffer(width, height, format, texName, renderScale) {};
		inline virtual ~Empty_FrameBuffer() {};

		inline void Bind() override {};
		inline void UnBind() override {};
		inline void UpdateSize(int width, int height) override {};
		inline void Unload() override {};
		inline void Reload() override {};
	private:
		


	};


}