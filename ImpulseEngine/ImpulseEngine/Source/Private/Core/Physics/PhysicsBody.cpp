#include "gepch.h"

#include "Public/Core/Physics/PhysicsBody.h"

namespace GEngine {



	PhysicsBody::PhysicsBody()
	{
		
	}

	PhysicsBody::~PhysicsBody()
	{
		
	}

	void PhysicsBody::CollideStart(Ref<PhysicsBody> other)
	{
		if (m_onCollideStartFunc) {
			m_onCollideStartFunc(other);
		}
	}

	void PhysicsBody::CollideEnd(Ref<PhysicsBody> other)
	{
		if (m_onCollideEndFunc) {
			m_onCollideEndFunc(other);
		}

	}

}