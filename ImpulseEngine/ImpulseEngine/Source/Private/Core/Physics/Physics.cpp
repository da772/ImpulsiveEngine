#include "gepch.h"
#include "Public/Core/Physics/Physics.h"
#include "Public/Core/Physics/PhysicsContext.h"
#ifdef GE_PHYSICS_BOX2D
#include "Public/Core/Platform/Physics/box2d/PhysicsContext_box2d.h"
#endif

namespace GEngine {

	Ref<PhysicsContext> Physics::m_context;
	long long Physics::m_lastUpdate;

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

	void Physics::SetGravity(const glm::vec2& gravity)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		m_context->SetGravity(gravity);
	}

	Ref<PhysicsBody> Physics::CreateBody(const PhysicsInfo& info)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		return m_context->CreateBody(info);
	}

	glm::vec2 Physics::GetTrajectoryPoint2D(const glm::vec2& startPos, const glm::vec2& startVel, float step)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		return m_context->GetTrajectoryPoint2D(startPos, startVel, step);
	}

	GEngine::Ref<GEngine::RayCastInfo> Physics::RayCast2D(const glm::vec2 startPos, const glm::vec2 endPos, const std::vector< Weak<PhysicsBody>>& ignoreBodies)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		return m_context->RayCast2D(startPos, endPos, ignoreBodies);
	}

	std::vector<GEngine::Weak<GEngine::PhysicsBody>> Physics::QueryCollision(const glm::vec2& position, const glm::vec2& scale, const std::vector<Weak<PhysicsBody>>& ignoreBodies)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		return m_context->QueryCollision(position, scale, ignoreBodies);
	}

	void Physics::Update(float dt)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		m_context->Simulate(dt);
	}

	glm::vec2 Physics::GetVelocityToPosition(const glm::vec2& startPos, const glm::vec2& endPos)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		return m_context->GetVelocityToPosition(startPos, endPos);
	}

	float Physics::GetVelocityMaxHeight(const glm::vec2& velocity)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		return m_context->GetVelocityMaxHeight(velocity);
	}

	float Physics::GetMaxVelocityTime(const glm::vec2& velocity)
	{
		return m_context->GetMaxVelocityTime(velocity);
	}

	float Physics::GetMaxHeight(const glm::vec2& startPos, const glm::vec2& startVel)
	{
		return m_context->GetMaxHeight(startPos, startVel);
	}

}