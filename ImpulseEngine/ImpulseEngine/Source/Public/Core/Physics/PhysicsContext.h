#pragma once


namespace GEngine {

	class PhysicsBody;
	struct PhysicsInfo;

	struct RayCastInfo {
		Weak<PhysicsBody> physicsBody;
		glm::vec2 hitPoint;
		glm::vec2 hitNormal;
		float fraction;
	};


	class PhysicsContext {

	public:
		PhysicsContext() {};
		virtual ~PhysicsContext() {};

		virtual Ref<PhysicsBody> CreateBody(const PhysicsInfo& info) = 0;
		virtual void SetGravity(const glm::vec2& gravity) = 0;
		virtual void* GetNativeWorld() = 0;
		virtual const glm::vec2 GetTrajectoryPoint2D(const glm::vec2& startPos, const glm::vec2& startVel, float step) = 0;
		virtual void Simulate(float timeStep, int velIteration = 8, int posIteration = 2) = 0;
		virtual Ref<RayCastInfo> RayCast2D(const glm::vec2& startPos, const glm::vec2& endPos, const std::vector<Weak<PhysicsBody>>& ignoreBodies) = 0;
		virtual std::vector<Weak<PhysicsBody>> QueryCollision(const glm::vec2& position, const glm::vec2& scale, const std::vector<Weak<PhysicsBody>>& ignoreBodies) = 0;
		virtual glm::vec2 GetVelocityToPosition(const glm::vec2& startPos, const glm::vec2& endPos) = 0;
		virtual float GetVelocityMaxHeight(const glm::vec2& velocity) = 0;
		virtual float GetMaxVelocityTime(const glm::vec2& velocity) = 0;
		virtual float GetMaxHeight(const glm::vec2& startPos, const glm::vec2& startVel) = 0;

	protected:

		glm::vec2 m_gravity = glm::vec2(0, -10.f);



	};

}