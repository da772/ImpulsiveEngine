#pragma once

#include "Public/Core/Physics/PhysicsStructures.h"

namespace GEngine {

#define GE_PHYSICS_SCALAR 1.f

	class Component;

	class PhysicsBody {


	public:
		PhysicsBody();
		virtual ~PhysicsBody();

		inline virtual void SetQuad(const glm::vec2& size, const glm::vec2& offset = glm::vec2(0), float mass = 0, float rotation = 0) = 0;

		inline virtual void SetPosition(const glm::vec2& position) { m_position = position; }
		inline virtual void SetRotation(const float rot) { m_rotation = rot; }

		inline virtual void SetLinearVelocity(const glm::vec2& velocity) { m_linearVelocity = velocity; }
		inline virtual void SetLinearDamping(const float linearDamping) { m_linearDamping = linearDamping; }
		inline virtual void SetAngularDamping(const float angularDamping) { m_angularDamping = angularDamping; }
		inline virtual void SetEnabled(const bool b) { m_enabled = b; }
		inline virtual void SetFixedRotation(const bool b) { m_fixedRotation = b; }
		inline virtual void SetGravityScale(const float f) { m_gravityScale = f; }
		inline virtual void SetAngularVelocity(const float f) { m_angluarVelocity = f; }

		inline virtual void SetBounce(const float f) { m_bounce = f; };
		inline virtual void SetScale(const glm::vec2& scale) { m_scale = scale; }

		inline virtual void SetSensor(const bool b) { m_sensor = b; }

		inline virtual const glm::vec2& GetPosition() { return m_position; }
		inline virtual const float GetRotation() { return m_rotation; }
		inline virtual const glm::vec2& GetScale() { return m_scale; }
		inline virtual const glm::vec2& GetLinearVelocity() { return m_linearVelocity; }
		inline virtual const float GetLinearDamping() { return m_linearDamping; }
		inline virtual const float GetAngularDamping() { return m_angularDamping; }
		inline virtual const bool CanSleep() { return m_canSleep; }
		inline virtual const bool IsAwake() { return m_awake; }
		inline virtual const bool IsFixedRotation() { return m_fixedRotation; }
		inline virtual const bool IsBullet() { return m_bullet; }
		inline virtual const bool IsEnabled() { return m_enabled; }
		inline virtual void* GetUserData() { return m_userData; }
		inline virtual const float GetGravityScale() { return m_gravityScale; }
		inline virtual const PhysicsInfoType& GetPhysicsType() { return m_type; }
		inline virtual const float GetAngularVelocity() { return m_angluarVelocity; };
		inline virtual const float GetBounce() { return m_bounce; }

		inline virtual void SetComponent(Weak<Component> c) { m_component = c; }
		inline virtual void SetSelf(Weak<PhysicsBody> self) { m_self.parent = self; }

		inline virtual Ref<Component> GetComponent() { return m_component.lock(); }

		inline virtual void SetOnCollideStartFunction(std::function<void(Ref<PhysicsBody>)> f) { m_onCollideStartFunc = f; }
		inline virtual void SetOnCollideEndFunction(std::function<void(Ref<PhysicsBody>)> f) { m_onCollideEndFunc = f; }

		virtual void CollideStart(Ref<PhysicsBody> other);
		virtual void CollideEnd(Ref<PhysicsBody> other);


	protected:
		std::function<void(Ref<PhysicsBody>)> m_onCollideStartFunc;
		std::function<void(Ref<PhysicsBody>)> m_onCollideEndFunc;

		PhysicsInfoType m_type = PhysicsInfoType::PHYSICS_Static;
		glm::vec2 m_position = glm::vec2(0);;
		float m_rotation = 0.f;
		glm::vec2 m_linearVelocity = glm::vec2(0, 0);
		float m_angluarVelocity = 0;
		float m_linearDamping = 0.f;
		float m_angularDamping = 0.f;
		bool m_canSleep = true;
		bool m_awake = true;
		bool m_fixedRotation = false;
		bool m_bullet = false;
		bool m_enabled = true;
		void* m_userData = nullptr;
		float m_gravityScale = 1.f;
		float m_bounce = 0.f;
		bool m_sensor = false;

		PhysicsParent m_self;
		Weak<Component> m_component;


		// Fixture
		glm::vec2 m_scale = glm::vec2(1);;
		float m_mass = 0;





	};

}
