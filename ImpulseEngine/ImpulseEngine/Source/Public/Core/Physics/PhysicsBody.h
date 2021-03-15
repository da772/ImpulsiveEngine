#pragma once

#include "Public/Core/Physics/PhysicsStructures.h"

namespace GEngine {

#define PHYSICS_LAYER_DEFAULT 0x02
#define PHYSICS_LAYER_01 0x04
#define PHYSICS_LAYER_02 0x08
#define PHYSICS_LAYER_03 0x10
#define PHYSICS_LAYER_04 0x20
#define PHYSICS_LAYER_05 0x40
#define PHYSICS_LAYER_06 0x80
#define PHYSICS_LAYER_07 0x100
#define PHYSICS_LAYER_08 0x200
#define GE_PHYSICS_SCALAR 1.f

	class Component;

	class PhysicsBody {


	public:
		PhysicsBody();
		virtual ~PhysicsBody();

		inline virtual void SetPosition(const Vector2f& position) { m_position = position; }
		inline virtual void SetRotation(const float rot) { m_rotation = rot; }

		inline virtual void SetAwake(bool b) { m_awake = b; }

		inline virtual void SetLinearVelocity(const Vector2f& velocity) { m_linearVelocity = velocity; }
		inline virtual void SetLinearDamping(const float linearDamping) { m_linearDamping = linearDamping; }
		inline virtual void SetAngularDamping(const float angularDamping) { m_angularDamping = angularDamping; }
		inline virtual void SetEnabled(const bool b) { m_enabled = b; }
		inline virtual void SetFixedRotation(const bool b) { m_fixedRotation = b; }
		inline virtual void SetGravityScale(const float f) { m_gravityScale = f; }
		inline virtual void SetAngularVelocity(const float f) { m_angluarVelocity = f; }

		inline virtual void SetBounce(const ColliderID id, const float f) { m_bounce = f; };
		inline virtual void SetScale(const Vector2f& scale) { m_scale = scale; }

		inline virtual void SetSensor(const ColliderID id, const bool b) { m_sensor = b; }

		inline virtual const Vector2f& GetPosition() { return m_position; }
		inline virtual const float GetRotation() { return m_rotation; }
		inline virtual const Vector2f& GetScale() { return m_scale; }
		inline virtual const Vector2f& GetLinearVelocity() { return m_linearVelocity; }
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
		inline virtual const float GetBounce(const ColliderID id) { return m_bounce; }

		inline virtual void SetMask(const ColliderID id,  uint16_t bits) { m_maskBits = bits; }
		inline virtual void SetCategory(const ColliderID id, const uint16_t bits) { m_categoryBits = bits; }
		inline virtual void SetGroupIndex(const ColliderID id, const int16_t index) { m_groupIndex = index; }

		inline virtual void SetComponent(Component* c) { m_component = c; }
		inline virtual void SetSelf(Weak<PhysicsBody> self) { m_self.parent = self; }

		inline virtual Component* GetComponent() { return m_component; }

		virtual void SetOnCollideStartFunction(const ColliderID id, std::function<void(Ref<PhysicsCollision>)> f) = 0;
		virtual void SetOnCollideEndFunction(const ColliderID id, std::function<void(Ref<PhysicsCollision>)> f) = 0;


		virtual const ColliderID CreateQuad(const Vector2f& size, const Vector2f& offset = Vector2f(0.f), float mass = 0, float rotation = 0, const std::string& tag = "") = 0;
		virtual const ColliderID CreateCircle(const Vector2f& size, const Vector2f& offset = Vector2f(0.f), float mass = 0, float rotation = 0, const std::string& tag = "") = 0;
		virtual void DestroyQuad(const ColliderID id) = 0;


	protected:

		PhysicsInfoType m_type = PhysicsInfoType::PHYSICS_Static;
		Vector2f m_position = Vector2f(0.f);
		float m_rotation = 0.f;
		Vector2f m_linearVelocity = Vector2f(0, 0);
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
		uint16_t m_categoryBits = PHYSICS_LAYER_DEFAULT;
		uint16_t m_maskBits = PHYSICS_LAYER_DEFAULT | PHYSICS_LAYER_01;
		int16_t m_groupIndex = PHYSICS_LAYER_DEFAULT | PHYSICS_LAYER_01;

		PhysicsParent m_self;
		Component* m_component;


		// Fixture
		Vector2f m_scale = Vector2f(1.f);
		float m_mass = 0;





	};

}
