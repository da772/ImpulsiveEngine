#include "gepch.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Renderer2D.h"
#include "Core/Renderer/RenderCommand.h"

#include "Public/Core/Application/Application.h"

#include "Public/Core/Renderer/Graphics/VertexArray.h"
#include "Public/Core/Renderer/Graphics/Buffer.h"
#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Renderer/Camera.h"
#include "Public/Core/Renderer/Pipeline/RenderPipeline.h"

#include "Public/Core/Renderer/Pipeline/RenderPipeline_2d.h"
#include "Public/Core/Renderer/Pipeline/RenderPipeline_ui.h"
#include "Public/Core/Renderer/Pipeline/RenderPipeline_viewport.h"

#include "Public/Core/Renderer/Graphics/Renderable.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Public/Core/Application/Scene.h"
#include "Public/Core/Application/SceneManager.h"





namespace GEngine {


	class DebugRenderable : public Renderable {

	public:
		DebugRenderable(std::function<void()> f = nullptr) : m_function(f) {};

		void Render() override
		{
			if (m_function)
				m_function();
		}

	private:
		std::function<void()> m_function;

	};



	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;
	float Renderer::renderScale = 1.f;
	Scope<Renderer::ShapeData> Renderer::s_ShapeData;
	std::vector<FPipeline> Renderer::queue;
	std::unordered_map<std::string, FPipeline> Renderer::queueId;

	void Renderer::BeginScene(Camera* camera)
	{
		if (camera == nullptr) {
			if (SceneManager::GetCurrentScene() != nullptr) {
				camera = SceneManager::GetCurrentScene()->GetCamera();
			}
			if (camera == nullptr)
				return;
		}
		Renderer::s_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
		Renderer::s_SceneData->ProjectionMatrix = camera->GetProjectionMatrix();
		Renderer::s_SceneData->ViewMatrix = camera->GetViewMatrix();
		Renderer::s_SceneData->position = camera->GetPosition();
		Renderer2D::BeginScene(camera);
		Renderer::GetPipeline("Debug2D")->SetCamera(camera);
		Renderer::GetPipeline("2d")->SetCamera(camera);
	}

	void Renderer::Init()
	{
		RenderCommand::Create();
		RenderCommand::Init();
		Renderer::OnInit();
		Renderer2D::Init();

	}

	void Renderer::OnInit()
	{
		Renderer::AddPipeline("2d", std::shared_ptr<RenderPipeline>(new RenderPipeline_2d(Renderer::GetRenderScale())), 50);
		Renderer::AddPipeline("Debug2D", std::shared_ptr<RenderPipeline>(new RenderPipeline_2d(Renderer::GetRenderScale(), "Renderpipeline_Debug2D")), 498);
		Renderer::AddPipeline("ui", std::shared_ptr<RenderPipeline>(new RenderPipeline_ui()), 1000);
		Renderer::AddPipeline("viewport", std::shared_ptr<RenderPipeline>(new RenderPipeline_viewport()), 1500);
        const std::vector<GEngine::FPipeline> pipelines = GEngine::Renderer::GetPipelines();
        for (const GEngine::FPipeline& p : pipelines) {
            p.p->SetSize(GEngine::Application::GetWidth(), GEngine::Application::GetHeight());
        }
		s_ShapeData.reset(new ShapeData());

		s_ShapeData->shape_shader = Shader::Create("EngineContent/shaders/TextureShader.glsl");
		s_ShapeData->empty_texture.reset();
		s_ShapeData->empty_texture = std::shared_ptr<GEngine::Texture2D>(GEngine::Texture2D::Create("batchBlank", 1, 1));
		uint32_t whiteTextureData = 0xffffffff;
		s_ShapeData->empty_texture->SetData(&whiteTextureData, sizeof(uint32_t));


		CreateBuffers();
	}

