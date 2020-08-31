#include "gepch.h"
#include "Public/Core/Application/Components/QuadColliderComponent.h"
#include "Public/Core/Application/Components/TransformComponent.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Collision/Collider2D.h"
#include "Public/Core/Collision/CollisionDetection.h"
#include "Public/Core/Scripting/ScriptObject.h"

#include "Public/Core/Physics/Physics.h"
#include "Public/Core/Physics/PhysicsStructures.h"
#include "Public/Core/Physics/PhysicsBody.h"

#include "Public/Core/FileSystem/FileSystem.h"
//Debug
#include "Public/Core/Renderer/Renderer.h"

namespace GEngine {

	QuadColliderComponent::QuadColliderComponent(bool dynamic, bool physics, const glm::vec2& position, const glm::vec2& scale, const float rotation, const float mass) : Component(),
		m_position(position), m_scale(scale), m_rotation(rotation),
		m_dynamic(dynamic), m_mass(mass), m_physics(physics)
	{
		if (dynamic)
			bUpdates = true;
		
	}

	QuadColliderComponent::~QuadColliderComponent()
	{
		m_endCollide = nullptr;
		m_onCollide = nullptr;
		m_body = nullptr;
	}

	void QuadColliderComponent::OnBegin()
	{
		Ref<Entity> e = entity.lock();
		m_worldPosition = glm::vec2(m_position.x + e->GetEntityPosition().x, m_position.y + e->GetEntityPosition().y);
		m_worldScale = glm::vec2(m_scale.x * e->GetEntityScale().x, m_scale.y * e->GetEntityScale().y);
		m_worldRotation = m_rotation + e->GetEntityRotation().z;

		PhysicsInfo info;
		info.type = m_dynamic ? PhysicsInfoType::PHYSICS_Dynamic : PhysicsInfoType::PHYSICS_Static;
		info.position = glm::vec2(e->GetEntityPosition().x, e->GetEntityPosition().y);
		info.rotation = glm::radians(e->GetEntityRotation().z);
		info.fixedRotation = true;
		m_body = Physics::CreateBody(info);
		m_body->SetComponent(std::static_pointer_cast<Component>(self.lock()));
		m_body->SetSensor(!m_physics);
		m_body->SetQuad(m_worldScale, m_position, m_mass, m_rotation);

		
		m_body->SetOnCollideStartFunction([this](Ref < PhysicsBody> c) {
			if (m_onCollide != nullptr) {
				m_onCollide(static_pointer_cast<QuadColliderComponent>(c->GetComponent()));
			}
		});
		m_body->SetOnCollideEndFunction([this](Ref < PhysicsBody> c) {
			if (m_endCollide != nullptr) {
				m_endCollide(static_pointer_cast<QuadColliderComponent>(c->GetComponent()));
			}
			});
	}

	void QuadColliderComponent::OnEnd()
	{
		m_body = nullptr;
	}

	void QuadColliderComponent::OnUpdate(Timestep ts)
	{
		if (m_physics && m_dynamic) {
			const glm::vec2& pos = m_body->GetPosition();
			const float rot = m_body->GetRotation();
			entity.lock()->SetEntityPosition(glm::vec3(pos.x, pos.y, 1));
			entity.lock()->SetEntityRotation(glm::vec3(0, 0, glm::degrees(rot)));
		}
	}

	void QuadColliderComponent::SetPosition(const float x, const float y)
	{
		Ref<Entity> e = entity.lock();
		m_position = glm::vec2(x + e->GetEntityPosition().x, y + e->GetEntityPosition().y);
		//m_collider->SetPosition(glm::vec3(m_position.x, m_position.y, 1));
		//m_body->SetPosition(glm::vec2(x, y));
	}

	void QuadColliderComponent::SetScale(const float x, const float y)
	{
		Ref<Entity> e = entity.lock();
		m_scale = glm::vec2(x * e->GetEntityScale().x, y * e->GetEntityScale().y);
		
		//m_collider->SetScale(glm::vec3(m_scale.x, m_scale.y,1));
	}

