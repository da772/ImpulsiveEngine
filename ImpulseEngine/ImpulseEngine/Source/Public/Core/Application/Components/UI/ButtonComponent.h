#pragma once

#include "Public/Core/Application/Component.h"

namespace GEngine {

	class BatchRenderer;
	class Texture2D;
	class SubTexture2D;
	class Collider2D;
	class Shader;
	class ScriptObject;

	class ButtonComponent : public Component {
	public:

		ButtonComponent(glm::vec3 pos, float rot, glm::vec2 scale, glm::vec4 color,float textureScale = 1 );
		virtual ~ButtonComponent();

		void SetImageSubTexture(Ref<SubTexture2D> texture);
		void SetImageTexture(Ref<Texture2D> texture);
		void SetImageColor(const glm::vec4& color);
		void SetImageRotation(float rot);
		void SetImagePosition(glm::vec2 pos);
		void SetImageScale(glm::vec2 scale);
		void SetColliderScale(glm::vec2 scale);
		void SetColliderPosition(glm::vec2 pos);
		void SetZOrder(float pos);

		void UnloadGraphics() override;
		void ReloadGraphics() override;

		void SetOnMouseEndCollideScript(Ref<ScriptObject> obj);
		void SetOnMouseStartCollideScript(Ref<ScriptObject> obj);

		inline void SetOnMouseStartCollide(std::function<void(float, float)> f) { m_onCollide = f; };
		inline void SetOnMouseEndCollide(std::function<void(float, float)> f) { m_endCollide = f; };

		inline void RemoveOnMouseCollideFunction() { m_onCollide = nullptr; }
		inline void RemoveEndMouseCollideFunction() { m_endCollide = nullptr; }


		void OnAttached(Ref<Entity> entity) override;
		void DeAttached(Ref<Entity> entity) override;

	protected:
		void OnBegin() override;
		void OnEnd() override;
		void OnUpdate(Timestep timestep) override;
		long m_ids[2];
		static Ref<BatchRenderer> s_ShapeFactory;
		glm::vec3 m_imagePosition, m_colliderPosition;
		float m_imageRotation, m_colliderRotation;
		glm::vec2 m_imageScale, m_colliderScale;
		glm::vec4 m_color;
		Ref<Collider2D> m_collider;
		Ref<Texture2D> m_texture = nullptr;
		Ref<Shader> m_Shader = nullptr;
		Ref<SubTexture2D> m_subTexture = nullptr;
		float m_textureScale;
		glm::vec2 m_worldPosition;
		glm::vec2 m_worldScale;
		float m_worldRotation;
		std::function<void(float, float)> m_onCollide;
		std::function<void(float, float)> m_endCollide;
		int m_debug = 1;
		bool b_debug = true;

	};


}
