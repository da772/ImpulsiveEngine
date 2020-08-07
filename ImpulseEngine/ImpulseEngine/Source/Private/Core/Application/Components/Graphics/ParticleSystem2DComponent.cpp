#include "gepch.h"


#include "Public/Core/Application/Components/Graphics/ParticleSystem2DComponent.h"
#include "Public/Core/Renderer/Graphics/ShapeBatch2D.h"
#include "Public/Core/Renderer/Graphics/Quad.h"
#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/RenderCommand.h"
#include "Public/Core/Util/Random.h"
#include "Public/Core/Renderer/Graphics/BatchRenderer.h"

#include "Public/Core/Util/Time.h"

#include "Public/Core/Scripting/ScriptObject.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace GEngine {


	Ref < BatchRenderer > ParticleSystem2DComponent::s_ShapeFactory = nullptr;

	ParticleSystem2DComponent::ParticleSystem2DComponent(const ParticleProps& props) : Component(), m_particleProps(props)
	{
		this->bUpdates = true;
		SetupBatch();
		m_particles = std::vector<Particle>(250);
	}

	ParticleSystem2DComponent::ParticleSystem2DComponent(Ref<ParticleProps> props) : Component()
	{
		m_particleProps = *props.get();
		this->bUpdates = true;
		SetupBatch();
		m_particles = std::vector<Particle>(250);
	}

	ParticleSystem2DComponent::~ParticleSystem2DComponent()
	{
	}

	void ParticleSystem2DComponent::Emit(glm::vec3 pos, int amt)
	{
		for (int i = 0; i < amt; i++) {
			Particle particle;

			particle.Active = true;
			particle.Position = { pos.x, pos.y };
			particle.zOrder = { pos.z };

			particle.Velocity = m_particleProps.Velocity;
			particle.Velocity.x += m_particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
			particle.Velocity.y += m_particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

			particle.LifeRemaining = m_particleProps.LifeTime;
			particle.Scale.x = m_particleProps.SizeBegin + m_particleProps.SizeEnd * (Random::Float() - .5f);
			particle.Scale.y = particle.Scale.x;

			if (m_particles[m_PoolIndex].id != -1) {
				s_ShapeFactory->RemoveShape(m_particles[m_PoolIndex].id);
				m_ids.erase(m_particles[m_PoolIndex].id);
				m_particles[m_PoolIndex].id = -1;
				particleCount--;
			}

			m_particles[m_PoolIndex] = particle;

			if (++m_PoolIndex >= m_particles.size())
				m_PoolIndex = 0;

			particleCount++;

			//auto entity = m_reg.create();
			//m_reg.emplace<Particle>(entity, particle);
		}


	}

	void ParticleSystem2DComponent::EmitScript(Ref<ScriptVector2> pos, int amt)
	{
		glm::vec2 p = pos->GetGlm();
		Emit(glm::vec3(p.x,p.y,1e6), amt);
	}

	int ParticleSystem2DComponent::ParticleCount()
	{
		return particleCount;
	}

	bool ParticleSystem2DComponent::ParticleUpdate(Particle& particle, double dt)
	{

		if (particle.LifeRemaining <= 0.f) {
			particle.Active = false;
			long id = particle.id;
			particle.id = -1;
			m_ids.erase(id);
			s_ShapeFactory->RemoveShape(id);
			particleCount--;
			return false;
		}

		particle.LifeRemaining -= dt;
		particle.Position += particle.Velocity * glm::vec2( dt, dt );
		particle.Rotation += 0.01f * dt;
		float life = particle.LifeRemaining / m_particleProps.LifeTime;
		float scale = glm::lerp(m_particleProps.SizeEnd, particle.Scale.x, life);
		particle.Scale = { scale, scale };
		glm::vec4 color = glm::lerp(m_particleProps.ColorEnd, m_particleProps.ColorBegin, life);

		
		if (particle.id != -1) {
			s_ShapeFactory->EditShape(particle.id, { particle.Position.x, particle.Position.y, particle.zOrder }, particle.Rotation, particle.Scale,
				color, m_particleProps.texture, 1);
		}
		else {
			if (m_particleProps.texture == nullptr && m_particleProps.subTexture == nullptr) {
				particle.id = s_ShapeFactory->AddShape({ particle.Position.x, particle.Position.y,  particle.zOrder },
					particle.Rotation, particle.Scale, color);
			}
			else if (m_particleProps.texture != nullptr) {
				particle.id = s_ShapeFactory->AddShape({ particle.Position.x, particle.Position.y,  particle.zOrder },
					particle.Rotation, particle.Scale, color, m_particleProps.texture);
			}
			else if (m_particleProps.subTexture != nullptr) {
				particle.id = s_ShapeFactory->AddShape({ particle.Position.x, particle.Position.y, particle.zOrder },
					particle.Rotation, particle.Scale, color, m_particleProps.subTexture, 1);
			}

			m_ids.insert(particle.id);
		}
		
		
		return true;

	}

	void ParticleSystem2DComponent::OnBegin()
	{
		SetupBatch();
		m_Began = true;

	}

	void ParticleSystem2DComponent::OnEnd()
	{
		if (ParticleSystem2DComponent::s_ShapeFactory != nullptr) {
			for (long id : m_ids) {
				s_ShapeFactory->RemoveShape(id);
			}
		}
	}

	void ParticleSystem2DComponent::OnUpdate(Timestep timestep)
	{
		long long time = Time::GetEpochTimeMS();
		long long tPassed = time - m_LastUpdate;
		double dt = tPassed / 1000.0;
		if (particleCount > 0) {
			for (Particle& p : m_particles) {
				if (p.Active)
					ParticleUpdate(p, dt);
			}
			m_LastUpdate = time;
		}
	}


	void ParticleSystem2DComponent::SetupBatch()
	{
		if (s_ShapeFactory == nullptr) {
			std::string path = std::string("Content/shaders/TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "Batch.glsl";
			Ref<Shader> m_Shader = Ref<Shader>(Shader::Create(path));
			ParticleSystem2DComponent::s_ShapeFactory = Ref<BatchRenderer>(new BatchRenderer("2d", Ref<Quad>(new Quad()),
				50000, m_Shader));
			s_ShapeFactory->SetSort(false);
		}
	}



	ParticleProps::ParticleProps()
	{
	}

	Ref<ScriptVector2> ParticleProps::GetVelocity()
	{
		return make_shared<ScriptVector2>(Velocity);
	}

	Ref<ScriptVector2> ParticleProps::GetVelocityVariation()
	{
		return make_shared<ScriptVector2>(VelocityVariation);
	}

	Ref<ScriptVector4> ParticleProps::GetColorBegin()
	{
		return make_shared<ScriptVector4>(ColorBegin);
	}

	Ref<ScriptVector4> ParticleProps::GetColorEnd()
	{
		return make_shared<ScriptVector4>(ColorEnd);
	}

	void ParticleProps::SetVelocity(Ref<ScriptVector2> vel)
	{
		Velocity = vel->GetGlm();
	}

	void ParticleProps::SetVelocityVariation(Ref<ScriptVector2> vel)
	{
		VelocityVariation = vel->GetGlm();
	}

	void ParticleProps::SetColorBegin(Ref<ScriptVector4> col)
	{
		ColorBegin = col->GetGlm();
	}

	void ParticleProps::SetColorEnd(Ref<ScriptVector4> col)
	{
		ColorEnd = col->GetGlm();
	}

}
