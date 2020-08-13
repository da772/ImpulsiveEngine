#include "gepch.h"
#ifdef GE_PHYSICS_BOX2D

#include "Public/Core/Platform/Physics/box2d/PhysicsBody_box2d.h"
#include "Public/Core/Physics/PhysicsStructures.h"
#include "Public/Core/Physics/Physics.h"
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>



namespace GEngine {

	PhysicsBody_box2d::PhysicsBody_box2d(void* nativeBody)
	{
		m_body = (b2Body*)nativeBody;
		const b2Vec2& pos = m_body->GetPosition();
		m_position = { pos.x* GE_PHYSICS_SCALAR, pos.y* GE_PHYSICS_SCALAR };
		m_rotation = m_body->GetAngle();
	}

	PhysicsBody_box2d::~PhysicsBody_box2d()
	{
		GE_CORE_DEBUG("DESTROYING PHYSICS BOX2D");
		b2World* world = (b2World*)Physics::GetWorld();
		if (world != nullptr) {
			world->DestroyBody(m_body);
			m_body = nullptr;
		}
	}

	void PhysicsBody_box2d::SetPosition(const glm::vec2& position)
	{
		PhysicsBody::SetPosition(position* GE_PHYSICS_SCALAR);
		m_body->SetTransform(b2Vec2(position.x* GE_PHYSICS_SCALAR, position.y* GE_PHYSICS_SCALAR), m_body->GetAngle());
	}

	void PhysicsBody_box2d::SetRotation(const float rot)
	{
		PhysicsBody::SetRotation(rot);
		m_body->SetTransform(m_body->GetPosition(), rot);
	}

	void PhysicsBody_box2d::SetLinearVelocity(const glm::vec2& velocity)
	{
		PhysicsBody::SetLinearVelocity(velocity* GE_PHYSICS_SCALAR);
		m_body->SetLinearVelocity(b2Vec2(velocity.x*GE_PHYSICS_SCALAR, velocity.y* GE_PHYSICS_SCALAR));
	}

	void PhysicsBody_box2d::SetLinearDamping(const float linearDamping)
	{
		PhysicsBody::SetLinearDamping(linearDamping* GE_PHYSICS_SCALAR);
		m_body->SetLinearDamping(linearDamping);
	}

	void PhysicsBody_box2d::SetAngularDamping(const float angularDamping)
	{
		PhysicsBody::SetAngularDamping(angularDamping);
		m_body->SetAngularDamping(angularDamping);
	}

	void PhysicsBody_box2d::SetEnabled(const bool b)
	{
		PhysicsBody::SetEnabled(b);
		m_body->SetEnabled(b);
	}

	void PhysicsBody_box2d::SetFixedRotation(const bool b)
	{
		PhysicsBody::SetFixedRotation(b);
		m_body->SetFixedRotation(b);
	}

	void PhysicsBody_box2d::SetGravityScale(const float f)
	{
		PhysicsBody::SetGravityScale(f);
		m_body->SetGravityScale(f);
	}

	void PhysicsBody_box2d::SetScale(const glm::vec2& scale)
	{
		PhysicsBody::SetScale(scale);
	}

	const glm::vec2& PhysicsBody_box2d::GetPosition()
	{
		const b2Vec2& pos = m_body->GetPosition();
		m_position = glm::vec2(pos.x/ GE_PHYSICS_SCALAR, pos.y/ GE_PHYSICS_SCALAR);
		return m_position;
	}

	const float PhysicsBody_box2d::GetRotation()
	{
		return  m_body->GetAngle();
	}

	const glm::vec2& PhysicsBody_box2d::GetLinearVelocity()
	{
		const b2Vec2& v = m_body->GetLinearVelocity();
		m_linearVelocity = glm::vec2(v.x/ GE_PHYSICS_SCALAR, v.y/ GE_PHYSICS_SCALAR);
		return m_linearVelocity;
	}

	void PhysicsBody_box2d::SetQuad(const glm::vec2& size, const glm::vec2& offset, float mass)
	{
		b2PolygonShape shape;
		shape.SetAsBox(size.x * GE_PHYSICS_SCALAR * 0.5f - shape.m_radius, size.y * GE_PHYSICS_SCALAR * 0.5f - shape.m_radius, b2Vec2(offset.x, offset.y), GetRotation());
		m_body->CreateFixture(&shape, mass);
	}


	void PhysicsBody_box2d::SetAngularVelocity(const float f)
	{
		PhysicsBody::SetAngularVelocity(f);
		m_body->SetAngularVelocity(f);
	}

	const float PhysicsBody_box2d::GetAngularVelocity()
	{
		return m_body->GetAngularVelocity();
	}

}
#endif