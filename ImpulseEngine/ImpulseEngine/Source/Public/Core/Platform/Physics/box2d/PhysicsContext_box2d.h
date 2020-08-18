#pragma once
#ifdef GE_PHYSICS_BOX2D

#include "Public/Core/Physics/PhysicsContext.h"

class b2World;
class ContactListener;

namespace GEngine {

	class PhysicsContext_box2d : public PhysicsContext {
	public:
		PhysicsContext_box2d();
		virtual ~PhysicsContext_box2d();

		Ref<PhysicsBody> CreateBody(const PhysicsInfo& info) override;

		void* GetNativeWorld() override;
		void SetGravity(const glm::vec2& gravity) override;

		void Simulate(float timeStep, int velIteration = 8, int posIteration = 3) override;



	private:
		b2World* m_world;
		

	};



}

#endif
