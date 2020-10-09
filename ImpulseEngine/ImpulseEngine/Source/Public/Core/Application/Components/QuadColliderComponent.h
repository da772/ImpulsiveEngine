#pragma once

#include "Public/Core/Application/Component.h"

namespace GEngine {

	class Collider2D;
	class ScriptObject;
	struct ScriptVector2;
	class PhysicsBody;

	class QuadColliderComponent : public Component {
	public:
		QuadColliderComponent(bool dyamic = false, bool physics = false, const glm::vec2& position = glm::vec2(0,0) , const glm::vec2& scale = glm::vec2(1,1), const float rotation = 0, const float mass = 0);
		~QuadColliderComponent();

		virtual void OnAttached(Ref<Entity> entity) override;
		inline virtual void DeAttached(Ref<Entity> entity) override;

		virtual void OnBegin() override;
		virtual void OnEnd() override;

		virtual void OnUpdate(Timestep ts) override;

		void SetPosition(const float x, const float y);
		void SetScale(const float x, const float y);

		const glm::vec2 GetPosition();
		const glm::vec2 GetScale();

		void SetGravityScale(const float f);


		const glm::vec2 GetLinearVelocity();

		Ref<ScriptVector2> GetLinearVelocityScript();
		Ref<ScriptVector2> GetPositionScript();
		Ref<ScriptVector2> GetScaleScript();

		void SetBounce(const float bounce);
		const float GetBounce();
		
		void SetDynamic(bool b);

		Ref<PhysicsBody> GetPhysicsBody() const { return m_body; }

		void IncreaseLinearVelocity(const float x, const float y);
		void IncreaseAngularVelocity(const float v);

		void SetVelocityX(const float x);
		void SetVelocityY(const float y);
		void SetVelocity(const float x, const float y);

		void SetCollisionLayers(const uint16_t category, const uint16_t mask, const int16_t index);

		void WakeBody();

		void SetOnCollideFunction(std::function<void(Ref<QuadColliderComponent>)> onCollideFunc);
		void SetOnCollideFunction_Script(Ref<ScriptObject> onColliderFunc);
		void SetEndCollideFunction(std::function<void(Ref<QuadColliderComponent>)> onCollideFunc);
		void SetEndCollideFunction_Script(Ref<ScriptObject> onCollideFunc);
		void RemoveOnCollideFunction();
		void RemoveEndCollideFunction();

	private:
		glm::vec2 m_position;
		glm::vec2 m_scale;
		glm::vec2 m_worldPosition;
		glm::vec2 m_worldScale;
		float m_worldRotation;
		bool m_physics =false;
		float m_rotation;
		bool m_dynamic = false;
		float m_mass = 0;
		Ref<PhysicsBody> m_body;
		bool m_movedSelf = false;
		uint16_t m_category = 0x02;
		int16_t m_groupIndex = 0x02;
		uint16_t m_mask = 0x02;

		std::function<void(Ref<QuadColliderComponent>)> m_onCollide;
		std::function<void(Ref<QuadColliderComponent>)> m_endCollide;



	};
}