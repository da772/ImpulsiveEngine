#include "gepch.h"
#include "Public/Core/Application/Components/UI/ButtonComponent.h"
#include "Public/Core/Renderer/Graphics/BatchRenderer.h"

#include "Public/Core/Application/Application.h"
#include "Public/Core/Application/Components/TransformComponent.h"
#include "Public/Core/Application/Entity.h"

#include "Public/Core/Scripting/ScriptObject.h"
#include "Public/Core/FileSystem/FileSystem.h"

#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/Graphics/Quad.h"
#include "Public/Core/Collision/Collider2D.h"
#include "Public/Core/Collision/CollisionDetection.h"


namespace GEngine {

	Ref<BatchRenderer> ButtonComponent::s_ShapeFactory = nullptr;

	ButtonComponent::ButtonComponent(glm::vec3 pos, float rot, glm::vec2 scale, glm::vec4 color, float textureScale /*= 1*/) :
		m_colliderPosition(pos), m_colliderRotation(rot), m_colliderScale(scale), m_imagePosition(pos), m_imageRotation(rot), m_imageScale(scale), m_color(color), m_textureScale(textureScale)
	{
		if (b_debug)
			m_debug = 2;
		if (s_ShapeFactory == nullptr) {
			std::string path = std::string("Content/shaders/TextureShader_" + std::to_string(RenderCommand::GetMaxTextureSlots())) + "UIBatch.glsl";
			m_Shader = Ref<Shader>(Shader::Create(path));
			ButtonComponent::s_ShapeFactory = Ref<BatchRenderer>(new BatchRenderer(ERenderType::UI, Ref<Quad>(new Quad()), 5000, m_Shader));
			s_ShapeFactory->SetRenderType(ERenderType::UI);
		}
	}

	ButtonComponent::~ButtonComponent()
	{

	}

	void ButtonComponent::SetImageSubTexture(Ref<SubTexture2D> texture)
	{
		m_subTexture = texture;
	}

	void ButtonComponent::SetImageTexture(Ref<Texture2D> texture)
	{
		m_texture = texture;
	}

	void ButtonComponent::SetImageColor(const glm::vec4& color)
	{
		if (s_ShapeFactory)
			s_ShapeFactory->SetColor(m_ids[0], color);
	}

	void ButtonComponent::SetImageRotation(float rot)
	{
		if (s_ShapeFactory)
			s_ShapeFactory->SetRotation(m_ids[0], rot);
	}

	void ButtonComponent::SetImagePosition(glm::vec2 pos)
	{
		s_ShapeFactory->SetPosition(m_ids[0], pos);
	}

	void ButtonComponent::SetImageScale(glm::vec2 scale)
	{
		s_ShapeFactory->SetScale(m_ids[0], scale);
	}

	void ButtonComponent::SetColliderScale(glm::vec2 scale)
	{
		if (b_debug && s_ShapeFactory)
			s_ShapeFactory->SetScale(m_ids[1], scale);
		m_collider->SetScale({ scale.x, scale.y, 1 });
	}

	void ButtonComponent::SetColliderPosition(glm::vec2 pos)
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
		glm::vec3 _pos = m_collider->GetPosition();
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

	void ButtonComponent::SetOnMouseEndCollideScript(Ref<ScriptObject> obj)
	{
		SetOnMouseEndCollide([obj](float f1, float f2) {
			obj->CallSelf(f1,f2);
			if (ScriptObject::HasError()) { 
				GE_CORE_ERROR("{0} at (\"{1}\")", ScriptObject::GetError(), FileSystem::FilePath(obj->GetPath()));
			}
			});
	}

	void ButtonComponent::SetOnMouseStartCollideScript(Ref<ScriptObject> obj)
	{
		SetOnMouseStartCollide([obj](float f1, float f2) {
			obj->CallSelf(f1, f2);
			if (ScriptObject::HasError()) {
				GE_CORE_ERROR("{0} at (\"{1}\")", ScriptObject::GetError(), FileSystem::FilePath(obj->GetPath()));
			}
			});
	}

