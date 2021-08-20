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
		Vector3f pos;
		Vector3f scale;
		std::wstring str;
	};

	struct UIObject {
		Vector3f pos;
		float rot;
		Vector3f scale;
		Vector4f color;
		Ref<Texture2D> texture = nullptr;
		Ref<SubTexture2D> subtexture = nullptr;
		Vector2f textureScale = { 1.f,1.f };
		float alphaChannel = 1.f;
		bool aspectRatio = false;
		bool text = false;
	};

	class GE_API UIComponent : public Component {

	public:
		UIComponent(Entity* e);
		UIComponent(Entity* e, Ref<Shader> shader);
		virtual ~UIComponent();
		const ShapeID CreateQuad(const  Vector3f& _pos, const float rot = 0, const Vector3f& scale = { 1,1,1 }, const Vector4f& _color = { 1,1,1,1.f }, Ref<Texture2D> texture = nullptr, bool aspectRatio = false, const Vector2f& textureScale = Vector2f(1, 1), const float alphaChannel = 4);

		

		const ShapeID CreateSubTexturedQuad(const Vector3f& _pos, const float rot, const Vector3f& scale, const Vector4f& _color, Ref<SubTexture2D> texture, const Vector2f& textureScale = Vector2f(1,1), const float alphaChannel = 4, bool text = false);
		const std::string CreateText(const std::string& string, Ref<Font> font, const Vector3f& pos, const Vector3f& scale, const Vector4f& color);
		const std::string CreateText_u32(uint32_t* string, int len, Ref<Font> font, const Vector3f& pos, const Vector3f& scale, const Vector4f& color);
		const void AddText(const std::string& id, const std::string& text, const Vector4f& color);
		void SetText(const std::string& s, const std::string& text, const Vector4f& color);
		void RemoveText(const std::string& id);
		void SetTextColor(const std::string& id, const Vector4f& color, int index, int count = 1);

		uint32_t GetTextSize(const std::string& id);

		void SetPosition(const std::string& id, const Vector2f position);
		void SetScale(const std::string& id, const Vector2f scale);
		void SetZOrder(const std::string& id, float order);

		void SetPosition(const ShapeID id, const Vector2f& position);
		void SetZOrder(const ShapeID id, const float zOrder);
		void SetRotation(const ShapeID id, const float rot);
		void SetScale(const ShapeID id, const Vector3f& scale);
		void SetColor(const ShapeID id, const Vector4f& color);
        
		Vector2f GetLocalScale(const ShapeID& id);
		Vector3f GetLocalPosition(const ShapeID& id);
		float GetLocalRotation(const ShapeID& id);
		Vector2f GetWorldScale(const ShapeID& id);
		Vector3f GetWorldPosition(const ShapeID& id);
		float GetWorldRotation(const ShapeID& id);
		Vector4f GetColor(const ShapeID& id);
		Ref<Texture2D> GetTexture(const ShapeID& id);
		Ref<SubTexture2D> GetSubTexture(const ShapeID& id);
		Vector2f GetTextureScale(const ShapeID& id);
		const UIObject& GetObject(const ShapeID& id);
		const UI_TextInfo& GetText(const std::string& id);
		const std::unordered_map<ShapeID, UIObject>& GetObjects() const;
		const std::unordered_map<std::string, UI_TextInfo>& GetTexts() const;
		Ref<BatchRenderer> GetBatchRenderer() const;

		float GetTextWidth(const std::string& id);
		float GetTextHeight(const std::string& id);

		void Remove(const ShapeID id);
		void Remove(const std::string& hash);
		void Clear();

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
		std::unordered_map<ShapeID, UIObject> m_objects;


	};







}
