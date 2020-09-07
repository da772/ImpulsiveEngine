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
		const long CreateQuad(const  Vector3& _pos, const float rot = 0, const Vector3& scale = { 1,1,1 }, const Vector4& _color = { 1,1,1,1.f }, Ref<Texture2D> texture = nullptr, const glm::vec2& textureScale = glm::vec2(1, 1), const float alphaChannel = 4);

		const long CreateQuadScript(Ref<ScriptVector3> _pos, const float rot, Ref<ScriptVector3> scale, Ref<ScriptVector4> _color, Ref<Texture2D> texture );
		

		const long CreateSubTexturedQuad(const Vector3& _pos, const float rot, const Vector3& scale, const Vector4& _color, Ref<SubTexture2D> texture, const glm::vec2& textureScale = glm::vec2(1,1), const float alphaChannel = 4);
		const std::string CreateText(const std::string& string, Ref<Font> font, const Vector3& pos, const Vector3& scale, const Vector4& color);
		void RemoveText(const std::string& id);

		void SetPosition(const long id, const glm::vec2& position);
		void SetPositionScript(const long id, Ref<ScriptVector2> position);
		void SetZOrder(const long id, const float zOrder);


		void SetColor(const long id, const glm::vec4& color);

		void Remove(const long id);
		void Remove(const std::string& hash);
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
		std::unordered_map<std::string, std::vector<long>> m_text;
		std::vector<long> m_ids;


	};







}
