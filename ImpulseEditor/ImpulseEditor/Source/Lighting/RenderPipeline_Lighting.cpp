#include "Lighting/RenderPipeline_Lighting.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace GEngine;

static float vert[20] = { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f };
static uint32_t sqindices[6] = { 0, 1, 2, 2, 3, 0 };

RenderPipeline_Lighting::RenderPipeline_Lighting(const float renderScale) : RenderPipeline("RenderPipeline_Lighting", renderScale)
{
	
	// Use Gaussian Blur Shader!!
	m_shaderColor = Shader::Create("Content/shaders/ColorShader.glsl");
	m_shaderMask = Shader::Create("Content/shaders/MaskShader.glsl");
	//m_shader = Shader::Create("Content/shaders/LightGlow.glsl");
	//m_shaderBlur = Shader::Create("Content/shaders/GaussianBlurShader.glsl");
//	m_shader = Shader::Create("Content/shaders/GaussianBlurShader.glsl");

	// Set Shader function to add blur parameters
	m_shaderStartFunc = []() {
		//RenderCommand::BlendFunc(0x0001, 0x0001);
	};
	m_shaderEndFunc = []() {
		//RenderCommand::BlendFunc(BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA);
	};
	CreateGraphics();
	m_frameBuffer = FrameBuffer::Create(0, 0, TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Linear | TEXTUREFLAGS_Min_Nearest, "RenderPipeline_Lighting", renderScale);
	m_frameBuffer_shadow = FrameBuffer::Create(0, 0, TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Linear | TEXTUREFLAGS_Min_Nearest, "RenderPipeline_Lighting_Shadow", renderScale);
	m_frameBuffer_shadow->UpdateSize(Application::GetWidth(), Application::GetHeight());
	m_frameBuffer_lights = FrameBuffer::Create(0, 0, TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest,
		"RenderPipeline_Lighting_Lights", renderScale);
	m_frameBuffer_lights->UpdateSize(Application::GetWidth(), Application::GetHeight());
	m_frameBuffer->UpdateSize(Application::GetWidth(), Application::GetHeight());
}

RenderPipeline_Lighting::~RenderPipeline_Lighting()
{
	
}

void RenderPipeline_Lighting::Render()
{
	std::lock_guard<std::mutex> g(renderMutex);
	// Render Lights
	m_frameBuffer_lights->Bind();
	Renderer::Prepare();
	RenderStart();
	//RenderCommand::BlendFunc(BLEND_SRC_ALPHA, BLEND_ONE_MINUS_DST_ALPHA);
	for (int i = 0; i < renderables.size(); i++) {
		renderables[i]->Render();
	}
	m_frameBuffer_lights->UnBind();
	//RenderCommand::BlendFunc(BLEND_SRC_ALPHA, BLEND_ONE_MINUS_SRC_ALPHA);
	// Render Shadows
	m_frameBuffer_shadow->Bind();
	Renderer::Prepare();
	m_shaderColor->Bind();
	glm::mat4 transform = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0))
		* glm::scale(glm::mat4(1.0), { 5000.f, 5000.f, 1.f });
	m_shaderColor->UploadUniformMat4("u_Transform", transform);
	if (Application::GetApp()->GetTargetCamera())
		m_shaderColor->UploadUniformMat4("u_ViewProjection", Application::GetApp()->GetTargetCamera()->GetViewProjectionMatrix());
	m_shaderColor->UploadUniformFloat4("u_Color", { 0,0,0,.55f });
	m_varray->Bind();
	RenderCommand::DrawIndexed(m_varray);
	m_frameBuffer_shadow->UnBind();
	// Combine Lights+Shadows
	m_frameBuffer->Bind();
	RenderCommand::BlendFuncSeparate(0x0302, 0x0303, 0x01, 0x01);
	Renderer::Prepare();
	m_shaderMask->Bind();
	transform = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)) * glm::scale(glm::mat4(1.0), { 1.f, 1.f, 1.f });
	m_shaderMask->UploadUniformMat4("u_Transform", transform);
	m_shaderMask->UploadUniformInt("u_Texture", 0);
	m_shaderMask->UploadUniformInt("u_Texture_Mask", 1);
	if (m_frameBuffer_shadow->GetTexture())
		m_frameBuffer_shadow->GetTexture()->Bind(0);
	if (m_frameBuffer_lights->GetTexture())
		m_frameBuffer_lights->GetTexture()->Bind(1);
	m_varray->Bind();
	RenderCommand::DrawIndexed(m_varray);
	m_frameBuffer->UnBind();
	/*
	m_frameBuffer_shadow->Bind();
	Renderer::Prepare();
	//RenderCommand::SetClearColor({0, 0, 0, .2f});
	m_shaderBlur->Bind();
	m_shaderBlur->UploadUniformFloat2("u_BlurDirection", { 1,0 });
	m_frameBuffer_lights->GetTexture()->Bind();
	m_varray->Bind();
	RenderCommand::DrawIndexed(m_varray);
	m_frameBuffer_shadow->UnBind();

	m_frameBuffer->Bind();
	Renderer::Prepare();
	//RenderCommand::SetClearColor({ 0, 0, 0, .2f });
	m_shaderBlur->UploadUniformFloat2("u_BlurDirection", { 0,1 });
	m_frameBuffer_shadow->GetTexture()->Bind();
	m_varray->Bind();
	RenderCommand::DrawIndexed(m_varray);
	m_frameBuffer->UnBind();
	*/
	RenderEnd();

	
}

void RenderPipeline_Lighting::RenderStart()
{
	Renderer::SetDepth(true);
}

void RenderPipeline_Lighting::RenderEnd()
{
	Renderer::SetDepth(true);
}

void RenderPipeline_Lighting::Unload()
{
	RenderPipeline::Unload();
	m_ib = nullptr;
	m_vb = nullptr;
	m_varray = nullptr;
	m_frameBuffer_shadow->Unload();
	m_frameBuffer_lights->Unload();
}

void RenderPipeline_Lighting::Reload()
{
	RenderPipeline::Reload();
	m_frameBuffer_shadow->Reload();
	m_frameBuffer_lights->Reload();
	CreateGraphics();
}

void RenderPipeline_Lighting::CreateGraphics()
{
	m_vb = Ref<VertexBuffer>(VertexBuffer::Create(20));
	m_vb->SetLayout(Ref<BufferLayout>(new BufferLayout({ {  GEngine::ShaderDataName::Position }, { GEngine::ShaderDataName::TextureCoord} })));
	m_vb->SetVertices(vert, sizeof(vert));
	m_varray = Ref<VertexArray>(VertexArray::Create());
	m_varray->AddVertexBuffer(m_vb);
	m_ib = Ref<GEngine::IndexBuffer>(GEngine::IndexBuffer::Create(sqindices, 6));
	m_varray->SetIndexBuffer(m_ib);
}
