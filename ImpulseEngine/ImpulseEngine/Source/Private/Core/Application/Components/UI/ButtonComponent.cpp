#include "gepch.h"
#include "Public/Core/Application/Components/UI/ButtonComponent.h"
#include "Public/Core/Renderer/Graphics/BatchRenderer.h"

#include "Public/Core/Application/Application.h"
#include "Public/Core/Application/Components/TransformComponent.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/FileSystem/FileSystem.h"

#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/Graphics/Quad.h"
#include "Public/Core/Collision/Collider2D.h"
#include "Public/Core/Collision/CollisionDetection.h"
#include "Public/Core/Application/Components/UI/UIComponent.h"


namespace GEngine {

	Ref<BatchRenderer> ButtonComponent::s_ShapeFactory = nullptr;

	ButtonComponent::ButtonComponent(Entity* e, const Vector3f& pos, const float rot, const Vector2f& scale, const Vector4f& color, Vector2f textureScale /*= 1*/) : Component(e),
		m_colliderPosition(pos), m_colliderRotation(rot), m_colliderScale(scale), m_imagePosition(pos), m_imageRotation(rot), m_imageScale(scale), m_color(color), m_textureScale(textureScale)
	{
		if (b_debug)
			m_debug = 2;
		if (s_ShapeFactory == nullptr) {
			std::string path = std::string("EngineContent/shaders//TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "UIBatch.glsl";
			m_Shader = Ref<Shader>(Shader::Create(path));
			ButtonComponent::s_ShapeFactory = UIComponent::s_ShapeFactory;
			s_ShapeFactory->SetRenderType(ERenderType::UI);
		}
	}

	ButtonComponent::~ButtonComponent()
	{

	}

	void ButtonComponent::SetImageSubTexture(Ref<SubTexture2D> texture)
	{
		m_subTexture = texture;
		if (m_ids[0] != 0) {
			s_ShapeFactory->SetSubTexture(m_ids[0], m_subTexture);
		}
	}

	void ButtonComponent::SetImageTexture(Ref<Texture2D> texture)
	{
		m_texture = texture;
		if (m_ids[0] != 0) {
			s_ShapeFactory->SetTexture(m_ids[0], m_texture);
		}
	}

	void ButtonComponent::SetImageColor(const Vector4f& color)
	{
		if (s_ShapeFactory)
			s_ShapeFactory->SetColor(m_ids[0], color);
	}

	void ButtonComponent::SetImageRotation(float rot)
	{
		if (s_ShapeFactory)
			s_ShapeFactory->SetRotation(m_ids[0], rot);
	}

	void ButtonComponent::SetImagePosition(const Vector2f& pos)
	{
		s_ShapeFactory->SetPosition(m_ids[0], pos);
	}

	void ButtonComponent::SetImageScale(const Vector2f& scale)
	{
		s_ShapeFactory->SetScale(m_ids[0], scale);
	}

	void ButtonComponent::SetColliderScale(const Vector2f& scale)
	{
		if (b_debug && s_ShapeFactory)
			s_ShapeFactory->SetScale(m_ids[1], scale);
		m_collider->SetScale({ scale.x, scale.y, 1 });
	}

	void ButtonComponent::SetColliderPosition(const Vector2f& pos)
	{
		if (b_debug && s_ShapeFactory)
			s_ShapeFactory->SetPosition(m_ids[1], pos);
		m_collider->SetPosition({ pos.x,pos.y, m_collider->GetPosition().z });
	}

	void ButtonComponent::SetZOrder(float pos)
	{
		if (s_ShapeFactory) {
			s_ShapeFactory->SetZOrder(m_ids[0], pos);
			if (b_debug)
				s_ShapeFactory->SetZOrder(m_ids[1], pos+.1f);
		}
		Vector3f _pos = m_collider->GetPosition();
		m_collider->SetPosition({ _pos.x, _pos.y, pos });
	}

	void ButtonComponent::UnloadGraphics()
	{
		if (s_ShapeFactory)
			s_ShapeFactory->UnloadGraphics();
	}

	void ButtonComponent::ReloadGraphics()
	{
		if (s_ShapeFactory)
			s_ShapeFactory->ReloadGraphics();
	}



	void ButtonComponent::OnBegin()
	{
		for (int i = 0; i < m_debug; i++) {
			if (i == 0)
				m_ids[i] = s_ShapeFactory->AddShape(m_imagePosition+m_entity->GetPosition(), m_imageRotation, m_imageScale, m_color, m_texture, m_textureScale);
			if (i == 1) {
				Vector3f m_p = m_colliderPosition;
				m_p.z += .1f;
				m_ids[i] = s_ShapeFactory->AddShape(m_p + m_entity->GetPosition(), m_colliderRotation, m_colliderScale, Vector4f(1, 0, 0, .25f));
			}
		}
		
		m_worldPosition = Vector2f(m_colliderPosition.x + m_entity->GetPosition().x, m_colliderPosition.y + m_entity->GetPosition().y);
		m_worldScale = Vector2f(m_colliderScale.x * m_entity->GetScale().x, m_colliderScale.y * m_entity->GetScale().y);
		m_worldRotation = m_colliderRotation + m_entity->GetRotation().z;
		m_collider = new Collider2D(m_worldPosition,
			m_worldScale, m_worldRotation);
		m_collider->SetColliderShape(EColliderShape::Quad);
		m_collider->SetColliderLayer(EColliderLayer::UI);
		m_collider->SetColliderType(EColliderType::Dynamic);
		m_collider->SetComponent(this);
		m_collider->SetEntity(m_entity);
		m_collider->SetUIOnEvent([this](const Event& e) { if (m_onEvent) m_onEvent(e); });
		m_collider->SetUICollisionStartFunction([this](float x, float y) {
			//GE_CORE_DEBUG("BUTTON COLLIDE START");
			if (m_onCollide != nullptr) {
				m_onCollide(x, y);
			}
			});
		m_collider->SetUICollisionEndFunction([this](float x, float y) {
			//GE_CORE_DEBUG("BUTTON COLLIDE END");
			if (m_endCollide != nullptr) {
				m_endCollide(x,y);
			}
			});

		CollisionDetection::AddCollider(m_collider);


		m_entity->AddTransformCallback(this, [this](Transform* transform, TransformData transData) {
			if (IsInitialized()) {
				for (int i = 0; i < m_debug; i++) {
					ShapeID id = m_ids[i];
					Vector3f pos = s_ShapeFactory->GetShapePosition(id);
					Vector3f nPos = pos - transData.position + transform->GetPosition();
					if (pos != nPos)
						s_ShapeFactory->SetPosition(id, nPos);
					float rot = s_ShapeFactory->GetShapeRotation(id);
					float nRot = rot - transData.rotation.z + transform->GetRotation().z;
					if (rot != nRot)
						s_ShapeFactory->SetRotation(id, nRot);
					Vector2f _scale = s_ShapeFactory->GetShapeScale(id);
					Vector3f scale(_scale.x, _scale.y, 1);
					Vector3f nScale = scale - transData.scale.z + transform->GetScale().z;
					if (scale != nScale)
						s_ShapeFactory->SetScale(id, { nScale.x, nScale.y });
				}
				Vector3f pos = Vector3f(m_colliderPosition.x, m_colliderPosition.y, 1);
				pos = pos + transform->GetPosition();
				m_worldPosition = pos;
				m_collider->SetPosition(pos);

				Vector3f scale = Vector3f(m_colliderScale.x, m_colliderScale.y, 1);
				scale = scale * transform->GetScale();
				m_worldScale = Vector2f(scale.x, scale.y);
				m_collider->SetScale(scale);

				Vector3f rotation = Vector3f(0, 0, m_colliderRotation);
				rotation = rotation + transData.rotation;
				rotation.x = 0;
				rotation.y = 0;
				m_worldRotation = rotation.z;
			}
			});
	}

	void ButtonComponent::OnEnd()
	{
		m_entity->RemoveTransformCallback(this);
		CollisionDetection::RemoveCollider(m_collider);
		delete m_collider;
		for (int i = 0; i < m_debug; i++) {
			s_ShapeFactory->RemoveShape(m_ids[i]);
		}
	}

	void ButtonComponent::OnUpdate(Timestep timestep)
	{
		
	}

}
