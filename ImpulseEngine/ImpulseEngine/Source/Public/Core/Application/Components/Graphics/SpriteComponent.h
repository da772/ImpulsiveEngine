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
		SpriteComponent();
		virtual ~SpriteComponent();

		const ShapeID CreateQuad(const Vector3& _pos, const float rot = 0, const Vector3& scale = { 1,1,1 }, const Vector4& _color = { 1,1,1,1.f },
			const Ref<Texture2D> texture = nullptr, const glm::vec2& textureScale = glm::vec2(1,1));




		ShapeID CreateQuadScript(const Ref<ScriptVector3>& _pos, const float rot, const Ref<ScriptVector3>& scale, const Ref<ScriptVector4>& _color, const Ref<Texture2D>& texture = nullptr,
			const glm::vec2& textureScale = glm::vec2(1, 1));


		ShapeID CreateSubTexturedQuad(const Vector3& _pos,const float rot,const Vector3& scale, const Vector4& _color, const Ref<SubTexture2D>& texture, const glm::vec2& textureScale = glm::vec2(1, 1));

		ShapeID CreateSubTexturedQuadScript(const Ref<ScriptVector3>& _pos, const float rot, const Ref<ScriptVector3>& scale, const Ref<ScriptVector4>& _color,
			const Ref<SubTexture2D>& texture, const glm::vec2& textureScale = glm::vec2(1, 1));


		void SetSubTexture(const ShapeID id,const Ref<SubTexture2D>& texture);
		void SetPosition(const ShapeID id, const glm::vec2& position);
		void SetPositionScript(const ShapeID id, const Ref<ScriptVector2>& position);
		void SetScaleScript(const ShapeID id, const Ref<ScriptVector2>& scale);
		void SetZOrder(const ShapeID id, const float zOrder);
		void SetQuadColor(const ShapeID id, const glm::vec4& color);
		void SetQuadColorScript(const ShapeID id, const Ref<ScriptVector4>& color);
		void SetRotation(const ShapeID id, const float rotation);

		virtual void OnAttached(Ref<Entity> entity) override;
		virtual void DeAttached(Ref<Entity> entity) override;

		virtual void UnloadGraphics() override;
		virtual void ReloadGraphics() override;

		void RemoveQuad(const ShapeID id);
		void ClearQuads();

		virtual void OnBegin() override;
		virtual void OnEnd() override;

	private:
		static Ref<BatchRenderer> s_ShapeFactory;
		Ref<Shader> m_Shader;
		std::vector<ShapeID> m_ids;


	};




}