	void Renderer::CreateBuffers()
	{
		s_ShapeData->box_vArray.reset(VertexArray::Create());
		s_ShapeData->box_vBuffer.reset(VertexBuffer::Create(s_ShapeData->Box, sizeof(s_ShapeData->Box)));
		s_ShapeData->box_vBuffer->SetLayout(std::shared_ptr< BufferLayout>(new GEngine::BufferLayout({ { GEngine::ShaderDataName::Position} })));
		s_ShapeData->box_vArray->AddVertexBuffer(s_ShapeData->box_vBuffer);

		s_ShapeData->line_vArray.reset(VertexArray::Create());
		s_ShapeData->line_vBuffer.reset(VertexBuffer::Create(s_ShapeData->line, sizeof(s_ShapeData->line)));
		s_ShapeData->line_vBuffer->SetLayout(std::shared_ptr<BufferLayout>(new BufferLayout({ {  GEngine::ShaderDataName::Position } })));
		s_ShapeData->line_vArray->AddVertexBuffer(s_ShapeData->line_vBuffer);
	}

	void Renderer::DestroyBuffers()
	{
		s_ShapeData->box_vArray = nullptr;
		s_ShapeData->box_vBuffer = nullptr;
		s_ShapeData->line_vArray = nullptr;
		s_ShapeData->line_vBuffer = nullptr;
	}

	void Renderer::Shutdown()
	{
		s_ShapeData = nullptr;
		Renderer::queue.clear();
		Renderer::queueId.clear();
		RenderCommand::Destroy();
		Renderer2D::Shutdown();
	}

	void Renderer::Render()
	{
		Prepare();
		for (u8 i = 0; i < Renderer::queue.size(); i++) {
			Renderer::queue[i].p->Render();
		}
	}

	void Renderer::Prepare()
	{
		RenderCommand::SetClearColor({ 0, 0, 0, 0});
		RenderCommand::Clear();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::AddPipeline(const char* id, Ref<RenderPipeline> p, int priority)
	{
        std::unordered_map<std::string, FPipeline> ::iterator it = Renderer::queueId.find(id);
		//GE_CORE_ASSERT(p != nullptr && it == Renderer::queue.end(), "INVLAID PIPELINE");
		if (it == Renderer::queueId.end()) {
			FPipeline _p(p, priority);
			Renderer::queue.push_back(_p);
			std::sort(Renderer::queue.begin(), Renderer::queue.end(), [](const FPipeline& p1, const FPipeline& p2) {return p1.i < p2.i; });
			Renderer::queueId[std::string(id)] = _p;
			return;
		}
		return;
	}

	void Renderer::SortPipelines()
	{
		std::sort(Renderer::queue.begin(), Renderer::queue.end(), [](const FPipeline& p1, const FPipeline& p2) {return p1.i < p2.i; });
	}

	GEngine::Ref<GEngine::RenderPipeline> Renderer::GetPipeline(const char* id)
	{
		if (Renderer::queueId.size() > 0 && Renderer::queueId.count(id)) {
			return Renderer::queueId[std::string(id)].p;
		}
		//GE_CORE_ASSERT(false, "PIPELINE NOT FOUND");
		return nullptr;
	}

	const std::vector<GEngine::FPipeline>& Renderer::GetPipelines()
	{
		return queue;
	}

	void Renderer::Unload()
	{
		for (const FPipeline& p : queue) {
			p.p->Unload();
		}
		DestroyBuffers();
	}

	void Renderer::Reload()
	{
		CreateBuffers();
		for (const FPipeline& p : queue) {
			p.p->Reload();
		}
        const std::vector<GEngine::FPipeline> pipelines = GEngine::Renderer::GetPipelines();
        for (const GEngine::FPipeline& p : pipelines) {
            p.p->GetFrameBuffer()->UpdateSize(GEngine::Application::GetWidth(), GEngine::Application::GetHeight());
        }
	}



	void Renderer::EndScene()
	{
		Renderer::queueId["Debug2D"].p->Clear();
		Renderer2D::EndScene();
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, bool debug)
	{
		Ref<Shader> _shader = shader ? shader : s_ShapeData->shape_shader;

		if (debug) {
		queueId["Debug2D"].p->Add(std::make_shared<DebugRenderable>([_shader, vertexArray, transform]() {
			_shader->Bind();
			_shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
			_shader->UploadUniformMat4("u_Transform", transform);
			vertexArray->Bind();
			RenderCommand::DrawIndexed(vertexArray);
			}));
		}
		else {
			_shader->Bind();
			_shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
			_shader->UploadUniformMat4("u_Transform", transform);
			vertexArray->Bind();
			RenderCommand::DrawIndexed(vertexArray);
		}
	}

	void Renderer::SubmitArrays(Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform, bool debug) {
		if (!shader)
			shader = s_ShapeData->shape_shader;
	
		if (debug) {

			queueId["Debug2D"].p->Add(std::make_shared<DebugRenderable>([shader, vertexArray, transform]() {
				shader->Bind();
				shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
				shader->UploadUniformMat4("u_Transform", transform);
				vertexArray->Bind();
				RenderCommand::DrawArrays(vertexArray);
				}));

		}  else {
			shader->Bind();
			shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
			shader->UploadUniformMat4("u_Transform", transform);
			vertexArray->Bind();
			RenderCommand::DrawArrays(vertexArray);
		}

	}

	void Renderer::SubmitArraysLines(Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform /*= glm::mat4(1.f)*/)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->UploadUniformMat4("u_Transform", transform);
		vertexArray->Bind();
		RenderCommand::DrawArraysLines(vertexArray);
	}

