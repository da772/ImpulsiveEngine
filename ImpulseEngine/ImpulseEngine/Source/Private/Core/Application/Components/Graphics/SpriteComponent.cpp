#include "gepch.h"

#include "Public/Core/Application/Components/Graphics/SpriteComponent.h"
#include "Public/Core/Renderer/Graphics/Quad.h"
#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/RenderCommand.h"

#include "Public/Core/Renderer/Graphics/BatchRenderer.h"
#include "Public/Core/Scripting/ScriptObject.h"
#include "Public/Core/Application/Components/TransformComponent.h"
#include "Public/Core/Application/Entity.h"


#include "Public/Core/Application/Application.h"

#include "Public/Core/Renderer/Graphics/Texture.h"

namespace GEngine {

	Ref < BatchRenderer >SpriteComponent::s_ShapeFactory = nullptr;
	

	void SpriteComponent::OnAttached(Ref<Entity> entity)
	{
		entity->AddTransformCallback(std::static_pointer_cast<Component>(self.lock()), [this](Ref<Transform> transform, TransformData transData) {
			if (IsInitialized()) {
				for (ShapeID id : m_ids) {
					Vector3f pos = m_shapeFactory->GetShapePosition(id);
					Vector3f nPos = pos - transData.position + transform->GetPosition();
					if (pos != nPos)
						m_shapeFactory->SetPosition(id, nPos.xy());
					float rot = m_shapeFactory->GetShapeRotation(id);
					float nRot = rot - transData.rotation.z + transform->GetRotation().z;
					if (rot != nRot)
						m_shapeFactory->SetRotation(id, nRot);
					Vector2f _scale = m_shapeFactory->GetShapeScale(id);
					Vector3f scale(_scale.x, _scale.y, 1);
					Vector3f nScale = scale - transData.scale.z + transform->GetScale().z;
					if (scale != nScale)
						m_shapeFactory->SetScale(id, {nScale.x, nScale.y});
				}
			}
			});
	}

	void SpriteComponent::DeAttached(Ref<Entity> entity)
	{
		entity->RemoveTransformCallback(std::static_pointer_cast<Component>(self.lock()));
	}

