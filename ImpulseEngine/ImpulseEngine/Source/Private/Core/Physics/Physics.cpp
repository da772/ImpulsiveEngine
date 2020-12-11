#include "gepch.h"
#include "Public/Core/Physics/Physics.h"
#include "Public/Core/Physics/PhysicsContext.h"
#ifdef GE_PHYSICS_BOX2D
#include "Public/Core/Platform/Physics/box2d/PhysicsContext_box2d.h"
#endif

#include "Public/Core/Util/Time.h"

namespace GEngine {

	Ref<PhysicsContext> Physics::m_context;
	uint64_t Physics::m_lastUpdate;

	void Physics::Initalize()
	{
		GE_CORE_ASSERT(!m_context, "PHYSICS CONTEXT ALREADY CREATED");
		if (m_context == nullptr) {

#ifdef GE_PHYSICS_BOX2D
			m_context = make_shared<PhysicsContext_box2d>();
#endif
		}
	}

	void Physics::Shutdown()
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		m_context = nullptr;

	}

	void* Physics::GetWorld()
	{
		if (m_context)
			return m_context->GetNativeWorld();
		return nullptr;
	}

	void Physics::SetGravity(const Vector2f& gravity)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		m_context->SetGravity(gravity);
	}

	Ref<PhysicsBody> Physics::CreateBody(const PhysicsInfo& info)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		return m_context->CreateBody(info);
	}

	Vector2f Physics::GetTrajectoryPoint2D(const Vector2f& startPos, const Vector2f& startVel, float step)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		return m_context->GetTrajectoryPoint2D(startPos, startVel, step);
	}

	GEngine::Ref<GEngine::RayCastInfo> Physics::RayCast2D(const Vector2f startPos, const Vector2f endPos, const std::vector< Weak<PhysicsBody>>& ignoreBodies)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		return m_context->RayCast2D(startPos, endPos, ignoreBodies);
	}

	std::vector<GEngine::Weak<GEngine::PhysicsBody>> Physics::QueryCollision(const Vector2f& position, const Vector2f& scale, const std::vector<Weak<PhysicsBody>>& ignoreBodies)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		return m_context->QueryCollision(position, scale, ignoreBodies);
	}

	void Physics::Update(float dt)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		m_context->Simulate(dt);
		/*uint64_t ct = Time::GetEpochTimeMS();
		if (ct - m_lastUpdate >= 16) {
		
			m_lastUpdate = Time::GetEpochTimeMS();
		}
		*/

		
	}

	Vector2f Physics::GetVelocityToPosition(const Vector2f& startPos, const Vector2f& endPos)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		return m_context->GetVelocityToPosition(startPos, endPos);
	}

	float Physics::GetVelocityMaxHeight(const Vector2f& velocity)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		return m_context->GetVelocityMaxHeight(velocity);
	}

	float Physics::GetMaxVelocityTime(const Vector2f& velocity)
	{
		return m_context->GetMaxVelocityTime(velocity);
	}

	float Physics::GetMaxHeight(const Vector2f& startPos, const Vector2f& startVel)
	{
		return m_context->GetMaxHeight(startPos, startVel);
	}

}