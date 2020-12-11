#pragma once

#include "Public/Core/Application/Component.h"


namespace GEngine {

	class BatchRenderer;
	class SubTexture2D;
	class Texture2D;
	class Shader;
	struct ScriptVector2;
	struct ScriptVector3;
	struct ScriptVector4;

	class SpriteComponent : public Component {

	public:
		SpriteComponent(Ref<Shader> shader = nullptr, const std::function<void()>& func = nullptr, const std::string& pipeline = "");
		virtual ~SpriteComponent();

		const ShapeID CreateQuad(const Vector3f& _pos, const float rot = 0, const Vector3f& scale = { 1,1,1 }, const Vector4f& _color = { 1,1,1,1.f },
			const Ref<Texture2D> texture = nullptr, const Vector2f& textureScale = Vector2f(1,1));




		ShapeID CreateQuadScript(const Ref<ScriptVector3>& _pos, const float rot, const Ref<ScriptVector3>& scale, const Ref<ScriptVector4>& _color, const Ref<Texture2D>& texture = nullptr,
			const Vector2f& textureScale = Vector2f(1, 1));


		ShapeID CreateSubTexturedQuad(const Vector3f& _pos,const float rot,const Vector3f& scale, const Vector4f& _color, const Ref<SubTexture2D>& texture, const Vector2f& textureScale = Vector2f(1, 1));

		ShapeID CreateSubTexturedQuadScript(const Ref<ScriptVector3>& _pos, const float rot, const Ref<ScriptVector3>& scale, const Ref<ScriptVector4>& _color,
			const Ref<SubTexture2D>& texture, const Vector2f& textureScale = Vector2f(1, 1));


		void SetSubTexture(const ShapeID id,const Ref<SubTexture2D>& texture);
		void SetPosition(const ShapeID id, const Vector2f& position);
		void SetPositionScript(const ShapeID id, const Ref<ScriptVector2>& position);
		void SetScaleScript(const ShapeID id, const Ref<ScriptVector2>& scale);
		void SetZOrder(const ShapeID id, const float zOrder);
		void SetQuadColor(const ShapeID id, const Vector4f& color);
		void SetQuadColorScript(const ShapeID id, const Ref<ScriptVector4>& color);
		void SetRotation(const ShapeID id, const float rotation);

		void SetTextureScale(const ShapeID id, const Vector2f& scale);

		virtual void OnAttached(Ref<Entity> entity) override;
		virtual void DeAttached(Ref<Entity> entity) override;

		virtual void UnloadGraphics() override;
		virtual void ReloadGraphics() override;

		void RemoveQuad(const ShapeID id);
		void ClearQuads();

		Vector2f GetQuadScale(const ShapeID& id);

		virtual void OnBegin() override;
		virtual void OnEnd() override;


		

	private:
		Ref<BatchRenderer> m_shapeFactory;
		static Ref<BatchRenderer> s_ShapeFactory;
		Ref<Shader> m_Shader;
		std::vector<ShapeID> m_ids;


	};




}