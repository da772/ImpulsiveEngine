#pragma once

#include "Public/Core/Physics/PhysicsStructures.h"

namespace GEngine {

#define GE_PHYSICS_SCALAR 1.f

	class PhysicsBody {


	public:
		inline PhysicsBody() {};
		inline virtual ~PhysicsBody() {};

		virtual void SetQuad(const glm::vec2& size, const glm::vec2& offset = glm::vec2(0), float mass = 0) = 0;

		virtual void SetPosition(const glm::vec2& position) { m_position = position; }
		virtual void SetRotation(const float rot) { m_rotation = rot; }
		
		virtual void SetLinearVelocity(const glm::vec2& velocity) { m_linearVelocity = velocity; }
		virtual void SetLinearDamping(const float linearDamping) { m_linearDamping = linearDamping; }
		virtual void SetAngularDamping(const float angularDamping) { m_angularDamping = angularDamping; }
		virtual void SetEnabled(const bool b) { m_enabled = b; }
		virtual void SetFixedRotation(const bool b) { m_fixedRotation = b; }
		virtual void SetGravityScale(const float f) { m_gravityScale = f; }
		virtual void SetAngularVelocity(const float f) { m_angluarVelocity = f; }

		virtual void SetScale(const glm::vec2& scale) { m_scale = scale; }

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



	protected:
		PhysicsInfoType m_type = PhysicsInfoType::PHYSICS_Static;
		glm::vec2 m_position = glm::vec2(0);;
		float m_rotation = 0.f;
		glm::vec2 m_linearVelocity = glm::vec2(0, 0);
		float m_angluarVelocity = 0;
		float m_linearDamping = .1f;
		float m_angularDamping = .1f;
		bool m_canSleep = true;
		bool m_awake = true;
		bool m_fixedRotation = false;
		bool m_bullet = false;
		bool m_enabled = true;
		void* m_userData = nullptr;
		float m_gravityScale = 1.f;



		// Fixture
		glm::vec2 m_scale = glm::vec2(1);;
		float m_mass = 0;





	};

}