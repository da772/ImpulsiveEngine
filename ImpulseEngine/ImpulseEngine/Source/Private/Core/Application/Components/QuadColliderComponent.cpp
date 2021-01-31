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
#include "Public/Core/Util/ThreadPool.h"

//Debug
#include "Public/Core/Renderer/Renderer.h"

namespace GEngine {

	QuadColliderComponent::QuadColliderComponent(bool dynamic, bool physics, const Vector2f& position, bool fixedRotation, float bounce) : Component(),
		m_position(position),
		m_dynamic(dynamic), m_physics(physics), m_bounce(bounce), m_fixedRotation(fixedRotation)
	{
		if (dynamic)
			bUpdates = true;

		m_tag = "QuadColliderComponent";
	}

	QuadColliderComponent::~QuadColliderComponent()
	{
		m_body = nullptr;
	}

	void QuadColliderComponent::OnBegin()
	{
		Ref<Entity> e = entity.lock();
		m_worldPosition = Vector2f(m_position.x + e->GetEntityPosition().x, m_position.y + e->GetEntityPosition().y);
		m_worldScale = Vector2f(m_scale.x * e->GetEntityScale().x, m_scale.y * e->GetEntityScale().y);
		m_worldRotation = m_rotation + e->GetEntityRotation().z;

		PhysicsInfo info;
		info.type = m_dynamic ? PhysicsInfoType::PHYSICS_Dynamic : PhysicsInfoType::PHYSICS_Kinematic;
		info.position = Vector2f(e->GetEntityPosition().x, e->GetEntityPosition().y);
		info.rotation = glm::radians(e->GetEntityRotation().z);
		info.fixedRotation = m_fixedRotation;
		m_body = Physics::CreateBody(info);
		m_body->SetComponent(std::static_pointer_cast<Component>(self.lock()));
	}


	const GEngine::ColliderID QuadColliderComponent::CreateQuad(const Vector2f& position, const Vector2f& scale, float mass, float rotation, const std::string& tag)
	{
		const ColliderID id = m_body->CreateQuad(scale, position, mass, rotation, tag);
		const FColliderQuad quad = { id, position, scale, mass, rotation, tag, m_bounce, true };
		m_body->SetSensor(id, !m_physics);
		m_body->SetGroupIndex(id, m_groupIndex);
		m_body->SetCategory(id, m_category);
		m_body->SetMask(id, m_mask);
		m_quads[id] = quad;
		ThreadPool::AddMainThreadFunction([this]() {m_body->SetAwake(true); });

		return id;
	}

	const GEngine::ColliderID QuadColliderComponent::CreateCircle(const Vector2f& position, const Vector2f& scale, float mass, float rotation /*= 0*/, const std::string& tag /*= ""*/)
	{
		const ColliderID id = m_body->CreateCircle(scale, position, mass, rotation, tag);
		const FColliderQuad quad = { id, position, scale, mass, rotation, tag, m_bounce, false };
		m_body->SetSensor(id, !m_physics);
		m_body->SetGroupIndex(id, m_groupIndex);
		m_body->SetCategory(id, m_category);
		m_body->SetMask(id, m_mask);
		m_quads[id] = quad;
		ThreadPool::AddMainThreadFunction([this]() {m_body->SetAwake(true); });
		return id;
	}

	void QuadColliderComponent::DestroyQuad(const ColliderID id)
	{
		m_body->DestroyQuad(id);
		m_quads.erase(id);
	}

	void QuadColliderComponent::OnEnd()
	{
		m_body = nullptr;
	}

	void QuadColliderComponent::OnUpdate(Timestep ts)
	{
		if (m_physics && m_dynamic) {
			const Vector2f& pos = m_body->GetPosition();
			const float rot = m_body->GetRotation();
			m_movedSelf = true;
			entity.lock()->SetEntityPosition(Vector3f(pos.x, pos.y, 1.f));
			entity.lock()->SetEntityRotation(Vector3f(0.f, 0.f, glm::degrees(rot)));
			m_movedSelf = false;
		}
	}

	void QuadColliderComponent::SetPosition(const float x, const float y)
	{
		Ref<Entity> e = entity.lock();
		m_position = Vector2f(x + e->GetEntityPosition().x, y + e->GetEntityPosition().y);
	}

	void QuadColliderComponent::SetScale(const float x, const float y)
	{
		Ref<Entity> e = entity.lock();
		m_scale = Vector2f(x * e->GetEntityScale().x, y * e->GetEntityScale().y);
		
		//m_collider->SetScale(Vector3f(m_scale.x, m_scale.y,1));
	}



	void QuadColliderComponent::SetRotation(const float rot)
	{
		Ref<Entity> e = entity.lock();
		m_rotation = rot + e->GetEntityRotation().z;
	}

