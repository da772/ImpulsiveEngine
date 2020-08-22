#pragma once

#include "Public/Core/Application/Component.h"

namespace GEngine {

	class Collider2D;
	class ScriptObject;
	struct ScriptVector2;
	class PhysicsBody;

	class QuadColliderComponent : public Component {
	public:
		QuadColliderComponent(bool dyamic = false, bool physics = false, glm::vec2 position = glm::vec2(0,0) , glm::vec2 scale = glm::vec2(1,1), float rotation = 0, float mass = 0);
		~QuadColliderComponent();

		virtual void OnAttached(Ref<Entity> entity) override;
		inline virtual void DeAttached(Ref<Entity> entity) override;

		virtual void OnBegin() override;
		virtual void OnEnd() override;

		virtual void OnUpdate(Timestep ts) override;

		void SetPosition(float x, float y);
		void SetScale(float x, float y);

		glm::vec2 GetPosition();
		glm::vec2 GetScale();


		const glm::vec2 GetLinearVelocity();

		Ref<ScriptVector2> GetLinearVelocityScript();
		Ref<ScriptVector2> GetPositionScript();
		Ref<ScriptVector2> GetScaleScript();

		void SetBounce(float bounce);
		float GetBounce();
		
		void SetDynamic(bool b);

		void IncreaseLinearVelocity(float x, float y);
		void IncreaseAngularVelocity(float v);

		void SetVelocityX(float x);
		void SetVelocityY(float y);
		void SetVelocity(float x, float y);

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

		std::function<void(Ref<QuadColliderComponent>)> m_onCollide;
		std::function<void(Ref<QuadColliderComponent>)> m_endCollide;



	};
}