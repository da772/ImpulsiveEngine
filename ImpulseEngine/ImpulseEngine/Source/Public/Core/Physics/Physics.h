#pragma once


namespace GEngine {

	class PhysicsContext;
	struct PhysicsInfo;
	class PhysicsBody;
	struct RayCastInfo;



	class Physics {

	public:
		static void Initalize();
		static void Shutdown();
		static void* GetWorld();
		static void SetGravity(const glm::vec2& gravity);
		static Ref<PhysicsBody> CreateBody(const PhysicsInfo& info);
		static glm::vec2 GetTrajectoryPoint2D(const glm::vec2& startPos, const glm::vec2& startVel, float step);
		static Ref<RayCastInfo> RayCast2D(const glm::vec2 startPos, const glm::vec2 endPos, const std::vector< Weak<PhysicsBody>>& ignoreBodies = {});
		static std::vector<Weak<PhysicsBody>> QueryCollision(const glm::vec2& position, const glm::vec2& scale, const std::vector<Weak<PhysicsBody>>& ignoreBodies);
		static void Update(float timestep);
		static glm::vec2 GetVelocityToPosition(const glm::vec2& startPos, const glm::vec2& endPos);
		static float GetVelocityMaxHeight(const glm::vec2& velocity);
		static float GetMaxVelocityTime(const glm::vec2& velocity);
		static float GetMaxHeight(const glm::vec2& startPos, const glm::vec2& startVel);

	private:
		static Ref<PhysicsContext> m_context;
		static uint64_t m_lastUpdate;

	};

}
