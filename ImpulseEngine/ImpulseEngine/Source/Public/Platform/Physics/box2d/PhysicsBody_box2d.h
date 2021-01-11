#pragma once

#ifdef GE_PHYSICS_BOX2D

#include "Public/Core/Physics/PhysicsBody.h"
#include "Public/Core/Physics/PhysicsStructures.h"

class b2Body;
class b2Fixture;

namespace GEngine {

	struct PhysicsInfo;

	class PhysicsBody_box2d : public PhysicsBody {
	public:
		PhysicsBody_box2d(void* nativeBody);
		virtual ~PhysicsBody_box2d();

		virtual void SetPosition(const Vector2f& position) override;
		virtual void SetRotation(const float rot) override;
		virtual void SetLinearVelocity(const Vector2f& velocity) override;
		virtual void SetLinearDamping(const float linearDamping) override;
		virtual void SetAngularDamping(const float angularDamping) override;
		virtual void SetEnabled(const bool b) override;
		virtual void SetFixedRotation(const bool b) override;
		virtual void SetGravityScale(const float f) override;
		virtual void SetScale(const Vector2f& scale) override;
		virtual void SetAngularVelocity(const float f) override;
		virtual void SetAwake(bool b) override;


		virtual void SetMask(const ColliderID id, uint16_t bits) override;
		virtual void SetCategory(const ColliderID id, const uint16_t bits) override;
		virtual void SetGroupIndex(const ColliderID id, const int16_t index) override;


		virtual void SetSensor(const ColliderID id, const bool b) override;

		virtual void SetBounce(const ColliderID id, const float f) override;

		const float GetAngularVelocity() override;
		const Vector2f& GetPosition() override;
		const float GetRotation() override;
		const Vector2f& GetLinearVelocity() override;
		const float virtual GetBounce(const ColliderID id) override;

		virtual const ColliderID CreateQuad(const Vector2f& size, const Vector2f& offset = Vector2f(0.f), float mass = 0, float rotation = 0, const std::string& tag = "") override;
		virtual const ColliderID CreateCircle(const Vector2f& size, const Vector2f& offset = Vector2f(0.f), float mass = 0, float rotation = 0, const std::string& tag = "") override;
		virtual void DestroyQuad(const ColliderID) override;

		virtual void SetOnCollideStartFunction(const ColliderID id, std::function<void(Ref<PhysicsCollision>)> f) override;
		virtual void SetOnCollideEndFunction(const ColliderID id, std::function<void(Ref<PhysicsCollision>)> f) override;


	protected:
		b2Body* m_body = nullptr;

		std::unordered_map<ColliderID, b2Fixture*> m_fixtures;
		std::unordered_map<ColliderID, PhysicsParent> m_fixtureData;



	};
 


}

#endif