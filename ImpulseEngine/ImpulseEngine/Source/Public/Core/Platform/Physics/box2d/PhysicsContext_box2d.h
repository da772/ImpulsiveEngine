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
		virtual const glm::vec2 GetTrajectoryPoint2D(const glm::vec2& startPos, const glm::vec2& startVel, float step) override;

		virtual Ref<RayCastInfo> RayCast2D(const glm::vec2& start, const glm::vec2& end, const std::vector<Weak<PhysicsBody>>& ignoreBodies) override;
		virtual std::vector<Weak<PhysicsBody>> QueryCollision(const glm::vec2& position, const glm::vec2& scale, const std::vector<Weak<PhysicsBody>>& ignoreBodies) override;
		void Simulate(float timeStep, int velIteration = 2, int posIteration = 3) override;



	private:
		b2World* m_world;
		

	};



}

#endif
