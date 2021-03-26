#pragma once


namespace GEngine {

	class PhysicsBody;
	struct PhysicsInfo;

	struct RayCastInfo {
		Weak<PhysicsBody> physicsBody;
		Vector2f hitPoint;
		Vector2f hitNormal;
		float fraction;
	};


	class GE_API PhysicsContext {

	public:
		PhysicsContext() {};
		virtual ~PhysicsContext() {};

		virtual Ref<PhysicsBody> CreateBody(const PhysicsInfo& info) = 0;
		virtual void SetGravity(const Vector2f& gravity) = 0;
		virtual void* GetNativeWorld() = 0;
		virtual const Vector2f GetTrajectoryPoint2D(const Vector2f& startPos, const Vector2f& startVel, float step) = 0;
		virtual void Simulate(float timeStep, int velIteration = 6, int posIteration = 3) = 0;
		virtual Ref<RayCastInfo> RayCast2D(const Vector2f& startPos, const Vector2f& endPos, const std::vector<Weak<PhysicsBody>>& ignoreBodies) = 0;
		virtual std::vector<Weak<PhysicsBody>> QueryCollision(const Vector2f& position, const Vector2f& scale, const std::vector<Weak<PhysicsBody>>& ignoreBodies) = 0;
		virtual Vector2f GetVelocityToPosition(const Vector2f& startPos, const Vector2f& endPos) = 0;
		virtual float GetVelocityMaxHeight(const Vector2f& velocity) = 0;
		virtual float GetMaxVelocityTime(const Vector2f& velocity) = 0;
		virtual float GetMaxHeight(const Vector2f& startPos, const Vector2f& startVel) = 0;

	protected:

		Vector2f m_gravity = Vector2f(0, -10.f);



	};

}