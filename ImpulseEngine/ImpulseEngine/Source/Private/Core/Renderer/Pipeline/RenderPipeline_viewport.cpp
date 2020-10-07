#include "gepch.h"

#include "Public/Core/Renderer/Pipeline/RenderPipeline_viewport.h"
#include "Public/Core/Renderer/Renderer.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"
#include "Public/Core/Renderer/Graphics/VertexArray.h"
#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Renderer/RenderCommand.h"

#include "Public/Core/Application/SceneManager.h"
#include "Public/Core/Application/Application.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GEngine {


	static float vert[20] = { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f };
	static uint32_t sqindices[6] = { 0, 1, 2, 2, 3, 0 };

	RenderPipeline_viewport::RenderPipeline_viewport() : RenderPipeline()
	{
		CreateGraphics();
	}

	void RenderPipeline_viewport::RenderStart()
	{
		Renderer::SetDepth(false);
	}

	void RenderPipeline_viewport::RenderEnd()
	{
		Renderer::SetDepth(true);
	}


	void RenderPipeline_viewport::Render()
	{
		std::lock_guard<std::mutex> g(renderMutex);
		const std::vector<FPipeline>& pipelines = Renderer::GetPipelines();
		m_frameBuffer->Bind();
		Renderer::Prepare();
        RenderStart();
		for (const FPipeline& p : pipelines) {
			if (p.p.get() == this) {
				continue;
			}
			
			Ref<Shader> shader = p.p->GetViewPortShader();
			Ref<Texture2D> texture = p.p->GetFrameBuffer()->GetTexture();
			const std::function<void()>& shaderFunc = p.p->GetShaderFunction();

			shader->Bind();
			RenderCommand::BindTexture(texture->GetRendererID(), 0);
			texture->Bind();
			if (shaderFunc != nullptr) shaderFunc();
			m_varray->Bind();
			RenderCommand::DrawIndexed(m_varray);
			
		}
		m_frameBuffer->UnBind();
        Renderer::Prepare();
		m_shader->Bind();
		m_frameBuffer->GetTexture()->Bind();
		m_varray->Bind();
		RenderCommand::DrawIndexed(m_varray);
		RenderEnd();
	}

	void RenderPipeline_viewport::CreateGraphics()
	{
		
		m_vb = Ref<VertexBuffer>(VertexBuffer::Create(20));
		m_vb->SetLayout(Ref<BufferLayout>(new BufferLayout({ {  GEngine::ShaderDataName::Position }, { GEngine::ShaderDataName::TextureCoord} })));
		m_vb->SetVertices(vert, sizeof(vert));
		m_varray = Ref<VertexArray>(VertexArray::Create());
		m_varray->AddVertexBuffer(m_vb);
		m_ib = Ref<GEngine::IndexBuffer>(GEngine::IndexBuffer::Create(sqindices,6));
		m_varray->SetIndexBuffer(m_ib);
	}

	void RenderPipeline_viewport::Unload()
	{
		RenderPipeline::Unload();
		m_ib = nullptr;
		m_vb = nullptr;
		m_varray = nullptr;
	}

	void RenderPipeline_viewport::Reload()
	{
		RenderPipeline::Reload();
		CreateGraphics();
	}

	}
