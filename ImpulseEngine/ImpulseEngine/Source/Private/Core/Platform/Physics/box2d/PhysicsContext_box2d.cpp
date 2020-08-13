#include "gepch.h"
#ifdef GE_PHYSICS_BOX2D

#include "Public/Core/Physics/PhysicsStructures.h"
#include "Public/Core/Platform/Physics/box2d/PhysicsContext_box2d.h"
#include "Public/Core/Platform/Physics/box2d/PhysicsBody_box2d.h"\
#include "Public/Core/Physics/Physics.h"


#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_math.h"


namespace GEngine {

	PhysicsContext_box2d::PhysicsContext_box2d() : m_world( new b2World(b2Vec2(m_gravity.x, m_gravity.y)) )
	{
		
	}

	PhysicsContext_box2d::~PhysicsContext_box2d()
	{
		delete m_world;
	}

	Ref<PhysicsBody> PhysicsContext_box2d::CreateBody(const PhysicsInfo& info)
	{
		GE_CORE_ASSERT(m_world, "WORLD NOT CREATED");
		b2BodyDef def;
		def.type = (b2BodyType)info.type;
		def.position.Set(info.position.x* GE_PHYSICS_SCALAR, info.position.y* GE_PHYSICS_SCALAR);
		def.angle = info.rotation;
		def.linearVelocity.Set(info.linearVelocity.x * GE_PHYSICS_SCALAR, info.linearVelocity.y * GE_PHYSICS_SCALAR);
		def.linearDamping = info.angularDamping * GE_PHYSICS_SCALAR;
		def.angularDamping = info.angularDamping;
		def.allowSleep = info.canSleep;
		def.awake = info.awake;
		def.fixedRotation = info.fixedRotation;
		def.bullet = info.bullet;
		def.enabled = info.enabled;
		def.userData = info.userData;
		def.gravityScale = info.gravityScale * GE_PHYSICS_SCALAR;
		void* bdy = (void*)m_world->CreateBody(&def);
		
		return make_shared<PhysicsBody_box2d>(bdy);
	}

	void* PhysicsContext_box2d::GetNativeWorld()
	{
		return (void*)m_world;
	}

	void PhysicsContext_box2d::SetGravity(const glm::vec2& gravity)
	{
		GE_CORE_ASSERT(m_world, "WORLD NOT CREATED");
		m_gravity = gravity;
		m_world->SetGravity(b2Vec2(m_gravity.x, m_gravity.y));
	}

	void PhysicsContext_box2d::Simulate(float timeStep, int velIteration, int posIteration)
	{
		GE_CORE_ASSERT(m_world, "WORLD NOT CREATED");
		m_world->Step(timeStep, velIteration, posIteration);
	}


}

#endif