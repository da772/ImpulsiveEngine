#include "gepch.h"
#include "Public/Core/Renderer/Renderer2D.h"

#include "Public/Core/Renderer/Graphics/VertexArray.h"
#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/Camera.h"
#include "Public/Core/Renderer/Graphics/Texture.h"

#include "Public/Core/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace GEngine {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Shader* TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Scope<Renderer2DStorage> sData;

	void Renderer2D::Init()
	{
		sData.reset(new Renderer2DStorage());

		sData->QuadVertexArray.reset(GEngine::VertexArray::Create());

		float sqvertices[20] = { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f };

		std::shared_ptr<GEngine::VertexBuffer> sqVB = std::shared_ptr<GEngine::VertexBuffer>(GEngine::VertexBuffer::Create(sqvertices, sizeof(sqvertices)));
		sqVB->SetLayout(std::shared_ptr<BufferLayout>( new BufferLayout({ {  GEngine::ShaderDataName::Position }, { GEngine::ShaderDataName::TextureCoord} } )));
		sData->QuadVertexArray->AddVertexBuffer(sqVB);

		uint32_t sqindices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<GEngine::IndexBuffer> sqib = std::shared_ptr<GEngine::IndexBuffer>(GEngine::IndexBuffer::Create(sqindices, sizeof(sqindices) / sizeof(u32)));
		sData->QuadVertexArray->SetIndexBuffer(sqib);
		
		sData->WhiteTexture.reset();
		sData->WhiteTexture = Texture2D::Create("whiteTexture",1,1);
		uint32_t whiteTextureData = 0xffffffff;
		sData->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		sData->TextureShader = Shader::Create("Content/shaders/TextureShader.glsl");
		sData->TextureShader->Bind();
		sData->TextureShader->UploadUniformInt("u_Texture", 0);

	}

	void Renderer2D::Shutdown()
	{
		if (sData != nullptr) {
			Shader::Destroy(sData->TextureShader);
		}
		sData.reset();
	}

	void Renderer2D::BeginScene(const Camera* camera)
	{
		sData->TextureShader->Bind();
		sData->TextureShader->UploadUniformMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		sData->TextureShader->UploadUniformFloat4("u_Color", color);
		sData->TextureShader->UploadUniformFloat2("u_UV", { 1.f,1.f });
        sData->TextureShader->UploadUniformFloat("u_Threshold", .5f);
		

		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::scale(glm::mat4(1.0f), { size.x,size.y, 1.f });
		sData->TextureShader->UploadUniformMat4("u_Transform", transform);

		sData->WhiteTexture->Bind();
		sData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(sData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, Texture2D* texture, float alphaTolerance, const glm::vec2& uvScale)
	{
		DrawQuad({ position.x, position.y, 0.f }, size, color, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Texture2D* texture, float alphaTolerance, const glm::vec2& uvScale)
	{
		sData->TextureShader->UploadUniformFloat4("u_Color", color);
		sData->TextureShader->UploadUniformFloat2("u_UV", uvScale);
        sData->TextureShader->UploadUniformFloat("u_alphaTolerance", alphaTolerance);

		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::scale(glm::mat4(1.0f), { size.x,size.y, 1.f });
		sData->TextureShader->UploadUniformMat4("u_Transform", transform);

		texture->Bind();

		sData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(sData->QuadVertexArray);
	}



}
