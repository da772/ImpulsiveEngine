#pragma once

#include "Public/Core/Application/Component.h"

namespace GEngine {

	class BatchRenderer;
	class Texture2D;
	class SubTexture2D;
	class Collider2D;
	class Shader;
	class ScriptObject;
	class Event;

	class ButtonComponent : public Component {
	public:

		ButtonComponent(const glm::vec3& pos, const float rot, const glm::vec2& scale, const glm::vec4& color, glm::vec2 textureScale = { 1,1 });
		virtual ~ButtonComponent();

		void SetImageSubTexture(Ref<SubTexture2D> texture);
		void SetImageTexture(Ref<Texture2D> texture);
		void SetImageColor(const glm::vec4& color);
		void SetImageRotation(const float rot);
		void SetImagePosition(const glm::vec2& pos);
		void SetImageScale(const glm::vec2& scale);
		void SetColliderScale(const glm::vec2& scale);
		void SetColliderPosition(const glm::vec2& pos);
		void SetZOrder(float pos);

		void UnloadGraphics() override;
		void ReloadGraphics() override;

		void SetOnMouseEndCollideScript(Ref<ScriptObject> obj);
		void SetOnMouseStartCollideScript(Ref<ScriptObject> obj);


		inline void SetOnMouseStartCollide(const std::function<void(float, float)> f) { m_onCollide = f; };
		inline void SetOnMouseEndCollide(const std::function<void(float, float)> f) { m_endCollide = f; };
		inline void SetOnEvent(const std::function<void(const Event&)> f) { m_onEvent = f; }

		inline void RemoveOnMouseCollideFunction() { m_onCollide = nullptr; }
		inline void RemoveEndMouseCollideFunction() { m_endCollide = nullptr; }


		void OnAttached(Ref<Entity> entity) override;
		void DeAttached(Ref<Entity> entity) override;

	protected:
		void OnBegin() override;
		void OnEnd() override;
		void OnUpdate(Timestep timestep) override;
		ShapeID m_ids[2] = { 0,0};
		static Ref<BatchRenderer> s_ShapeFactory;
		glm::vec3 m_imagePosition, m_colliderPosition;
		float m_imageRotation, m_colliderRotation;
		glm::vec2 m_imageScale, m_colliderScale;
		glm::vec4 m_color;
		Ref<Collider2D> m_collider;
		Ref<Texture2D> m_texture = nullptr;
		Ref<Shader> m_Shader = nullptr;
		Ref<SubTexture2D> m_subTexture = nullptr;
		glm::vec2 m_textureScale;
		glm::vec2 m_worldPosition;
		glm::vec2 m_worldScale;
		float m_worldRotation;
		std::function<void(float, float)> m_onCollide;
		std::function<void(float, float)> m_endCollide;
		std::function<void(const Event&)> m_onEvent;
		int m_debug = 1;
		bool b_debug = false;

	};


}