	void ButtonComponent::OnAttached(Ref<Entity> entity)
	{
		entity->AddTransformCallback(std::static_pointer_cast<Component>(self.lock()), [this](Ref<Transform> transform, TransformData transData) {
			if (IsInitialized()) {
				for (int i = 0; i < m_debug; i++) {
					long id = m_ids[i];
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
						s_ShapeFactory->SetScale(id, { nScale.x, nScale.y });
				}
				glm::vec3 pos = glm::vec3(m_colliderPosition.x, m_colliderPosition.y, 1);
				pos = pos + transform->GetPosition();
				m_worldPosition = pos;
				m_collider->SetPosition(pos);

				glm::vec3 scale = glm::vec3(m_colliderScale.x, m_colliderScale.y, 1);
				scale = scale * transform->GetScale();
				m_worldScale = glm::vec2(scale.x, scale.y);
				m_collider->SetScale(scale);

				glm::vec3 rotation = glm::vec3(0, 0, m_colliderRotation);
				rotation = rotation + transData.rotation;
				rotation.x = 0;
				rotation.y = 0;
				m_worldRotation = rotation.z;
			}
			});
	}

	void ButtonComponent::DeAttached(Ref<Entity> entity)
	{
		entity->RemoveTransformCallback(std::static_pointer_cast<Component>(self.lock()));
	}

	void ButtonComponent::OnBegin()
	{
		Ref<Entity> e = entity.lock();
		for (int i = 0; i < m_debug; i++) {
			if (i == 0)
				m_ids[i] = s_ShapeFactory->AddShape(m_imagePosition+e->GetEntityPosition(), m_imageRotation, m_imageScale, m_color, m_texture, m_textureScale);
			if (i == 1) {
				glm::vec3 m_p = m_colliderPosition;
				m_p.z += .1f;
				m_ids[i] = s_ShapeFactory->AddShape(m_p + e->GetEntityPosition(), m_colliderRotation, m_colliderScale, glm::vec4(1, 0, 0, .25f), nullptr);
			}
		}
		
		m_worldPosition = glm::vec2(m_colliderPosition.x + e->GetEntityPosition().x, m_colliderPosition.y + e->GetEntityPosition().y);
		m_worldScale = glm::vec2(m_colliderScale.x * e->GetEntityScale().x, m_colliderScale.y * e->GetEntityScale().y);
		m_worldRotation = m_colliderRotation + e->GetEntityRotation().z;
		m_collider = make_shared<Collider2D>(m_worldPosition,
			m_worldScale, m_worldRotation);
		m_collider->SetColliderShape(EColliderShape::Quad);
		m_collider->SetColliderLayer(EColliderLayer::UI);
		m_collider->SetColliderType(EColliderType::Dynamic);
		m_collider->SetComponent(static_pointer_cast<Component>(self.lock()));
		m_collider->SetEntity(static_pointer_cast<Entity>(entity.lock()));
		m_collider->SetUICollisionStartFunction([this](float x, float y) {
			GE_CORE_DEBUG("BUTTON COLLIDE START");
			if (m_onCollide != nullptr) {
				m_onCollide(x, y);
			}
			});
		m_collider->SetUICollisionEndFunction([this](float x, float y) {
			GE_CORE_DEBUG("BUTTON COLLIDE END");
			if (m_endCollide != nullptr) {
				m_endCollide(x,y);
			}
			});
		CollisionDetection::AddCollider(m_collider);
	}

	void ButtonComponent::OnEnd()
	{
		CollisionDetection::RemoveCollider(m_collider);
		for (int i = 0; i < m_debug; i++) {
			s_ShapeFactory->RemoveShape(m_ids[i]);
		}
	}

	void ButtonComponent::OnUpdate(Timestep timestep)
	{
		
	}

}