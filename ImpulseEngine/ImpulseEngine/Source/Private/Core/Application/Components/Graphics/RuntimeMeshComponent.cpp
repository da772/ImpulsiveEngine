#include "gepch.h"

#include "Public/Core/Application/Components/Graphics/RuntimeMeshComponent.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"
#include "Public/Core/Renderer/Graphics/VertexArray.h"
#include "Public/Core/Renderer/Material.h"
#include "Public/Core/Renderer/Renderer.h"
#include "Public/Core/Renderer/Pipeline/RenderPipeline.h"
#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/Graphics/Texture.h"

#include "Public/Core/Renderer/RenderCommand.h"



/*****
*	DEBUG
*	
*****/

#include "Public/Core/Application/SceneManager.h"
#include "Public/Core/Application/Scene.h"
#include "Public/Core/Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace GEngine {

	RuntimeMeshComponent::RuntimeMeshComponent(Entity* e) : Component(e)
	{
		go_tag = "Runtime Mesh Component";
	}

	RuntimeMeshComponent::~RuntimeMeshComponent()
	{
		if (m_Renderable != nullptr) {
			Renderer::GetPipeline("2d")->Remove(m_Renderable);
		}
	}


	void RuntimeMeshComponent::OnBegin()
	{
		CreateMesh();
	}

	void RuntimeMeshComponent::SetMeshData(const std::vector<float>& vertices, const std::vector<u32>& indices, const Ref<BufferLayout>& layout, const Ref<Material>& material)
	{
		this->m_Vertices = vertices;
		this->m_Indices = indices;
		this->m_Layout = layout;
		this->m_Material = material;
	}

	void RuntimeMeshComponent::CreateMesh()
	{
		if (m_Renderable != nullptr) {
			DestroyMesh();
		}

		m_VertexArray = std::shared_ptr<VertexArray>(VertexArray::Create());
		m_VertexBuffer = std::shared_ptr<GEngine::VertexBuffer>(GEngine::VertexBuffer::Create(m_Vertices.data(), sizeof(float) * m_Vertices.size()));;
		m_VertexBuffer->SetLayout(m_Layout);
		m_IndexBuffer = std::shared_ptr<GEngine::IndexBuffer>(GEngine::IndexBuffer::Create(m_Indices.data(), m_Indices.size()));
		
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_Renderable = std::shared_ptr<Renderable>((Renderable*)new RuntimeMeshRenderable(m_VertexArray, m_Material));

		Renderer::GetPipeline("2d")->Add(m_Renderable);

	}

	


	void RuntimeMeshComponent::DestroyMesh()
	{
		if (m_Renderable != nullptr) {
			Renderer::GetPipeline("2d")->Remove(m_Renderable);
			m_Renderable = nullptr;
			m_VertexArray = nullptr;
			m_IndexBuffer = nullptr;
			m_VertexBuffer = nullptr;
			m_Layout = nullptr;
		}
	}

	RuntimeMeshRenderable::RuntimeMeshRenderable(Ref<VertexArray> vertexArray, Ref<Material> material) : m_VertexArray(vertexArray), m_Material(material)
	{

	}

	RuntimeMeshRenderable::~RuntimeMeshRenderable()
	{
		GE_CORE_DEBUG("DESTROYING RENDERABLE");
		m_VertexArray = nullptr;
		m_Material = nullptr;
	}

	void RuntimeMeshRenderable::Render(Camera* cam)
	{
		m_Material->GetShader()->Bind();
		m_Material->GetShader()->UploadUniformMat4("u_ViewProjection", cam->GetViewProjectionMatrix());

		m_Material->GetShader()->UploadUniformFloat4("u_Color", { 1.f,1.f,1.f,1.f });
		m_Material->GetShader()->UploadUniformFloat2("u_UV", { 1.f,1.f });

		glm::mat4 transform = glm::translate(glm::mat4(1.f), { 0,0,0 }) * glm::scale(glm::mat4(1.0f), { 100.f,100.f, 1.f });
		m_Material->GetShader()->UploadUniformMat4("u_Transform", transform);

		m_Material->GetTexture()->Bind();
		m_VertexArray->Bind();

		RenderCommand::DrawIndexed(m_VertexArray);
	}

}