#include "gepch.h"
#include "Public/Core/Physics/Physics.h"
#include "Public/Core/Physics/PhysicsContext.h"
#ifdef GE_PHYSICS_BOX2D
#include "Public/Core/Platform/Physics/box2d/PhysicsContext_box2d.h"
#endif

namespace GEngine {

	Ref<PhysicsContext> Physics::m_context;

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

	void Physics::Update(float timestep)
	{
		GE_CORE_ASSERT(m_context, "PHYSICS CONTEXT NOT CREATED");
		m_context->Simulate(timestep);
	}

}