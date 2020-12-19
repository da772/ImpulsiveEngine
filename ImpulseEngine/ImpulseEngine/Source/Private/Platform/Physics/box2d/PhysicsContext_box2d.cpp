#include "gepch.h"
#ifdef GE_PHYSICS_BOX2D

#include "Public/Core/Physics/PhysicsStructures.h"
#include "Public/Platform/Physics/box2d/PhysicsContext_box2d.h"
#include "Public/Platform/Physics/box2d/PhysicsBody_box2d.h"
#include "Public/Core/Physics/Physics.h"
#include "Public/Core/Collision/Collider.h"
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_math.h"
#include "box2d/b2_contact.h"
#include "box2d/b2_world_callbacks.h"
#include "Public/Core/Util/GEMath.h"


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
					if (p1->onStartCollide)
						p1->onStartCollide(make_shared<PhysicsCollision>( c2, p2->tag ));
					if (p2->onStartCollide)
						p2->onStartCollide(make_shared<PhysicsCollision>(c1, p1->tag));
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
					if (p1->onEndCollide)
						p1->onEndCollide(make_shared<PhysicsCollision>(c2, p2->tag));
					if (p2->onEndCollide)
						p2->onEndCollide(make_shared<PhysicsCollision>(c1, p1->tag));
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

	void PhysicsContext_box2d::SetGravity(const Vector2f& gravity)
	{
		GE_CORE_ASSERT(m_world, "WORLD NOT CREATED");
		m_gravity = gravity;
		m_world->SetGravity(b2Vec2(m_gravity.x, m_gravity.y));
	}

	const Vector2f PhysicsContext_box2d::GetTrajectoryPoint2D(const Vector2f& startPos, const Vector2f& startVel, float step)
	{
		float t = 1.f / 60.f; // seconds per time step (at 60fps)
		Vector2f stepVelocity = { t * startVel.x, t * startVel.y }; // m/s
		Vector2f stepGravity = { t * t *m_world->GetGravity().x , t * t* m_world->GetGravity().y }; // m/s/s
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
			hitInfo->hitPoint = Vector2f(point.x, point.y);
			hitInfo->hitNormal= Vector2f(normal.x, normal.y);
			hitInfo->fraction = fraction;
			return 0;
		}

		Ref<RayCastInfo> hitInfo = nullptr;
		const std::vector<Weak<PhysicsBody>>& ignoreBodies;

	};

	Ref<RayCastInfo> PhysicsContext_box2d::RayCast2D(const Vector2f& start, const Vector2f& end, const std::vector<Weak<PhysicsBody>>& ignoreBodies)
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


	std::vector<Weak<PhysicsBody>> PhysicsContext_box2d::QueryCollision(const Vector2f& position, const Vector2f& scale, const std::vector<Weak<PhysicsBody>>& ignoreBodies)
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


	float PhysicsContext_box2d::GetVelocityMaxHeight(const Vector2f& velocity)
	{
		return CalculateVerticalVelocityForHeight(velocity.y);
	}

	Vector2f PhysicsContext_box2d::GetVelocityToPosition(const Vector2f& startPos, const Vector2f& endPos)
	{

		float velY = CalculateVerticalVelocityForHeight(endPos.y);
		Vector2f sVel = { 0, velY };
		float topTime = TimeToTop(sVel);
		float targetPosX = endPos.x;
		float velX = targetPosX / topTime * 60.f;
		return { velX, velY };

	}

	float PhysicsContext_box2d::CalculateVerticalVelocityForHeight(float height)
	{
		float t = 1 / 60.f;
		Vector2f stepGravity = t * t* Vector2f( m_world->GetGravity().x,m_world->GetGravity().y);

		float a = .5f / stepGravity.y;
		float b = .5f;
		float c = height;

		float q1 = (-b - std::sqrt(b * b - 4 * a * c)) / (2 * a);
		float q2 = (-b + std::sqrt(b * b - 4 * a * c)) / (2 * a);

		float v = q1;
		if (v < 0)
			v = q2;

		return v * 60.f;
	}

	float PhysicsContext_box2d::TimeToTop(const Vector2f& velocity)
	{
		float t = 1 / 60.0f;
		Vector2f stepVelocity = t * velocity; // m/s
		Vector2f stepGravity = t * t* Vector2f(m_world->GetGravity().x, m_world->GetGravity().y); // m/s/s

		float n = -stepVelocity.y / stepGravity.y - 1;
		return n;
	}

	float PhysicsContext_box2d::GetMaxVelocityTime(const Vector2f& velocity)
	{
		return TimeToTop(velocity);
	}

	float PhysicsContext_box2d::GetMaxHeight(const Vector2f& startPos, const Vector2f& startVel)
	{
		float t = 1 / 60.f;
		Vector2f stepVel = t * startVel;
		Vector2f stepGravity = t * t * Vector2f(m_world->GetGravity().x, m_world->GetGravity().y);
		float n = -stepVel.y / stepGravity.y - 1;
		return startPos.y + n * stepVel.y + .5f * (n * n + n) * stepGravity.y;
	}

}

#endif