#include "gepch.h"
#include "Public/Core/Application/Components/QuadColliderComponent.h"
#include "Public/Core/Application/Components/TransformComponent.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Collision/Collider2D.h"
#include "Public/Core/Collision/CollisionDetection.h"
#include "Public/Core/Scripting/ScriptObject.h"

//Debug
#include "Public/Core/Renderer/Renderer.h"

namespace GEngine {

	QuadColliderComponent::QuadColliderComponent(bool dynamic, glm::vec2 position, glm::vec2 scale, float rotation) : Component(), m_position(position), m_scale(scale), m_rotation(rotation),
		m_dynamic(dynamic)
	{
		//this->bUpdates = true;
	}

	QuadColliderComponent::~QuadColliderComponent()
	{
		if (m_collider) {
			CollisionDetection::RemoveCollider(m_collider);
		}
		m_endCollide = nullptr;
		m_onCollide = nullptr;
	}

	void QuadColliderComponent::OnBegin()
	{
		Ref<Entity> e = entity.lock();
		m_worldPosition = glm::vec2(m_position.x + e->GetEntityPosition().x, m_position.y + e->GetEntityPosition().y);
		m_worldScale = glm::vec2(m_scale.x * e->GetEntityScale().x, m_scale.y * e->GetEntityScale().y);
		m_worldRotation = m_rotation + e->GetEntityRotation().z;
		m_collider = make_shared<Collider2D>(m_worldPosition,
			m_worldScale, m_worldRotation);
		m_collider->SetColliderShape(Quad);
		m_collider->SetColliderLayer(Game);
		m_collider->SetColliderType(m_dynamic ? Dynamic : Static);
		m_collider->SetComponent(static_pointer_cast<Component>(self.lock()));
		m_collider->SetEntity(static_pointer_cast<Entity>(entity.lock()));
		m_collider->SetCollisionStartFunction([this](Ref < Collider> c) {
			if (m_onCollide != nullptr) {
				m_onCollide(static_pointer_cast<QuadColliderComponent>(c->GetComponent().lock()));
			}
		});
		m_collider->SetCollisionEndFunction([this](Ref < Collider> c) {
			if (m_endCollide != nullptr) {
				m_endCollide(static_pointer_cast<QuadColliderComponent>(c->GetComponent().lock()));
			}
			});
		CollisionDetection::AddCollider(m_collider);
	}

	void QuadColliderComponent::OnEnd()
	{
		CollisionDetection::RemoveCollider(m_collider);
		m_collider = nullptr;
	}

	void QuadColliderComponent::OnUpdate(Timestep ts)
	{
		//Renderer::DrawCube(glm::vec3(m_position.x, m_position.y, 1e6), glm::vec3(.25, .25, 1), glm::vec4(1, 0, 1, 1));
	}

	void QuadColliderComponent::SetPosition(float x, float y)
	{
		Ref<Entity> e = entity.lock();
		m_position = glm::vec2(x + e->GetEntityPosition().x, y + e->GetEntityPosition().y);
		m_collider->SetPosition(glm::vec3(m_position.x, m_position.y, 1));
	}

	void QuadColliderComponent::SetScale(float x, float y)
	{
		Ref<Entity> e = entity.lock();
		m_scale = glm::vec2(x * e->GetEntityScale().x, y * e->GetEntityScale().y);
		m_collider->SetScale(glm::vec3(m_scale.x, m_scale.y,1));
	}

	glm::vec2 QuadColliderComponent::GetPosition()
	{
		return m_worldPosition;
	}

	glm::vec2 QuadColliderComponent::GetScale()
	{
		return m_worldScale;
	}

	Ref<GEngine::ScriptVector2> QuadColliderComponent::GetPositionScript()
	{
		return make_unique<ScriptVector2>(GetPosition());
	}

	Ref<ScriptVector2> QuadColliderComponent::GetScaleScript()
	{
		return make_unique<ScriptVector2>(GetScale());
	}

	void QuadColliderComponent::SetOnCollideFunction(std::function<void(Ref<QuadColliderComponent>)> onCollideFunc)
	{
		m_onCollide = onCollideFunc;
	}

	void QuadColliderComponent::SetOnCollideFunction_Script(Ref<ScriptObject> onColliderFunc)
	{
		SetOnCollideFunction([onColliderFunc](Ref<QuadColliderComponent> collider) { onColliderFunc->CallSelf(collider);  });
	}

	void QuadColliderComponent::SetEndCollideFunction(std::function<void(Ref<QuadColliderComponent>)> onCollideFunc)
	{
		m_endCollide = onCollideFunc;
	}

	void QuadColliderComponent::SetEndCollideFunction_Script(Ref<ScriptObject> onCollideFunc)
	{
		SetEndCollideFunction([onCollideFunc](Ref<QuadColliderComponent> collider) { onCollideFunc->CallSelf(collider);  });
	}

	void QuadColliderComponent::RemoveOnCollideFunction()
	{
		m_onCollide = nullptr;
	}

	void QuadColliderComponent::RemoveEndCollideFunction()
	{
		m_endCollide = nullptr;
	}

	void QuadColliderComponent::OnAttached(Ref<Entity> entity)
	{
		entity->AddTransformCallback(std::static_pointer_cast<Component>(self.lock()), [this](Ref<Transform> transform, TransformData transData) {
			if (IsInitialized()) {
				glm::vec3 pos = glm::vec3(m_position.x, m_position.y, 1);
				pos = pos + transform->GetPosition();
				m_worldPosition = pos;
				m_collider->SetPosition(pos);


			
				glm::vec3 scale = glm::vec3(m_scale.x, m_scale.y, 1);
				scale = scale * transform->GetScale();
				m_worldScale = glm::vec2(scale.x, scale.y);
				m_collider->SetScale(scale);

				glm::vec3 rotation = glm::vec3(0, 0, m_rotation);
				rotation = rotation + transData.rotation;
				rotation.x = 0;
				rotation.y = 0;
				m_worldRotation = rotation.z;
			}
			});
	}

	void QuadColliderComponent::DeAttached(Ref<Entity> entity)
	{
		entity->RemoveTransformCallback(std::static_pointer_cast<Component>(self.lock()));
	}



}