	SpriteComponent::SpriteComponent(Ref<Shader> shader, const std::function<void()>& shaderFunc, const std::string& pipelineId)
	{
		
		if (shader != nullptr || pipelineId.size() > 0) {
			if (shader == nullptr) {
				std::string path = std::string("Content/shaders/TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "Batch.glsl";
				shader = Ref<Shader>(Shader::Create(path));
			}
			m_Shader = shader;
			m_shapeFactory = Ref<BatchRenderer>(new BatchRenderer(ERenderType::GAME, Ref<Quad>(new Quad()),
				1000, m_Shader, pipelineId.size() > 0 ? pipelineId.c_str() : nullptr, shaderFunc));
			return;
		} else if (s_ShapeFactory == nullptr) {
			std::string path = std::string("Content/shaders/TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "Batch.glsl";
			m_Shader = Ref<Shader>(Shader::Create(path));
			SpriteComponent::s_ShapeFactory = Ref<BatchRenderer>(new BatchRenderer(ERenderType::GAME, Ref<Quad>(new Quad()),
				1000, m_Shader, nullptr, shaderFunc));
			
		}
		m_shapeFactory = s_ShapeFactory;
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

		
	}

	const ShapeID SpriteComponent::CreateQuad(const Vector3f& _pos, const float rotation, const Vector3f& scale, const Vector4f& _color,
		const Ref<Texture2D> texture, const Vector2f& textureScale)
	{
		Ref<Entity> e = entity.lock();
		Vector3f _scale = scale * e->GetEntityScale();

		ShapeID id = m_shapeFactory->AddShape(_pos + e->GetEntityPosition(), rotation + e->GetEntityRotation().z, _scale.xy(), _color, texture, textureScale);
		m_ids.push_back(id);
		return id;
	}

	ShapeID SpriteComponent::CreateQuadScript(const Ref<ScriptVector3>& _pos, const float rot, const Ref<ScriptVector3>& scale, const Ref<ScriptVector4>& _color,
		const Ref<Texture2D>& texture, const Vector2f& textureScale)
	{
		return CreateQuad(_pos->GetGlm(), rot, scale->GetGlm(), _color->GetGlm(), texture,textureScale);
	}

	ShapeID SpriteComponent::CreateSubTexturedQuadScript(const Ref<ScriptVector3>& _pos, const float rot, const Ref<ScriptVector3>& scale,
		const Ref<ScriptVector4>& _color,const Ref<SubTexture2D>& texture, const Vector2f& textureScale)
	{
		return CreateSubTexturedQuad(_pos->GetGlm(), rot , scale->GetGlm(), _color->GetGlm(), texture, textureScale);
	}

	void SpriteComponent::SetSubTexture(const ShapeID id, const Ref<SubTexture2D>& texture)
	{
		m_shapeFactory->SetSubTexture(id, texture);
	}

	void SpriteComponent::SetPosition(const ShapeID id, const Vector2f& position)
	{
		m_shapeFactory->SetPosition(id, position + Vector2f( entity.lock()->GetEntityPosition().x, entity.lock()->GetEntityPosition().y ) );
	}

	void SpriteComponent::SetPositionScript(const ShapeID id, const Ref<ScriptVector2>& position)
	{
		SetPosition(id, position->GetGlm());
	}

	void SpriteComponent::SetScaleScript(const ShapeID id,const Ref<ScriptVector2>& scale)
	{
		m_shapeFactory->SetScale(id, scale->GetGlm());
	}

	void SpriteComponent::SetZOrder(const ShapeID id, const float zOrder)
	{
		m_shapeFactory->SetZOrder(id, zOrder+entity.lock()->GetEntityPosition().z);
	}

	void SpriteComponent::SetQuadColor(const ShapeID id, const Vector4f& color)
	{
		m_shapeFactory->SetColor(id, color);
	}

	void SpriteComponent::SetQuadColorScript(const ShapeID id, const Ref<ScriptVector4>& color)
	{
		SetQuadColor(id, color->GetGlm());
	}

	void SpriteComponent::SetRotation(const ShapeID id, const float rotation)
	{
		m_shapeFactory->SetRotation(id, rotation);
	}

	void SpriteComponent::SetTextureScale(const ShapeID id, const Vector2f& scale)
	{
		m_shapeFactory->SetTextureScale(id, scale);
	}

	ShapeID SpriteComponent::CreateSubTexturedQuad(const Vector3f& _pos, const float rot /*= 0*/, const Vector3f& scale /*= { 1,1,1 }*/,
		const Vector4f& _color /*= { 1,1,1,1.f }*/, const Ref<SubTexture2D>& texture /*= nullptr*/, const Vector2f& textureScale/*= 1*/)
	{
		Ref<Entity> e = entity.lock();
		ShapeID id = m_shapeFactory->AddShape(_pos + e->GetEntityPosition(), rot + e->GetEntityRotation().z, (scale * e->GetEntityScale()).xy(), _color, texture, textureScale);
		m_ids.push_back(id);
		return id;
	}

	void SpriteComponent::RemoveQuad(const ShapeID id)
	{
		m_shapeFactory->RemoveShape(id);
		m_ids.erase(std::find(m_ids.begin(), m_ids.end(), id));
	}

	void SpriteComponent::ClearQuads()
	{
		for (ShapeID id : m_ids)
			m_shapeFactory->RemoveShape(id);
		m_ids.clear();
	}

	Vector2f SpriteComponent::GetQuadScale(const ShapeID& id)
	{
		return m_shapeFactory->GetShapeScale(id);
	}

	void SpriteComponent::OnBegin()
	{
	
		
	}

	void SpriteComponent::OnEnd()
	{
		ClearQuads();
	}


}
