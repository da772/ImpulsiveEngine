#pragma once


namespace GEngine {

	class PhysicsBody;
	struct PhysicsInfo;


	class PhysicsContext {

	public:
		PhysicsContext() {};
		virtual ~PhysicsContext() {};

		virtual Ref<PhysicsBody> CreateBody(const PhysicsInfo& info) = 0;
		virtual void SetGravity(const glm::vec2& gravity) = 0;
		virtual void* GetNativeWorld() = 0;

		virtual void Simulate(float timeStep, int velIteration = 8, int posIteration = 2) = 0;


	protected:

		glm::vec2 m_gravity = glm::vec2(0, -10.f);



	};

}