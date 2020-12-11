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

		ButtonComponent(const Vector3f& pos, const float rot, const Vector2f& scale, const Vector4f& color, Vector2f textureScale = { 1,1 });
		virtual ~ButtonComponent();

		void SetImageSubTexture(Ref<SubTexture2D> texture);
		void SetImageTexture(Ref<Texture2D> texture);
		void SetImageColor(const Vector4f& color);
		void SetImageRotation(const float rot);
		void SetImagePosition(const Vector2f& pos);
		void SetImageScale(const Vector2f& scale);
		void SetColliderScale(const Vector2f& scale);
		void SetColliderPosition(const Vector2f& pos);
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
		Vector3f m_imagePosition, m_colliderPosition;
		float m_imageRotation, m_colliderRotation;
		Vector2f m_imageScale, m_colliderScale;
		Vector4f m_color;
		Ref<Collider2D> m_collider;
		Ref<Texture2D> m_texture = nullptr;
		Ref<Shader> m_Shader = nullptr;
		Ref<SubTexture2D> m_subTexture = nullptr;
		Vector2f m_textureScale;
		Vector2f m_worldPosition;
		Vector2f m_worldScale;
		float m_worldRotation;
		std::function<void(float, float)> m_onCollide;
		std::function<void(float, float)> m_endCollide;
		std::function<void(const Event&)> m_onEvent;
		int m_debug = 1;
		bool b_debug = false;

	};


}
