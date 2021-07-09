#pragma once

#include "Public/Core/Application/Components/Scripts/NativeScript.h"


namespace GEngine {

	class BatchRenderer;
	class SubTexture2D;
	class Texture2D;
	class Shader;
	struct ScriptVector2;
	struct ScriptVector3;
	struct ScriptVector4;

	class GE_API SpriteComponent : public NativeScript {

	public:
		SpriteComponent(Entity* e, Ref<Shader> shader = nullptr, const std::function<void()>& func = nullptr, const std::string& pipeline = "");
		virtual ~SpriteComponent();

		const ShapeID CreateQuad(const Vector3f& _pos, const float rot = 0, const Vector3f& scale = { 1,1,1 }, const Vector4f& _color = { 1,1,1,1 },
			const Ref<Texture2D> texture = nullptr, const Vector2f& textureScale = Vector2f(1,1));

		ShapeID CreateSubTexturedQuad(const Vector3f& _pos,const float rot,const Vector3f& scale, const Vector4f& _color, const Ref<SubTexture2D>& texture, const Vector2f& textureScale = Vector2f(1, 1));


		void SetSubTexture(const ShapeID id,const Ref<SubTexture2D>& texture);
		void SetPosition(const ShapeID id, const Vector2f& position);
		void SetZOrder(const ShapeID id, const float zOrder);
		void SetQuadColor(const ShapeID id, const Vector4f& color);
		void SetRotation(const ShapeID id, const float rotation);
		void SetQuadScale(const ShapeID id, const Vector2f& scale);

		void SetSafeParams(const ShapeID& id, const Vector2f& pos, const float& rot, const Vector2f& scale, const Vector4f& color);

		void SetTextureScale(const ShapeID id, const Vector2f& scale);

		virtual void UnloadGraphics() override;
		virtual void ReloadGraphics() override;

		void RemoveQuad(const ShapeID id);
		void ClearQuads();

		Vector2f GetQuadScale(const ShapeID& id);

		virtual void OnBegin() override;
		virtual void OnEnd() override;

		inline std::vector<ShapeID> GetQuads() { return m_ids; };
		inline Ref<BatchRenderer> GetBatchRenderer() const { return m_shapeFactory; }


		

	private:
		Ref<BatchRenderer> m_shapeFactory;
		static Ref<BatchRenderer> s_ShapeFactory;
		Ref<Shader> m_Shader;
		std::vector<ShapeID> m_ids;


	};




}