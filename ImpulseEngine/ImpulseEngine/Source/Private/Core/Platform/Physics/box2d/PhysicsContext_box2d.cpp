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
		def.linearDamping = info.linearDamping * GE_PHYSICS_SCALAR;
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

	const glm::vec2 PhysicsContext_box2d::GetTrajectoryPoint2D(const glm::vec2& startPos, const glm::vec2& startVel, float step)
	{
		float t = 1.f / 60.f; // seconds per time step (at 60fps)
		glm::vec2 stepVelocity = { t * startVel.x, t * startVel.y }; // m/s
		glm::vec2 stepGravity = { t * t *m_world->GetGravity().x , t * t* m_world->GetGravity().y }; // m/s/s
		return { startPos.x + step * stepVelocity.x + 0.5f * (step * step + step) * stepGravity.x, startPos.y + step * stepVelocity.y +.5f * (step*step+step) * stepGravity.y };
		
	}



	class _b2_RayCastCallback : public b2RayCastCallback {

	public:
		_b2_RayCastCallback(const std::vector<Weak<PhysicsBody>>& bodies) : ignoreBodies(bodies) {

		}
		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override
		{
			hitInfo = make_shared<RayCastInfo>();
			
			Ref<PhysicsBody> _pbody = ((PhysicsParent*)(fixture->GetUserData()))->parent.lock();

			for (const Weak<PhysicsBody>& b : ignoreBodies) {
				if (b.lock() == _pbody) {
					return -1;
				}
			}

			hitInfo->physicsBody = _pbody;
			hitInfo->hitPoint = glm::vec2(point.x, point.y);
			hitInfo->hitNormal= glm::vec2(normal.x, normal.y);
			hitInfo->fraction = fraction;
			return 0;
		}

		Ref<RayCastInfo> hitInfo = nullptr;
		const std::vector<Weak<PhysicsBody>>& ignoreBodies;

	};

	Ref<RayCastInfo> PhysicsContext_box2d::RayCast2D(const glm::vec2& start, const glm::vec2& end, const std::vector<Weak<PhysicsBody>>& ignoreBodies)
	{

		_b2_RayCastCallback callback(ignoreBodies);
		m_world->RayCast(&callback, { start.x, start.y }, { end.x, end.y });
		return callback.hitInfo;
	}

	class __b2QueryCallback : public b2QueryCallback {
		public:
		__b2QueryCallback(const std::vector<Weak<PhysicsBody>>& ignoreBodies) : ignoreBodies(ignoreBodies) {
		}
	
		bool ReportFixture(b2Fixture* fixture) override
		{
			Ref<PhysicsBody> _pbody = ((PhysicsParent*)(fixture->GetUserData()))->parent.lock();

			for (const Weak<PhysicsBody>& b : ignoreBodies) {
				if (b.lock() == _pbody) {
					return -1;
				}
			}

			if (_pbody != nullptr) {
				hitBodies.push_back(_pbody);
			}
			return 0;
		}

		std::vector<Weak<PhysicsBody>> hitBodies;	
		const std::vector<Weak<PhysicsBody>>& ignoreBodies;
	};


	std::vector<Weak<PhysicsBody>> PhysicsContext_box2d::QueryCollision(const glm::vec2& position, const glm::vec2& scale, const std::vector<Weak<PhysicsBody>>& ignoreBodies)
	{
		__b2QueryCallback callback(ignoreBodies);
		b2AABB aabb;
		aabb.lowerBound = { position.x - scale.x/2.f , position.y - scale.y/2.f };
		aabb.upperBound = { position.x + scale.x/2.f, position.y + scale.y/2.f };
		m_world->QueryAABB(&callback, aabb);
		return callback.hitBodies;
	}

	void PhysicsContext_box2d::Simulate(float timeStep, int velIteration, int posIteration)
	{
		GE_CORE_ASSERT(m_world, "WORLD NOT CREATED");
		m_world->Step(timeStep, velIteration, posIteration);
	}


}

#endif