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

		virtual void SetPosition(const glm::vec2& position) override;
		virtual void SetRotation(const float rot) override;
		virtual void SetLinearVelocity(const glm::vec2& velocity) override;
		virtual void SetLinearDamping(const float linearDamping) override;
		virtual void SetAngularDamping(const float angularDamping) override;
		virtual void SetEnabled(const bool b) override;
		virtual void SetFixedRotation(const bool b) override;
		virtual void SetGravityScale(const float f) override;
		virtual void SetScale(const glm::vec2& scale) override;
		virtual void SetAngularVelocity(const float f) override;

		const float GetAngularVelocity() override;
		const glm::vec2& GetPosition() override;
		const float GetRotation() override;
		const glm::vec2& GetLinearVelocity() override;

		virtual void SetQuad(const glm::vec2& size, const glm::vec2& offset = glm::vec2(0), float mass = 0) override;

	protected:
		b2Body* m_body = nullptr;
		b2Fixture* m_fixture = nullptr;



	};
 


}

#endif