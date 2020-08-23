#pragma once
#include "Public/Core/Application/Component.h"
#include <entt.hpp>


namespace GEngine {

	class Texture2D;
	class SubTexture2D;
	class BatchRenderer;
	struct ScriptVector3;
	struct ScriptVector2;
	struct ScriptVector4;


	struct ParticleProps {
	public:

		ParticleProps();

		
		inline Ref<Texture2D> GetTexture() { return texture;}
		inline Ref<SubTexture2D> GetSubTexture() { return subTexture; }
		inline void SetTexture(Ref<Texture2D> t) { texture = t; }
		inline void SetSubTexture(Ref<SubTexture2D> t) { subTexture = t; }


		glm::vec2 Velocity = glm::vec2(0,0), VelocityVariation = glm::vec2(.5f,.5f);
		glm::vec4 ColorBegin = glm::vec4(1,1,1,1), ColorEnd = glm::vec4(0,0,0,1);
		float SizeBegin = 1.f, SizeEnd = .5f, SizeVariation = .25f;
		float LifeTime = 1.f;
		
		Ref<Texture2D> texture = nullptr;
		Ref<SubTexture2D> subTexture = nullptr;

		Ref<ScriptVector2> GetVelocity();
		Ref<ScriptVector2> GetVelocityVariation();
		Ref<ScriptVector4> GetColorBegin();
		Ref<ScriptVector4> GetColorEnd();

		void SetVelocity(const Ref<ScriptVector2>& vel);
		void SetVelocityVariation(const Ref<ScriptVector2>& vel);
		void SetColorBegin(const Ref<ScriptVector4>& col);
		void SetColorEnd(const Ref<ScriptVector4>& col);

		inline float GetSizeBegin() { return SizeBegin; }
		inline float GetSizeEnd() { return SizeEnd; }
		inline float GetSizeVariation() { return SizeVariation; }
		inline float GetLifetime() { return LifeTime; }

		inline void SetSizeBegin(float v) { SizeBegin = v; }
		inline void SetSizeEnd(float v) { SizeEnd = v; }
		inline void SetSizeVariation(float v) { SizeVariation = v; }
		inline void SetLifeTime(float v) { LifeTime = v; }

	};

	class ParticleSystem2DComponent : public Component {

	public:
		ParticleSystem2DComponent(const ParticleProps& props);
		ParticleSystem2DComponent(const Ref<ParticleProps>& props);
		~ParticleSystem2DComponent();


		void Emit(const glm::vec3& postition, const int amt = 1);
		void EmitScript(const Ref<ScriptVector2>& pos, const int amt);

		virtual void UnloadGraphics() override;
		virtual void ReloadGraphics() override;

		int ParticleCount();
	protected:
		void OnBegin() override;


		void OnEnd() override;


		void OnUpdate(Timestep timestep) override;


	private:
		ParticleProps m_particleProps;

		bool bUnloaded = false;
		int m_PoolIndex = 0;

		struct Particle {
			glm::vec2 Position;
			float zOrder;
			glm::vec2 Velocity;
			float Rotation = 0.f;
			glm::vec2 Scale;
			float LifeRemaining;
			long id = -1;
			bool Active = false;
			
		};

		bool m_Began = false;
		int particleCount = 0;

		std::vector<Particle> m_particles;

		int m_particlePos = 0;

		long long m_LastUpdate = 0;
		static Ref < BatchRenderer > s_ShapeFactory ;
		std::unordered_set<long> m_ids;
		std::mutex m_destroyMutex;

		entt::registry m_reg;

		void SetupBatch();
	public:
		bool ParticleUpdate(Particle& particle, double dt);

	};

}
