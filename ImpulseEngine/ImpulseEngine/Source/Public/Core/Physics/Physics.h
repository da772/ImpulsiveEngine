#pragma once


namespace GEngine {

	class PhysicsContext;
	struct PhysicsInfo;
	class PhysicsBody;
	

	class Physics {

	public:
		static void Initalize();
		static void Shutdown();
		static void* GetWorld();
		static void SetGravity(const glm::vec2& gravity);
		static Ref<PhysicsBody> CreateBody(const PhysicsInfo& info);
		static void Update(float timestep);

	private:
		static Ref<PhysicsContext> m_context;

	};

}
