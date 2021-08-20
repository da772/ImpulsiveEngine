#include "gepch.h"

#include "Public/Core/Application/Components/Graphics/SpriteComponent.h"
#include "Public/Core/Renderer/Graphics/Quad.h"
#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/RenderCommand.h"

#include "Public/Core/Renderer/Graphics/BatchRenderer.h"
#include "Public/Core/Application/Components/TransformComponent.h"
#include "Public/Core/Application/Entity.h"


#include "Public/Core/Application/Application.h"

#include "Public/Core/Renderer/Graphics/Texture.h"

namespace GEngine {

	Ref < BatchRenderer >SpriteComponent::s_ShapeFactory = nullptr;
	

	SpriteComponent::SpriteComponent(Entity* e, Ref<Shader> shader, const std::function<void()>& shaderFunc, const std::string& pipelineId) : Component(e)
	{
		go_tag = "Sprite Component";

		if (s_ShapeFactory == nullptr) {
			std::string path = std::string("EngineContent/shaders/TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "Batch.glsl";
			m_Shader = shader != nullptr ? shader : Ref<Shader>(Shader::Create(path));
			SpriteComponent::s_ShapeFactory = Ref<BatchRenderer>(new BatchRenderer(ERenderType::GAME, Ref<Quad>(new Quad()), 5000, m_Shader, pipelineId.size() > 0 ? pipelineId.c_str() : nullptr));
			s_ShapeFactory->SetRenderType(ERenderType::GAME);
		}

		m_shapeFactory = s_ShapeFactory;
		m_entity->GetTransform()->AddTransformCallback(GetHash(), [this](Transform* transform, TransformData transData) {
			for (const auto& p : m_objects) {
				ShapeID id = p.first;
				if (transform->GetWorldPosition() != transData.GetWorldPosition())
					SetPosition(id, m_objects[id].pos.xy());
				if (transform->GetWorldPosition().z != transData.GetWorldPosition().z)
					SetZOrder(id, m_objects[id].pos.z);
				if (transform->GetWorldRotation() != transData.GetWorldRotation())
					SetRotation(id, m_objects[id].rot);
				if (transform->GetWorldScale() != transData.GetWorldScale())
					SetScale(id, m_objects[id].scale);
			}

			});
	}


	void SpriteComponent::ReloadGraphics() {
		if (m_shapeFactory) {
			m_shapeFactory->ReloadGraphics();
		}
	}

	void SpriteComponent::UnloadGraphics() {
		if (m_shapeFactory) {
			m_shapeFactory->UnloadGraphics();
		}
	}

	SpriteComponent::~SpriteComponent()
	{
		m_entity->GetTransform()->RemoveTransformCallback(GetHash());
		ClearQuads();
	}

	const ShapeID SpriteComponent::CreateQuad(const Vector3f& _pos, const float rotation, const Vector3f& scale, const Vector4f& _color,
		const Ref<Texture2D> texture, const Vector2f& textureScale)
	{
		Vector3f _scale = scale * m_entity->GetTransform()->GetWorldScale();

		ShapeID id = m_shapeFactory->AddShape(_pos, rotation , scale.xy(), _color, texture, textureScale);
		m_objects[id] = { _pos, rotation, scale, _color, texture, nullptr, textureScale };
		return id;
	}


	void SpriteComponent::SetSubTexture(const ShapeID id, const Ref<SubTexture2D>& texture)
	{
		m_objects[id].subtexture = texture;
		m_objects[id].texture = nullptr;
		m_shapeFactory->SetSubTexture(id, texture);
	}

	void SpriteComponent::SetPosition(const ShapeID id, const Vector2f& position)
	{
		m_objects[id].pos = { position, m_objects[id].pos.z };
		m_shapeFactory->SetPosition(id, position + m_entity->GetTransform()->GetWorldPosition().xy()  );
	}

	void SpriteComponent::SetScale(const ShapeID id, const Vector2f& scale)
	{
		m_objects[id].scale = { scale, m_objects[id].scale.z };
		m_shapeFactory->SetScale(id, scale * m_entity->GetTransform()->GetWorldScale().xy());
	}

	void SpriteComponent::SetParams(const ShapeID& id, const Vector2f& pos, const float& rot, const Vector2f& scale, const Vector4f& color)
	{
		m_objects[id].pos = { pos, m_objects[id].pos.z };
		m_objects[id].scale = { scale, m_objects[id].scale.z };
		m_objects[id].rot = rot;
		m_objects[id].color = color;
		m_shapeFactory->SetSafeParams(id, pos + m_entity->GetTransform()->GetWorldPosition().xy(), rot+m_entity->GetTransform()->GetWorldRotation().z, scale * m_entity->GetTransform()->GetWorldScale().xy(), color);
	}

	void SpriteComponent::SetZOrder(const ShapeID id, const float zOrder)
	{
		m_objects[id].pos.z = zOrder;
		m_shapeFactory->SetZOrder(id, zOrder+ m_entity->GetTransform()->GetWorldPosition().z);
	}

	void SpriteComponent::SetColor(const ShapeID id, const Vector4f& color)
	{
		m_objects[id].color = color;
		m_shapeFactory->SetColor(id, color);
	}

	void SpriteComponent::SetRotation(const ShapeID id, const float rotation)
	{
		m_objects[id].rot = rotation;
		m_shapeFactory->SetRotation(id, rotation + m_entity->GetTransform()->GetWorldRotation().z);
	}


	void SpriteComponent::SetTextureScale(const ShapeID id, const Vector2f& scale)
	{
		m_objects[id].textureScale = scale;
		m_shapeFactory->SetTextureScale(id, scale);
	}

	ShapeID SpriteComponent::CreateSubTexturedQuad(const Vector3f& _pos, const float rot /*= 0*/, const Vector3f& scale /*= { 1,1,1 }*/,
		const Vector4f& _color /*= { 1,1,1,1.f }*/, const Ref<SubTexture2D>& texture /*= nullptr*/, const Vector2f& textureScale/*= 1*/)
	{
		ShapeID id = m_shapeFactory->AddShape(_pos + m_entity->GetTransform()->GetWorldPosition(), rot + m_entity->GetTransform()->GetWorldRotation().z, (scale * m_entity->GetTransform()->GetWorldScale()).xy(), _color, texture, textureScale);
		m_objects[id] = { _pos, rot, scale, _color, nullptr, texture, textureScale };
		return id;
	}

	void SpriteComponent::RemoveQuad(const ShapeID id)
	{
		m_shapeFactory->RemoveShape(id);
		m_objects.erase(id);
	}

	void SpriteComponent::ClearQuads()
	{
		for (const auto& p : m_objects) {
			m_shapeFactory->RemoveShape(p.first);
		}
		m_objects.clear();
	}

	GEngine::Vector2f SpriteComponent::GetLocalScale(const ShapeID& id)
	{
		return m_objects[id].scale;
	}

	GEngine::Vector3f SpriteComponent::GetLocalPosition(const ShapeID& id)
	{
		return m_objects[id].pos;
	}

	float SpriteComponent::GetLocalRotation(const ShapeID& id)
	{
		return m_objects[id].rot;
	}

	GEngine::Vector2f SpriteComponent::GetWorldScale(const ShapeID& id)
	{
		return m_objects[id].scale * m_entity->GetTransform()->GetWorldScale();
	}

	GEngine::Vector3f SpriteComponent::GetWorldPosition(const ShapeID& id)
	{
		return m_objects[id].pos + m_entity->GetTransform()->GetWorldPosition();
	}

	float SpriteComponent::GetWorldRotation(const ShapeID& id)
	{
		return m_objects[id].rot + m_entity->GetTransform()->GetWorldRotation().z;
	}

	GEngine::Vector4f SpriteComponent::GetColor(const ShapeID& id)
	{
		return m_objects[id].color;
	}

	GEngine::Ref<GEngine::Texture2D> SpriteComponent::GetTexture(const ShapeID& id)
	{
		return m_objects[id].texture;
	}

	GEngine::Ref<GEngine::SubTexture2D> SpriteComponent::GetSubTexture(const ShapeID& id)
	{
		return m_objects[id].subtexture;
	}

	GEngine::Vector2f SpriteComponent::GetTextureScale(const ShapeID& id)
	{
		return m_objects[id].textureScale;
	}

	const GEngine::SpriteObject& SpriteComponent::GetObject(const ShapeID& id)
	{
		return m_objects[id];
	}


	void SpriteComponent::OnBegin()
	{
	
		
	}

	void SpriteComponent::OnEnd()
	{
	
	}


}
