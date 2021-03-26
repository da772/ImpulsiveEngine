#pragma once
#ifdef GE_PHYSICS_BOX2D

#include "Public/Core/Physics/PhysicsContext.h"

class b2World;
class ContactListener;

namespace GEngine {



	class GE_API PhysicsContext_box2d : public PhysicsContext {
	public:
		PhysicsContext_box2d();
		virtual ~PhysicsContext_box2d();

		Ref<PhysicsBody> CreateBody(const PhysicsInfo& info) override;

		void* GetNativeWorld() override;
		void SetGravity(const Vector2f& gravity) override;
		virtual const Vector2f GetTrajectoryPoint2D(const Vector2f& startPos, const Vector2f& startVel, float step) override;

		virtual Ref<RayCastInfo> RayCast2D(const Vector2f& start, const Vector2f& end, const std::vector<Weak<PhysicsBody>>& ignoreBodies) override;
		virtual std::vector<Weak<PhysicsBody>> QueryCollision(const Vector2f& position, const Vector2f& scale, const std::vector<Weak<PhysicsBody>>& ignoreBodies) override;
		void Simulate(float timeStep, int velIteration = 8, int posIteration = 3) override;
		virtual float GetVelocityMaxHeight(const Vector2f& velocity) override;
		virtual Vector2f GetVelocityToPosition(const Vector2f& startPos, const Vector2f& endPos) override;
		float CalculateVerticalVelocityForHeight(float height);
		float TimeToTop(const Vector2f& velocity);
		virtual float GetMaxVelocityTime(const Vector2f& velocity) override;
		virtual float GetMaxHeight(const Vector2f& startPos, const Vector2f& startVel) override;

	private:
		b2World* m_world;
		

	};



}

#endif
