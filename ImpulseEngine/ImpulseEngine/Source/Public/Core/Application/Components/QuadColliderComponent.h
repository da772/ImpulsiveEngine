#pragma once

#include "Public/Core/Application/Component.h"

namespace GEngine {

	class Collider2D;
	class ScriptObject;

	class QuadColliderComponent : public Component {
	public:
		QuadColliderComponent(bool dyamic = false, glm::vec2 position = glm::vec2(0,0) , glm::vec2 scale = glm::vec2(1,1), float rotation = 0);
		~QuadColliderComponent();

		virtual void OnAttached(Ref<Entity> entity) override;
		inline virtual void DeAttached(Ref<Entity> entity) override;

		virtual void OnBegin() override;
		virtual void OnEnd() override;

		virtual void OnUpdate(Timestep ts) override;
		
		void SetDynamic(bool b);

		void SetOnCollideFunction(std::function<void(Ref<QuadColliderComponent>)> onCollideFunc);
		void SetOnCollideFunction_Script(Ref<ScriptObject> onColliderFunc);
		void SetEndCollideFunction(std::function<void(Ref<QuadColliderComponent>)> onCollideFunc);
		void SetEndCollideFunction_Script(Ref<ScriptObject> onCollideFunc);
		void RemoveOnCollideFunction();
		void RemoveEndCollideFunction();

	private:
		glm::vec2 m_position;
		glm::vec2 m_scale;
		float m_rotation;
		bool m_dynamic = false;
		Ref<Collider2D> m_collider;
		std::function<void(Ref<QuadColliderComponent>)> m_onCollide;
		std::function<void(Ref<QuadColliderComponent>)> m_endCollide;



	};
}