	const Vector2f QuadColliderComponent::GetPosition()
	{
		return m_worldPosition;
	}

	const Vector2f QuadColliderComponent::GetScale()
	{
		return m_worldScale;
	}

	const GEngine::Vector2f QuadColliderComponent::GetLocalPosition()
	{
		return m_worldPosition - GetEntityPosition().xy();
	}

	const GEngine::Vector2f QuadColliderComponent::GetLocalScale()
	{
		return m_worldScale / GetEntity()->GetEntityScale().xy();
	}

	const float QuadColliderComponent::GetLocalRotation()
	{
		return m_worldRotation - GetEntity()->GetEntityRotation().z;
	}

	void QuadColliderComponent::SetGravityScale(const float f)
	{
		if (m_physics)
			m_body->SetGravityScale(f);
	}

	const Vector2f QuadColliderComponent::GetLinearVelocity()
	{
		if (m_physics) {
			return m_body->GetLinearVelocity();
		}
		return Vector2f(0.f);
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

	void QuadColliderComponent::SetBounce(const ColliderID id,const float bounce)
	{
		m_bounce = bounce;
		m_quads[id].bounce = bounce;
		m_body->SetBounce(id, bounce);
	}

	const float QuadColliderComponent::GetBounce(const ColliderID id)
	{
		return m_quads[id].bounce;
	}

	void QuadColliderComponent::SetFixedRotation(const bool rot)
	{
		if (m_physics)
			m_body->SetFixedRotation(rot);
		m_fixedRotation = rot;
	}


	const GEngine::FColliderQuad& QuadColliderComponent::GetQuadCollider(const ColliderID id)
	{
		return m_quads[id];
	}

	void QuadColliderComponent::IncreaseLinearVelocity(float x, float y)
	{
		Vector2f v = m_body->GetLinearVelocity();
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
		Vector2f v = m_body->GetLinearVelocity();
		m_body->SetLinearVelocity({ x, v.y });
	}

	void QuadColliderComponent::SetVelocityY(const float y)
	{
		Vector2f v = m_body->GetLinearVelocity();
		m_body->SetLinearVelocity({ v.x, y });
	}

	void QuadColliderComponent::SetVelocity(const float x, const float y)
	{
		m_body->SetLinearVelocity({ x, y });
	}

	void QuadColliderComponent::SetCollisionLayers(const ColliderID id, const uint16_t category, const uint16_t mask, const int16_t index)
	{
		m_mask = mask;
		m_category = category;
		m_groupIndex = index;
		if (m_body) {
			m_body->SetCategory(id, category);
			m_body->SetMask(id, mask);
			m_body->SetGroupIndex(id, index);
		}
	}

	void QuadColliderComponent::WakeBody(bool wake)
	{
		m_body->SetAwake(wake);
	}

	void QuadColliderComponent::SetOnCollideFunction(const ColliderID id, std::function<void(Ref<PhysicsCollision>)> onCollideFunc)
	{
		m_body->SetOnCollideStartFunction(id, onCollideFunc);
	}

	void QuadColliderComponent::SetEndCollideFunction(const ColliderID id, std::function<void(Ref<PhysicsCollision>)> onCollideFunc)
	{
		m_body->SetOnCollideEndFunction(id, onCollideFunc);
	}


	void QuadColliderComponent::OnAttached(Ref<Entity> entity)
	{
		entity->AddTransformCallback(std::static_pointer_cast<Component>(self.lock()), [this](Ref<Transform> transform, TransformData transData) {
			if (IsInitialized()) {
				Vector3f pos = Vector3f(m_position.x, m_position.y, 1);
				pos = pos + transform->GetPosition();
				m_worldPosition = pos;
				if (!m_physics) {
					m_body->SetPosition(transform->GetPosition());
					m_body->SetAwake(true);
				}
				else if (!m_movedSelf ) {
					m_body->SetPosition(transform->GetPosition());
					m_body->SetAwake(true);
				}
					
				


			
				Vector3f scale = Vector3f(m_scale.x, m_scale.y, 1);
				scale = scale * transform->GetScale();
				m_worldScale = Vector2f(scale.x, scale.y);
				m_body->SetScale(m_worldScale);

					

				Vector3f rotation = Vector3f(0, 0, m_rotation);
				rotation = rotation + transData.rotation;
				rotation.x = 0;
				rotation.y = 0;
				m_worldRotation = rotation.z;
			}
			});
	}

	void QuadColliderComponent::DeAttached(Ref<Entity> entity)
	{
		if (!entity) return;
		entity->RemoveTransformCallback(std::static_pointer_cast<Component>(self.lock()));
	}



}