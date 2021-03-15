#include "ParticleSystem2D.h"




ParticleSystem2D::ParticleSystem2D(const uint32_t& count, const Vector3f& startPos, const Vector2f& vel, const Vector2f& startScale, const Vector2f& endScale, Ref<Texture2D> texture, Ref<Shader> shader, const Vector4f& startColor, const Vector4f& endColor, const float& lifespan, bool loop /*= true*/)
	: m_particleCount(count), m_startPosition(startPos), m_velocity(vel), m_startColor(startColor), m_endColor(endColor), m_loop(loop), m_lifeSpan(lifespan), m_texture(texture), m_startScale(startScale),
	m_endScale(endScale), m_shader(shader)
{
	
}

ParticleSystem2D::~ParticleSystem2D()
{
	m_renderable = nullptr;
	m_pipeline = nullptr;
}

void ParticleSystem2D::UnloadGraphics()
{
	m_pipeline->Remove(m_renderable);
	m_lastRenderableTime = dynamic_pointer_cast<ParticleSystem2D_Renderable>(m_renderable)->GetLastRenderTime();
	m_renderable = nullptr;
}

void ParticleSystem2D::ReloadGraphics()
{
	m_renderable = make_shared<ParticleSystem2D_Renderable>(this, m_lastRenderableTime);
	m_pipeline->Add(m_renderable);
}

void ParticleSystem2D::OnBegin()
{
	if (m_texture == nullptr) {
		m_texture = Texture2D::Create("batchBlank");
	}
	m_pipeline = Renderer::GetPipeline("2d");
	m_renderable = make_shared<ParticleSystem2D_Renderable>(this);
	m_pipeline->Add(m_renderable);
}

void ParticleSystem2D::OnEnd()
{
	m_pipeline->Remove(m_renderable);
	m_renderable = nullptr;
}

ParticleSystem2D_Renderable::ParticleSystem2D_Renderable(ParticleSystem2D* system, float time) : m_system(system), m_totalTime(time)
{
	m_Priority = m_system->GetStartPosition().z;
	m_Time = Time::GetEpochTimeNS();
	Quad q = Quad();
	
	m_vertexBuffer = shared_ptr<VertexBuffer>( VertexBuffer::Create(q.GetVertices(m_system->GetStartPosition(), 0, { m_system->GetStartScale(), 1 }, m_system->GetStartColor(), m_system->GetTexture()->GetRendererID(), { 1,1 }).data(), q.GetVerticesSize()*sizeof(uint32_t)));
	m_vertexBuffer->SetLayout(q.GetBufferLayout());
	std::vector<u32> indices = q.GetIndices(0);
	m_indexBuffer = shared_ptr<IndexBuffer>( IndexBuffer::Create(indices.data(), q.GetIndicesSize()));
	m_vertexArray = Ref<VertexArray>(VertexArray::Create());
	m_vertexArray->AddVertexBuffer(m_vertexBuffer);
	m_vertexArray->SetIndexBuffer(m_indexBuffer);
	m_lastTime = Time::GetEpochTimeMS();
}

void ParticleSystem2D_Renderable::Render()
{
	double timeMS = Time::GetEpochTimeMS();
	double timeStep = (timeMS - m_lastTime) / 1e3f;
	m_totalTime += timeStep;
	if (m_system->DoesLoop() && m_totalTime > m_system->GetLifeSpan()) {
		m_totalTime = 0;
	}
	m_system->GetShader()->Bind();
	m_system->GetShader()->UploadUniformMat4("u_ViewProjection", SceneManager::GetCurrentViewProjectionMatrix());
	m_system->GetShader()->UploadUniformFloat("u_Time", m_totalTime);
	m_system->GetShader()->UploadUniformFloat("u_Lifespan", m_system->GetLifeSpan());
	m_system->GetShader()->UploadUniformFloat2("u_Vel", m_system->GetVelocity());
	m_system->GetShader()->UploadUniformFloat4("u_EndColor", m_system->GetEndColor());
	m_system->GetTexture()->Bind();
	m_vertexArray->Bind();
	RenderCommand::DrawIndexedInstanced(m_vertexArray, m_indexBuffer->GetIndexCount(), m_system->GetParticleCount());

	m_lastTime = timeMS;
}

ParticleSystem2D_Renderable::~ParticleSystem2D_Renderable()
{
	m_indexBuffer = nullptr;
	m_vertexArray = nullptr;
	m_vertexBuffer = nullptr;
}
