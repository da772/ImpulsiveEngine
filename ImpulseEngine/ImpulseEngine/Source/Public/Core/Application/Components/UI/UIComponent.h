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
	struct StringInfo;


	struct UI_TextInfo {
		Ref<StringInfo> info;
		std::vector<ShapeID> shapes;
		Ref<Font> font;
	};

	class UIComponent : public Component {

	public:
		UIComponent();
		UIComponent(Ref<Shader> shader);
		virtual ~UIComponent();
		const ShapeID CreateQuad(const  Vector3f& _pos, const float rot = 0, const Vector3f& scale = { 1,1,1 }, const Vector4f& _color = { 1,1,1,1.f }, Ref<Texture2D> texture = nullptr, bool aspectRatio = true, const Vector2f& textureScale = Vector2f(1, 1), const float alphaChannel = 4);

		const ShapeID CreateQuadScript(Ref<ScriptVector3> _pos, const float rot, Ref<ScriptVector3> scale, Ref<ScriptVector4> _color, Ref<Texture2D> texture );
		

		const ShapeID CreateSubTexturedQuad(const Vector3f& _pos, const float rot, const Vector3f& scale, const Vector4f& _color, Ref<SubTexture2D> texture, const Vector2f& textureScale = Vector2f(1,1), const float alphaChannel = 4);
		const std::string CreateText(const std::string& string, Ref<Font> font, const Vector3f& pos, const Vector3f& scale, const Vector4f& color);
		const void AddText(const std::string& id, const std::string& text, const Vector3f& pos, const Vector3f& scale, const Vector4f& color);
		void RemoveText(const std::string& id);
		void SetTextColor(const std::string& id, const Vector4f& color, int index, int count = 1);

		uint32_t GetTextSize(const std::string& id);

		void SetPosition(const ShapeID id, const Vector2f& position);
		void SetPositionScript(const ShapeID id, Ref<ScriptVector2> position);
		void SetZOrder(const ShapeID id, const float zOrder);

        
        Vector2f GetQuadScale(const ShapeID& id);

		void SetColor(const ShapeID id, const Vector4f& color);

		void Remove(const ShapeID id);
		void Remove(const std::string& hash);
		void ClearQuads();

		virtual void OnUpdate(Timestep ts) override;

		virtual void OnBegin() override;
		virtual void OnEnd() override;
		virtual void OnAttached(Ref<Entity> entity) override;
		virtual void DeAttached(Ref<Entity> entity) override;

		virtual void UnloadGraphics() override;
		virtual void ReloadGraphics() override;

		static Ref<BatchRenderer> s_ShapeFactory;
	private:
		static void RemoveQuads(ShapeID id);
		

		uint32_t m_textCounter = 0;

		Ref<Shader> m_Shader;
		std::unordered_map<std::string, UI_TextInfo> m_text;
		std::vector<ShapeID> m_ids;


	};







}
