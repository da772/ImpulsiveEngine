#pragma once

#include "Public/Core/Application/Component.h"


namespace GEngine {

	class Shader;
	class Font;
	class BatchRenderer;
	class Texture2D;
	class SubTexture2D;
	struct ScriptVector2;
	struct ScriptVector3;
	struct ScriptVector4;

	class UIComponent : public Component {

	public:
		UIComponent();
		UIComponent(Ref<Shader> shader);
		virtual ~UIComponent();
		long CreateQuad(Vector3 _pos, float rot = 0, Vector3 scale = { 1,1,1 }, Vector4 _color = { 1,1,1,1.f }, Ref<Texture2D> texture = nullptr, float textureScale = 1, float alphaChannel = 4);

		long CreateQuadScript(Ref<ScriptVector3> _pos, float rot, Ref<ScriptVector3> scale, Ref<ScriptVector4> _color, Ref<Texture2D> texture );
		long CreateSubTexturedQuadScript(Ref<ScriptVector3> _pos, float rot, Ref<ScriptVector3> scale, Ref<ScriptVector4> _color, Ref<SubTexture2D> texture, float textureScale);

		long CreateSubTexturedQuad(Vector3 _pos, float rot, Vector3 scale, Vector4 _color, Ref<SubTexture2D> texture, float textureScale = 1.f, float alphaChannel = 4);

		long CreateText(std::string string, Ref<Font> font, Vector3 pos, Vector3 scale, Vector4 color);
		long CreateTextScript(std::string string, Ref<Font> font, Ref<ScriptVector3> pos, Ref<ScriptVector3> scale, Ref<ScriptVector4> color);
		void RemoveText(long id);

		void SetPosition(long id, glm::vec2 position);
		void SetPositionScript(long id, Ref<ScriptVector2> position);
		void SetZOrder(long id, float zOrder);


		void SetColor(long id, glm::vec4 color);

		void Remove(long id);
		void ClearQuads();

		virtual void OnUpdate(Timestep ts) override;

		virtual void OnBegin() override;
		virtual void OnEnd() override;
		virtual void OnAttached(Ref<Entity> entity) override;
		virtual void DeAttached(Ref<Entity> entity) override;

		virtual void UnloadGraphics() override;
		virtual void ReloadGraphics() override;

	private:
		static void RemoveQuads(u32 id);
		static Ref<BatchRenderer> s_ShapeFactory;

		uint32_t m_textCounter = 0;

		Ref<Shader> m_Shader;
		std::unordered_map<long, std::vector<long>> m_text;
		std::vector<long> m_ids;


	};







}
