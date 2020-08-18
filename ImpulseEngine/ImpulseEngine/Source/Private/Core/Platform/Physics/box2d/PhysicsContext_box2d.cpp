#include "gepch.h"
#ifdef GE_PHYSICS_BOX2D

#include "Public/Core/Physics/PhysicsStructures.h"
#include "Public/Core/Platform/Physics/box2d/PhysicsContext_box2d.h"
#include "Public/Core/Platform/Physics/box2d/PhysicsBody_box2d.h"
#include "Public/Core/Physics/Physics.h"
#include "Public/Core/Collision/Collider.h"
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_math.h"
#include "box2d/b2_contact.h"
#include "box2d/b2_world_callbacks.h"



namespace GEngine {

	class ContactListener : public b2ContactListener
	{
		void BeginContact(b2Contact* contact) {

			
			PhysicsParent* p1 = (PhysicsParent*)(contact->GetFixtureA()->GetUserData());
			PhysicsParent* p2 = (PhysicsParent*)(contact->GetFixtureB()->GetUserData());
			if (p1 && p2) {
				Ref<PhysicsBody> c1 = (p1)->parent.lock();
				Ref<PhysicsBody> c2 = (p2)->parent.lock();
				if (c1 && c2) {
					c1->CollideStart(c2);
					c2->CollideStart(c1);
				}
			}
			
		}

		void EndContact(b2Contact* contact) {

			
			PhysicsParent* p1 = (PhysicsParent*)(contact->GetFixtureA()->GetUserData());
			PhysicsParent* p2 = (PhysicsParent*)(contact->GetFixtureB()->GetUserData());
			if (p1 && p2) {
				Ref<PhysicsBody> c1 = (p1)->parent.lock();
				Ref<PhysicsBody> c2 = (p2)->parent.lock();
				if (c1 && c2) {
					c1->CollideEnd(c2);
					c2->CollideEnd(c1);
				}
			}
			
		}
	};

	ContactListener m_listener;

	PhysicsContext_box2d::PhysicsContext_box2d() : m_world( new b2World(b2Vec2(m_gravity.x, m_gravity.y)) )
	{
		m_world->SetContactListener(&m_listener);
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
		Ref<PhysicsBody_box2d> b = make_shared<PhysicsBody_box2d>(bdy);
		b->SetSelf(b);
		return b;
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