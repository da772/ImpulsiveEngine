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

	struct SpriteObject {
		Vector3f pos;
		float rot;
		Vector3f scale;
		Vector4f color;
		Ref<Texture2D> texture = nullptr;
		Ref<SubTexture2D> subtexture = nullptr;
		Vector2f textureScale = { 1.f,1.f };
	};

	class GE_API SpriteComponent : public Component {

	public:
		SpriteComponent(Entity* e, Ref<Shader> shader = nullptr, const std::function<void()>& func = nullptr, const std::string& pipeline = "");
		virtual ~SpriteComponent();

		const ShapeID CreateQuad(const Vector3f& _pos, const float rot = 0, const Vector3f& scale = { 1,1,1 }, const Vector4f& _color = { 1,1,1,1 },
			const Ref<Texture2D> texture = nullptr, const Vector2f& textureScale = Vector2f(1,1));

		ShapeID CreateSubTexturedQuad(const Vector3f& _pos,const float rot,const Vector3f& scale, const Vector4f& _color, const Ref<SubTexture2D>& texture, const Vector2f& textureScale = Vector2f(1, 1));


		void SetSubTexture(const ShapeID id,const Ref<SubTexture2D>& texture);
		void SetPosition(const ShapeID id, const Vector2f& position);
		void SetZOrder(const ShapeID id, const float zOrder);
		void SetColor(const ShapeID id, const Vector4f& color);
		void SetRotation(const ShapeID id, const float rotation);
		void SetScale(const ShapeID id, const Vector2f& scale);

		void SetParams(const ShapeID& id, const Vector2f& pos, const float& rot, const Vector2f& scale, const Vector4f& color);

		void SetTextureScale(const ShapeID id, const Vector2f& scale);

		virtual void UnloadGraphics() override;
		virtual void ReloadGraphics() override;

		void RemoveQuad(const ShapeID id);
		void ClearQuads();

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
		const SpriteObject& GetObject(const ShapeID& id);


		virtual void OnBegin() override;
		virtual void OnEnd() override;

		inline const std::unordered_map<ShapeID, SpriteObject>&  GetObjects() { return m_objects; };
		inline Ref<BatchRenderer> GetBatchRenderer() const { return m_shapeFactory; }


		

	private:
		Ref<BatchRenderer> m_shapeFactory;
		static Ref<BatchRenderer> s_ShapeFactory;
		Ref<Shader> m_Shader;
		std::unordered_map<ShapeID, SpriteObject> m_objects;


	};




}