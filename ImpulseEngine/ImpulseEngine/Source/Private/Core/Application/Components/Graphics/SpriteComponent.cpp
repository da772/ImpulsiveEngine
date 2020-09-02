#include "gepch.h"

#include "Public/Core/Application/Components/Graphics/SpriteComponent.h"
#include "Public/Core/Renderer/Graphics/Quad.h"
#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/RenderCommand.h"

#include "Public/Core/Renderer/Graphics/BatchRenderer.h"
#include "Public/Core/Scripting/ScriptObject.h"
#include "Public/Core/Application/Components/TransformComponent.h"
#include "Public/Core/Application/Entity.h"

namespace GEngine {

	Ref < BatchRenderer >SpriteComponent::s_ShapeFactory = nullptr;
	

	void SpriteComponent::OnAttached(Ref<Entity> entity)
	{
		entity->AddTransformCallback(std::static_pointer_cast<Component>(self.lock()), [this](Ref<Transform> transform, TransformData transData) {
			if (IsInitialized()) {
				for (long id : m_ids) {
					Vector3 pos = s_ShapeFactory->GetShapePosition(id);
					Vector3 nPos = pos - transData.position + transform->GetPosition();
					if (pos != nPos)
						s_ShapeFactory->SetPosition(id, nPos);
					float rot = s_ShapeFactory->GetShapeRotation(id);
					float nRot = rot - transData.rotation.z + transform->GetRotation().z;
					if (rot != nRot)
						s_ShapeFactory->SetRotation(id, nRot);
					Vector2 _scale = s_ShapeFactory->GetShapeScale(id);
					Vector3 scale(_scale.x, _scale.y, 1);
					Vector3 nScale = scale - transData.scale.z + transform->GetScale().z;
					if (scale != nScale)
						s_ShapeFactory->SetScale(id, {nScale.x, nScale.y});
				}
			}
			});
	}

	void SpriteComponent::DeAttached(Ref<Entity> entity)
	{
		entity->RemoveTransformCallback(std::static_pointer_cast<Component>(self.lock()));
	}

	SpriteComponent::SpriteComponent()
	{
		
		if (s_ShapeFactory == nullptr) {
			std::string path = std::string("Content/shaders/TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "Batch.glsl";
			m_Shader = Ref<Shader>(Shader::Create(path));
			SpriteComponent::s_ShapeFactory = Ref<BatchRenderer>(new BatchRenderer(ERenderType::GAME, Ref<Quad>(new Quad()),
				5000, m_Shader));
		}	
	}

	void SpriteComponent::ReloadGraphics() {
		if (s_ShapeFactory) {
			s_ShapeFactory->ReloadGraphics();
		}
	}

	void SpriteComponent::UnloadGraphics() {
		if (s_ShapeFactory) {
			s_ShapeFactory->UnloadGraphics();
		}
	}

	SpriteComponent::~SpriteComponent()
	{

		
	}

	long SpriteComponent::CreateQuad(const Vector3& _pos, const float rotation, const Vector3& scale, const Vector4& _color,
		const Ref<Texture2D> texture, const glm::vec2& textureScale)
	{
		Ref<Entity> e = entity.lock();
		long id = s_ShapeFactory->AddShape(_pos+e->GetEntityPosition(), rotation+e->GetEntityRotation().z, scale* e->GetEntityScale(), _color, texture, textureScale);
		m_ids.push_back(id);
		return id;
	}

	long SpriteComponent::CreateQuadScript(const Ref<ScriptVector3>& _pos, const float rot, const Ref<ScriptVector3>& scale, const Ref<ScriptVector4>& _color,
		const Ref<Texture2D>& texture, const glm::vec2& textureScale)
	{
		return CreateQuad(_pos->GetGlm(), rot, scale->GetGlm(), _color->GetGlm(), texture, textureScale);
	}

	long SpriteComponent::CreateSubTexturedQuadScript(const Ref<ScriptVector3>& _pos, const float rot, const Ref<ScriptVector3>& scale,
		const Ref<ScriptVector4>& _color,const Ref<SubTexture2D>& texture, const glm::vec2& textureScale)
	{
		return CreateSubTexturedQuad(_pos->GetGlm(), rot , scale->GetGlm(), _color->GetGlm(), texture, textureScale);
	}

	void SpriteComponent::SetSubTexture(const long id, const Ref<SubTexture2D>& texture)
	{
		s_ShapeFactory->SetSubTexture(id, texture);
	}

	void SpriteComponent::SetPosition(const long id, const glm::vec2& position)
	{
		s_ShapeFactory->SetPosition(id, position + glm::vec2( entity.lock()->GetEntityPosition().x, entity.lock()->GetEntityPosition().y ) );
	}

	void SpriteComponent::SetPositionScript(const long id, const Ref<ScriptVector2>& position)
	{
		SetPosition(id, position->GetGlm());
	}

	void SpriteComponent::SetScaleScript(const long id,const Ref<ScriptVector2>& scale)
	{
		s_ShapeFactory->SetScale(id, scale->GetGlm());
	}

	void SpriteComponent::SetZOrder(const long id, const float zOrder)
	{
		s_ShapeFactory->SetZOrder(id, zOrder+entity.lock()->GetEntityPosition().z);
	}

	void SpriteComponent::SetQuadColor(const long id, const glm::vec4& color)
	{
		s_ShapeFactory->SetColor(id, color);
	}

	void SpriteComponent::SetQuadColorScript(const long id, const Ref<ScriptVector4>& color)
	{
		SetQuadColor(id, color->GetGlm());
	}

	void SpriteComponent::SetRotation(const long id, const float rotation)
	{
		s_ShapeFactory->SetRotation(id, rotation);
	}

	long SpriteComponent::CreateSubTexturedQuad(const Vector3& _pos, const float rot /*= 0*/, const Vector3& scale /*= { 1,1,1 }*/,
		const Vector4& _color /*= { 1,1,1,1.f }*/, const Ref<SubTexture2D>& texture /*= nullptr*/, const glm::vec2& textureScale/*= 1*/)
	{
		Ref<Entity> e = entity.lock();
		long id = s_ShapeFactory->AddShape(_pos + e->GetEntityPosition(), rot + e->GetEntityRotation().z, scale * e->GetEntityScale(), _color, texture, textureScale);
		m_ids.push_back(id);
		return id;
	}

	void SpriteComponent::RemoveQuad(const long id)
	{
		s_ShapeFactory->RemoveShape(id);
		m_ids.erase(std::find(m_ids.begin(), m_ids.end(), id));
	}

	void SpriteComponent::ClearQuads()
	{
		for (long id : m_ids)
			s_ShapeFactory->RemoveShape(id);
		m_ids.clear();
	}

	void SpriteComponent::OnBegin()
	{
	
		
	}

	void SpriteComponent::OnEnd()
	{
		ClearQuads();
	}


}