	void Renderer::DrawCube(const Vector3f& position, const Vector3f& scale, const Vector4f& color)
	{
		Renderer::s_ShapeData->shape_shader->Bind();
		Renderer::s_ShapeData->shape_shader->UploadUniformInt("u_Texture", 0);

		glm::mat4 transform = glm::translate(glm::mat4(1.f), glm::vec3(position.x, position.y, position.z)) * glm::scale(glm::mat4(1), glm::vec3(scale.x, scale.y,scale.z));

		Renderer::s_ShapeData->shape_shader->UploadUniformFloat4("u_Color", color);
		Renderer::s_ShapeData->shape_shader->UploadUniformFloat2("u_UV", { 1.f,1.f });
		Renderer::s_ShapeData->shape_shader->UploadUniformFloat("u_Threshold", 0.f);

		Renderer::s_ShapeData->empty_texture->Bind();

		GEngine::Renderer::SubmitArrays(s_ShapeData->shape_shader, s_ShapeData->box_vArray, transform);
	}



	void Renderer::DrawLine(const Vector3f& startPos, const Vector3f& endPos, const Vector4f& color)
	{

		Renderer::s_ShapeData->shape_shader->Bind();
		Renderer::s_ShapeData->shape_shader->UploadUniformInt("u_Texture", 0);

		Renderer::s_ShapeData->shape_shader->UploadUniformFloat4("u_Color", color);
		Renderer::s_ShapeData->shape_shader->UploadUniformFloat2("u_UV", { 1.f,1.f });
		Renderer::s_ShapeData->shape_shader->UploadUniformFloat("u_Threshold", 0.f);

		Renderer::s_ShapeData->empty_texture->Bind();

		float f[] = {
			startPos.x,startPos.y,startPos.z,
			endPos.x,endPos.y,endPos.z
		};

		s_ShapeData->line_vBuffer->SetVertices(f, sizeof(f));
		glm::mat4 transform = glm::translate(glm::mat4(1.f), { 0,0,0 });

		queueId["Debug2D"].p->Add(std::make_shared<DebugRenderable>([transform]() {

			GEngine::Renderer::SubmitArraysLines(s_ShapeData->shape_shader, s_ShapeData->line_vArray, transform);

			}));

	}

