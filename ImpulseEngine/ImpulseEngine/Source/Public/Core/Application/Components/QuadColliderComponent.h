#pragma once

#include "Public/Core/Application/Component.h"

namespace GEngine {

	class Collider2D;
	class PhysicsBody;
	class PhysicsCollision;

	struct FColliderQuad {
		ColliderID id;
		Vector2f position;
		Vector2f scale;
		float mass;
		float rotation;
		std::string tag;
		float bounce;
		bool quad;
	};

	class GE_API QuadColliderComponent : public Component {
	public:
		QuadColliderComponent(Entity* e, bool dyamic = false, bool physics = false, const Vector2f& position = Vector2f(0,0), bool fixedRotation = true, float bounce = 0.f);
		~QuadColliderComponent();

		virtual void OnBegin() override;
		virtual void OnEnd() override;

		virtual void OnUpdate(Timestep ts) override;

		void SetPosition(const float x, const float y);
		void SetScale(const float x, const float y);
		void SetRotation(const float rot);

		const ColliderID CreateQuad(const Vector2f& position, const Vector2f& scale, float mass, float rotation = 0, const std::string& tag = "");
		const ColliderID CreateCircle(const Vector2f& position, const Vector2f& scale, float mass, float rotation = 0, const std::string& tag = "");
		void DestroyQuad(const ColliderID id);

		const Vector2f GetPosition();
		const Vector2f GetScale();

		const Vector2f GetLocalPosition();
		const Vector2f GetLocalScale();
		const float GetLocalRotation();

		void SetGravityScale(const float f);


		const Vector2f GetLinearVelocity();


		void SetBounce(const ColliderID id, const float bounce);
		const float GetBounce(const ColliderID id);
		void SetFixedRotation(const bool rot);
		inline bool GetFixedRotation() const { return m_fixedRotation; };
		

		Ref<PhysicsBody> GetPhysicsBody() const { return m_body; }

		const FColliderQuad& GetQuadCollider(const ColliderID id);

		void IncreaseLinearVelocity(const float x, const float y);
		void IncreaseAngularVelocity(const float v);

		void SetVelocityX(const float x);
		void SetVelocityY(const float y);
		void SetVelocity(const float x, const float y);

		void SetCollisionLayers(const ColliderID id, const uint16_t category, const uint16_t mask, const int16_t index);

		void WakeBody(bool wake = true);

		void SetOnCollideFunction(const ColliderID id, std::function<void(Ref<PhysicsCollision>)> onCollideFunc);
		void SetEndCollideFunction(const ColliderID id, std::function<void(Ref<PhysicsCollision>)> onCollideFunc);

		inline void SetPhysics(bool b) { m_physics = b; }
		inline void SetDynamic(bool b) { m_dynamic = b; }


		inline bool IsDynamic() const { return m_dynamic; }
		inline bool HasPhysics() const { return m_physics; }

		bool b_UpdatePos = true;
		
		std::unordered_map<ColliderID, FColliderQuad>& GetColliders() { return m_quads; }

	private:
		Vector2f m_position;
		Vector2f m_scale;
		Vector2f m_worldPosition;
		Vector2f m_worldScale;
		float m_worldRotation;
		bool m_physics =false;
		float m_rotation;
		bool m_dynamic = false;
		float m_mass = 0;
		bool m_fixedRotation;
		float m_bounce;
		Ref<PhysicsBody> m_body;
		bool m_movedSelf = false;
		uint16_t m_category = 0x02;
		int16_t m_groupIndex = 0x02;
		uint16_t m_mask = 0x02;

		std::unordered_map<ColliderID, FColliderQuad> m_quads;




	};
}