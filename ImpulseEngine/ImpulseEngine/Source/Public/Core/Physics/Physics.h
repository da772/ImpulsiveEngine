#pragma once


namespace GEngine {

	class PhysicsContext;
	struct PhysicsInfo;
	class PhysicsBody;
	struct RayCastInfo;



	class Physics {

	public:
		static void Initialize();
		static void Shutdown();
		static void* GetWorld();
		static void SetGravity(const Vector2f& gravity);
		static Ref<PhysicsBody> CreateBody(const PhysicsInfo& info);
		static Vector2f GetTrajectoryPoint2D(const Vector2f& startPos, const Vector2f& startVel, float step);
		static Ref<RayCastInfo> RayCast2D(const Vector2f startPos, const Vector2f endPos, const std::vector< Weak<PhysicsBody>>& ignoreBodies = {});
		static std::vector<Weak<PhysicsBody>> QueryCollision(const Vector2f& position, const Vector2f& scale, const std::vector<Weak<PhysicsBody>>& ignoreBodies);
		static void Update(float timestep);
		static Vector2f GetVelocityToPosition(const Vector2f& startPos, const Vector2f& endPos);
		static float GetVelocityMaxHeight(const Vector2f& velocity);
		static float GetMaxVelocityTime(const Vector2f& velocity);
		static float GetMaxHeight(const Vector2f& startPos, const Vector2f& startVel);

	private:
		static Ref<PhysicsContext> m_context;
		static double m_lastUpdate;
		static int m_catchUp;

	};

}
