#include "gepch.h"
#ifdef GE_PHYSICS_BOX2D

#include "Public/Core/Platform/Physics/box2d/PhysicsBody_box2d.h"
#include "Public/Core/Physics/PhysicsStructures.h"
#include "Public/Core/Physics/Physics.h"
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>

#include "Public/Core/Util/Time.h"



namespace GEngine {

	PhysicsBody_box2d::PhysicsBody_box2d(void* nativeBody) : m_body((b2Body*)nativeBody)
	{
		const b2Vec2& pos = m_body->GetPosition();
		m_position = { pos.x* GE_PHYSICS_SCALAR, pos.y* GE_PHYSICS_SCALAR };
		m_rotation = m_body->GetAngle();
		
	}

	PhysicsBody_box2d::~PhysicsBody_box2d()
	{
		b2World* world = (b2World*)Physics::GetWorld();
		if (world != nullptr) {
			for (const std::pair<ColliderID, b2Fixture*>& f : m_fixtures) {
				m_body->DestroyFixture(f.second);
			}
			world->DestroyBody(m_body);
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

	const float PhysicsBody_box2d::GetBounce(const ColliderID id)
	{
		return m_fixtures[id]->GetRestitution();
	}

#define PI 3.14159265358979323846

	const ColliderID PhysicsBody_box2d::CreateQuad(const glm::vec2& size, const glm::vec2& offset, float mass, float rotation, const std::string& tag)
	{
		const ColliderID id = Time::GetEpochTimeNS();
		
		b2PolygonShape shape;
		shape.SetAsBox(size.x * GE_PHYSICS_SCALAR * 0.5f - shape.m_radius, size.y * GE_PHYSICS_SCALAR * 0.5f - shape.m_radius, 
			b2Vec2(offset.x, offset.y), (float) ((PI / 180.f) * rotation) );
		m_fixtures[id] = m_body->CreateFixture(&shape, mass);
		m_fixtureData[id].parent = m_self.parent;
		m_fixtureData[id].tag = tag;
		m_fixtures[id]->SetUserData((void*)&m_fixtureData[id]);
		m_fixtures[id]->SetSensor(m_sensor);
		m_fixtures[id]->SetDensity(0);
		b2Filter filter;
		filter.categoryBits = m_categoryBits;
		filter.groupIndex = m_groupIndex;
		filter.maskBits = m_maskBits;
		m_fixtures[id]->SetFilterData(filter);
		return id;
	}


	const GEngine::ColliderID PhysicsBody_box2d::CreateCircle(const glm::vec2& size, const glm::vec2& offset /*= glm::vec2(0)*/, float mass /*= 0*/, float rotation /*= 0*/, const std::string& tag /*= ""*/)
	{
		const ColliderID id = Time::GetEpochTimeNS();

		b2CircleShape shape;
		shape.m_p = b2Vec2(offset.x, offset.y);
		shape.m_radius = size.x;
		m_fixtures[id] = m_body->CreateFixture(&shape, mass);
		m_fixtureData[id].parent = m_self.parent;
		m_fixtureData[id].tag = tag;
		m_fixtures[id]->SetUserData((void*)&m_fixtureData[id]);
		m_fixtures[id]->SetSensor(m_sensor);
		m_fixtures[id]->SetDensity(0);
		b2Filter filter;
		filter.categoryBits = m_categoryBits;
		filter.groupIndex = m_groupIndex;
		filter.maskBits = m_maskBits;
		m_fixtures[id]->SetFilterData(filter);
		return id;
	}

	void PhysicsBody_box2d::DestroyQuad(const ColliderID id)
	{
		m_body->DestroyFixture(m_fixtures[id]);
		m_fixtureData.erase(id);
		m_fixtures.erase(id);
	}

	void PhysicsBody_box2d::SetOnCollideStartFunction(const ColliderID id, std::function<void(Ref<PhysicsCollision>)> f)
	{
		m_fixtureData[id].onStartCollide = f;
	}

	void PhysicsBody_box2d::SetOnCollideEndFunction(const ColliderID id, std::function<void(Ref<PhysicsCollision>)> f)
	{
		m_fixtureData[id].onEndCollide = f;
	}


	void PhysicsBody_box2d::SetAngularVelocity(const float f)
	{
		PhysicsBody::SetAngularVelocity(f);
		m_body->SetAngularVelocity(f);
	}

	void PhysicsBody_box2d::SetAwake(bool b)
	{
		PhysicsBody::SetAwake(b);
		m_body->SetAwake(b);
	}

	void PhysicsBody_box2d::SetMask(const ColliderID id, const uint16_t bits)
	{
		PhysicsBody::SetMask(id, bits);
		if (m_fixtures[id]) {
			b2Filter filter = m_fixtures[id]->GetFilterData();
			filter.maskBits = bits;
			m_fixtures[id]->SetFilterData(filter);
		}

	}

	void PhysicsBody_box2d::SetCategory(const ColliderID id, const uint16_t bits)
	{
		PhysicsBody::SetCategory(id, bits);
		if (m_fixtures[id]) {
			b2Filter filter = m_fixtures[id]->GetFilterData();
			filter.categoryBits = bits;
			m_fixtures[id]->SetFilterData(filter);
		}
	}

	void PhysicsBody_box2d::SetGroupIndex(const ColliderID id, const int16_t index)
	{
		PhysicsBody::SetGroupIndex(id, index);
		if (m_fixtures[id]) {
			b2Filter filter = m_fixtures[id]->GetFilterData();
			filter.groupIndex = index;
			m_fixtures[id]->SetFilterData(filter);
		}
	}

	void PhysicsBody_box2d::SetSensor(const ColliderID id, const bool b)
	{
		PhysicsBody::SetSensor(id, b);
		if (m_fixtures[id])
			m_fixtures[id]->SetSensor(m_sensor);
	}

	void PhysicsBody_box2d::SetBounce(const ColliderID id, const float f)
	{
		PhysicsBody::SetBounce(id, f);
		m_fixtures[id]->SetRestitution(f);
	}

	const float PhysicsBody_box2d::GetAngularVelocity()
	{
		return m_body->GetAngularVelocity();
	}



}
#endif