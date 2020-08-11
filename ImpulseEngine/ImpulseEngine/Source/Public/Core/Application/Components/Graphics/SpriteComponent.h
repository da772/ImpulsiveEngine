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

		long CreateQuad(Vector3 _pos, float rot = 0, Vector3 scale = { 1,1,1 }, Vector4 _color = { 1,1,1,1.f }, Ref<Texture2D> texture = nullptr, float textureScale = 1);


		long CreateQuadScript(Ref<ScriptVector3> _pos, float rot, Ref<ScriptVector3> scale, Ref<ScriptVector4> _color, Ref<Texture2D> texture = nullptr);


		long CreateSubTexturedQuad(Vector3 _pos, float rot, Vector3 scale, Vector4 _color, Ref<SubTexture2D> texture, float textureScale = 1.f);

		long CreateSubTexturedQuadScript(Ref<ScriptVector3> _pos, float rot, Ref<ScriptVector3> scale, Ref<ScriptVector4> _color, Ref<SubTexture2D> texture, float tscale);


		void SetSubTexture(long id, Ref<SubTexture2D> texture);
		void SetPosition(long id, glm::vec2 position);
		void SetPositionScript(long id, Ref<ScriptVector2> position);
		void SetZOrder(long id, float zOrder);
		void SetQuadColor(long id, glm::vec4 color);
		void SetQuadColorScript(long id, Ref<ScriptVector4> color);
		void SetRotation(long id, float rotation);

		virtual void OnAttached(Ref<Entity> entity) override;
		virtual void DeAttached(Ref<Entity> entity) override;

		virtual void UnloadGraphics() override;
		virtual void ReloadGraphics() override;

		void RemoveQuad(long id);
		void ClearQuads();

		virtual void OnBegin() override;
		virtual void OnEnd() override;

	private:
		static void RemoveQuads(u32 id);
		static Ref<BatchRenderer> s_ShapeFactory;
		Ref<Shader> m_Shader;
		std::vector<int> m_ids;


	};




}