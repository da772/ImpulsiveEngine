#pragma once
#include "Core/Renderer/Graphics/VertexArray.h"

namespace GEngine {

	class GE_API Empty_VertexArray : public VertexArray {

	public :
		inline Empty_VertexArray() {};
		inline virtual ~Empty_VertexArray() {};
		inline virtual void Bind() const override {};
		inline virtual void UnBind() override {};
		inline virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override {};
		inline virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override {};
		virtual inline const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() override { return {}; };
		virtual inline const Ref<IndexBuffer>& GetIndexBuffer() override { return nullptr; };

	};

}