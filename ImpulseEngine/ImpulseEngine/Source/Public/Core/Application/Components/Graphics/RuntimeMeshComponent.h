#pragma once

#include "Public/Core/Application/Component.h"
#include "Public/Core/Renderer/Graphics/Renderable.h"

namespace GEngine {
	
	
	class BufferLayout;
	class Material;
	class IndexBuffer;
	class VertexBuffer;
	class VertexArray;

	class RuntimeMeshComponent : public Component {

	public:
		RuntimeMeshComponent();
		virtual ~RuntimeMeshComponent();

		virtual void OnBegin() override;

		void SetMeshData(const std::vector<float>& vertices, const std::vector<u32>& indices, Ref<BufferLayout> layout, Ref<Material> material);
		void CreateMesh();
		void DestroyMesh();

	protected:
		Ref<Renderable> m_Renderable;
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		Ref<Material> m_Material;
		Ref<BufferLayout> m_Layout;

		std::vector<float> m_Vertices;
		std::vector<u32> m_Indices;


	};


	class RuntimeMeshRenderable : Renderable {

	public:
		RuntimeMeshRenderable() {};
		RuntimeMeshRenderable(Ref<VertexArray> vertexArray, Ref<Material> material);
		virtual ~RuntimeMeshRenderable();

		virtual void Render() override;
	private:
		Ref<VertexArray> m_VertexArray;
		Ref<Material> m_Material;

	};





}
