#pragma once

#include "Public/Core/Application/Component.h"

namespace GEngine {

	class Collider2D;
	class ScriptObject;
	struct ScriptVector2;
	class PhysicsBody;
	class PhysicsCollision;

	struct FColliderQuad {
		ColliderID id;
		glm::vec2 position;
		glm::vec2 scale;
		float mass;
		float rotation;
	};

	class QuadColliderComponent : public Component {
	public:
		QuadColliderComponent(bool dyamic = false, bool physics = false, const glm::vec2& position = glm::vec2(0,0));
		~QuadColliderComponent();

		virtual void OnAttached(Ref<Entity> entity) override;
		inline virtual void DeAttached(Ref<Entity> entity) override;

		virtual void OnBegin() override;
		virtual void OnEnd() override;

		virtual void OnUpdate(Timestep ts) override;

		void SetPosition(const float x, const float y);
		void SetScale(const float x, const float y);

		const ColliderID CreateQuad(const glm::vec2& position, const glm::vec2& scale, float mass, float rotation = 0, const std::string& tag = "");
		const ColliderID CreateCircle(const glm::vec2& position, const glm::vec2& scale, float mass, float rotation = 0, const std::string& tag = "");
		void DestroyQuad(const ColliderID id);

		const glm::vec2 GetPosition();
		const glm::vec2 GetScale();

		void SetGravityScale(const float f);


		const glm::vec2 GetLinearVelocity();

		Ref<ScriptVector2> GetLinearVelocityScript();
		Ref<ScriptVector2> GetPositionScript();
		Ref<ScriptVector2> GetScaleScript();

		void SetBounce(const ColliderID id, const float bounce);
		const float GetBounce(const ColliderID id);
		
		void SetDynamic(bool b);

		Ref<PhysicsBody> GetPhysicsBody() const { return m_body; }

		const FColliderQuad& GetQuadCollider(const ColliderID id);

		void IncreaseLinearVelocity(const float x, const float y);
		void IncreaseAngularVelocity(const float v);

		void SetVelocityX(const float x);
		void SetVelocityY(const float y);
		void SetVelocity(const float x, const float y);

		void SetCollisionLayers(const ColliderID id, const uint16_t category, const uint16_t mask, const int16_t index);

		void WakeBody();

		void SetOnCollideFunction(const ColliderID id, std::function<void(Ref<PhysicsCollision>)> onCollideFunc);
		void SetEndCollideFunction(const ColliderID id, std::function<void(Ref<PhysicsCollision>)> onCollideFunc);
		

	private:
		glm::vec2 m_position;
		glm::vec2 m_scale;
		glm::vec2 m_worldPosition;
		glm::vec2 m_worldScale;
		float m_worldRotation;
		bool m_physics =false;
		float m_rotation;
		bool m_dynamic = false;
		float m_mass = 0;
		Ref<PhysicsBody> m_body;
		bool m_movedSelf = false;
		uint16_t m_category = 0x02;
		int16_t m_groupIndex = 0x02;
		uint16_t m_mask = 0x02;

		std::unordered_map<ColliderID, FColliderQuad> m_quads;




	};
}