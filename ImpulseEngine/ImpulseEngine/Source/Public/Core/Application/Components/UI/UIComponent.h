#pragma once

#include "Public/Core/Application/Components/Scripts/NativeScript.h"


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

	class GE_API UIComponent : public NativeScript {

	public:
		UIComponent(Entity* e);
		UIComponent(Entity* e, Ref<Shader> shader);
		virtual ~UIComponent();
		const ShapeID CreateQuad(const  Vector3f& _pos, const float rot = 0, const Vector3f& scale = { 1,1,1 }, const Vector4f& _color = { 1,1,1,1.f }, Ref<Texture2D> texture = nullptr, bool aspectRatio = true, const Vector2f& textureScale = Vector2f(1, 1), const float alphaChannel = 4);

		

		const ShapeID CreateSubTexturedQuad(const Vector3f& _pos, const float rot, const Vector3f& scale, const Vector4f& _color, Ref<SubTexture2D> texture, const Vector2f& textureScale = Vector2f(1,1), const float alphaChannel = 4);
		const std::string CreateText(const std::string& string, Ref<Font> font, const Vector3f& pos, const Vector3f& scale, const Vector4f& color);
		const std::string CreateText_u32(uint32_t* string, int len, Ref<Font> font, const Vector3f& pos, const Vector3f& scale, const Vector4f& color);
		const void AddText(const std::string& id, const std::string& text, const Vector3f& pos, const Vector3f& scale, const Vector4f& color);
		void RemoveText(const std::string& id);
		void SetTextColor(const std::string& id, const Vector4f& color, int index, int count = 1);

		uint32_t GetTextSize(const std::string& id);

		void SetPosition(const ShapeID id, const Vector2f& position);
		void SetZOrder(const ShapeID id, const float zOrder);
		void SetScale(const ShapeID id, const Vector3f& scale);

        
        Vector2f GetQuadScale(const ShapeID& id);

		void SetColor(const ShapeID id, const Vector4f& color);
		Ref<Texture2D> GetTexture(const ShapeID id);

		void Remove(const ShapeID id);
		void Remove(const std::string& hash);
		void ClearQuads();

		virtual void OnUpdate(Timestep ts) override;

		virtual void OnBegin() override;
		virtual void OnEnd() override;

		virtual void UnloadGraphics() override;
		virtual void ReloadGraphics() override;

		static Ref<BatchRenderer> s_ShapeFactory;
	private:
		

		uint32_t m_textCounter = 0;

		Ref<Shader> m_Shader;
		std::unordered_map<std::string, UI_TextInfo> m_text;
		std::vector<ShapeID> m_ids;


	};







}