	void Renderer::DrawLines(const std::vector<float>& lines, const Vector4f& color)
	{

		queueId["Debug2D"].p->Add(std::make_shared<DebugRenderable>([lines, color]() {

			Renderer::s_ShapeData->shape_shader->Bind();
			Renderer::s_ShapeData->shape_shader->UploadUniformInt("u_Texture", 0);

			Renderer::s_ShapeData->shape_shader->UploadUniformFloat4("u_Color", color);
			Renderer::s_ShapeData->shape_shader->UploadUniformFloat2("u_UV", { 1.f,1.f });
			Renderer::s_ShapeData->shape_shader->UploadUniformFloat("u_Threshold", 0.f);

			Renderer::s_ShapeData->empty_texture->Bind();

			s_ShapeData->line_vBuffer->SetVertices((float*)lines.data(), lines.size() * sizeof(float));
			s_ShapeData->line_vArray->SetVertexCount(lines.size() / 3.f);

			glm::mat4 transform = glm::translate(glm::mat4(1.f), { 0,0,0 });

			GEngine::Renderer::SubmitArraysLines(s_ShapeData->shape_shader, s_ShapeData->line_vArray, transform);

			}));

	}

	void Renderer::DrawDebugLines(const std::vector<float>& lines, const Vector4f& color)
	{
		queueId["Debug2D"].p->Add(std::make_shared<DebugRenderable>([lines, color]() {

			Renderer::s_ShapeData->shape_shader->Bind();
			Renderer::s_ShapeData->shape_shader->UploadUniformInt("u_Texture", 0);

			Renderer::s_ShapeData->shape_shader->UploadUniformFloat4("u_Color", color);
			Renderer::s_ShapeData->shape_shader->UploadUniformFloat2("u_UV", { 1.f,1.f });
			Renderer::s_ShapeData->shape_shader->UploadUniformFloat("u_Threshold", 0.f);

			Renderer::s_ShapeData->empty_texture->Bind();

			s_ShapeData->line_vBuffer->SetVertices((float*)lines.data(), lines.size() * sizeof(float));
			s_ShapeData->line_vArray->SetVertexCount(lines.size() / 3.f);

			glm::mat4 transform = glm::translate(glm::mat4(1.f), { 0,0,0 });

			GEngine::Renderer::SubmitArraysLines(s_ShapeData->shape_shader, s_ShapeData->line_vArray, transform);

			}));
	}

	void Renderer::DrawCircle(const Vector3f& position, float rotation, const Vector3f& scale, const Vector4f& color)
	{
		
	}

	void Renderer::DrawText3D(const char* txt, float scale, const Vector3f& position, const Vector4f& color, bool center) {

		const glm::mat4 model = {
			scale, 0.0f, 0.0f, 0.0f,
			0.0f, -scale, 0.0f, 0.0f,
			0.0f, 0.0f, scale, 0.0f,
			position.x, position.y, position.z, 1.0f,
		};

		RenderCommand::DrawText3D(txt, color, scale, model, Renderer::s_SceneData->ViewMatrix, Renderer::s_SceneData->ProjectionMatrix, center);
	}

	void Renderer::DrawText3D_Billboard(const char* txt, float scale, const Vector3f& position, const Vector4f& color, bool center)
	{
		const glm::mat4 model = {
		scale, 0.0f, 0.0f, 0.0f,
		0.0f, -scale, 0.0f, 0.0f,
		0.0f, 0.0f, scale, 0.0f,
		position.x, position.y, position.z, 1.0f,
		};

		RenderCommand::DrawText3D(txt, color, scale, model, Renderer::s_SceneData->ViewMatrix, Renderer::s_SceneData->ProjectionMatrix, center);
	}

	void Renderer::SetRenderScale(const float f)
	{
		renderScale = f;
		for (const FPipeline p : queue) {
			p.p->SetSize(p.p->GetWidth(), p.p->GetHeight());
		}
	}

	const float Renderer::GetRenderScale()
	{
		return renderScale; 
	}

	int Renderer::GetMaxTextureSlots()
	{
		return RenderCommand::GetMaxTextureSlots();
	}

	void Renderer::SetDepth(bool b)
	{
		RenderCommand::SetDepth(b);
	}

}