	const glm::vec2 QuadColliderComponent::GetPosition()
	{
		return m_worldPosition;
	}

	const glm::vec2 QuadColliderComponent::GetScale()
	{
		return m_worldScale;
	}

	const glm::vec2 QuadColliderComponent::GetLinearVelocity()
	{
		if (m_physics) {
			return m_body->GetLinearVelocity();
		}
		return glm::vec2(0);
	}

	Ref<ScriptVector2> QuadColliderComponent::GetLinearVelocityScript()
	{
		return make_shared<ScriptVector2>(GetLinearVelocity());
	}

	Ref<GEngine::ScriptVector2> QuadColliderComponent::GetPositionScript()
	{
		return make_unique<ScriptVector2>(GetPosition());
	}

	Ref<ScriptVector2> QuadColliderComponent::GetScaleScript()
	{
		return make_unique<ScriptVector2>(GetScale());
	}

	void QuadColliderComponent::SetBounce(const float bounce)
	{
		if (m_physics)
			m_body->SetBounce(bounce);
	}

	const float QuadColliderComponent::GetBounce()
	{
		if (m_physics)
			return m_body->GetBounce();
		return 0;
	}

	void QuadColliderComponent::SetDynamic(bool b)
	{
		m_dynamic = b;
	}

	void QuadColliderComponent::IncreaseLinearVelocity(float x, float y)
	{
		glm::vec2 v = m_body->GetLinearVelocity();
		v.x += x;
		v.y += y;
		m_body->SetLinearVelocity(v);
	}

	void QuadColliderComponent::IncreaseAngularVelocity(const float v)
	{
		m_body->SetAngularVelocity(m_body->GetAngularVelocity() + v);
	}

	void QuadColliderComponent::SetVelocityX(const float x)
	{
		glm::vec2 v = m_body->GetLinearVelocity();
		m_body->SetLinearVelocity({ x, v.y });
	}

	void QuadColliderComponent::SetVelocityY(const float y)
	{
		glm::vec2 v = m_body->GetLinearVelocity();
		m_body->SetLinearVelocity({ v.x, y });
	}

	void QuadColliderComponent::SetVelocity(const float x, const float y)
	{
		m_body->SetLinearVelocity({ x, y });
	}

	void QuadColliderComponent::SetOnCollideFunction(std::function<void(Ref<QuadColliderComponent>)> onCollideFunc)
	{
		m_onCollide = onCollideFunc;
	}

	void QuadColliderComponent::SetOnCollideFunction_Script(Ref<ScriptObject> onColliderFunc)
	{
		SetOnCollideFunction([onColliderFunc](Ref<QuadColliderComponent> collider) { onColliderFunc->CallSelf(collider); if (ScriptObject::HasError()) { GE_CORE_ERROR("{0} at (\"{1}\")", ScriptObject::GetError(), FileSystem::FilePath(onColliderFunc->GetPath())); }  });
	}

	void QuadColliderComponent::SetEndCollideFunction(std::function<void(Ref<QuadColliderComponent>)> onCollideFunc)
	{
		m_endCollide = onCollideFunc;
	}

	void QuadColliderComponent::SetEndCollideFunction_Script(Ref<ScriptObject> onCollideFunc)
	{
		SetEndCollideFunction([onCollideFunc](Ref<QuadColliderComponent> collider) {
			onCollideFunc->CallSelf(collider);
			if (ScriptObject::HasError())
			{
				GE_CORE_ERROR("{0} at (\"{1}\")", ScriptObject::GetError(), FileSystem::FilePath(onCollideFunc->GetPath()));
			}
			});
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
				if (!m_physics)
					m_body->SetPosition(transform->GetPosition());


			
				glm::vec3 scale = glm::vec3(m_scale.x, m_scale.y, 1);
				scale = scale * transform->GetScale();
				m_worldScale = glm::vec2(scale.x, scale.y);
				if (!m_physics)
					m_body->SetScale(m_worldScale);

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