#include "gepch.h"

#include "Public/Core/Application/Entity.h"
#include "Public/Core/Application/Component.h"
#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Application/Components/TransformComponent.h"

#include "Public/Core/Util/ThreadPool.h"
#include "Public/Core/Util/Factory.h"

namespace GEngine {


	Entity::Entity(const uint32_t& id)
	{
		go_hash = id;
		transform = AddComponent<Transform>(this);
	}

	int Entity::refCount = 0;

	Entity::~Entity()
	{
		GE_LOG_DEBUG("ENTITY DESTROYED");
	}

	void Entity::UnloadGraphics() {
        OnUnloadGraphics();
		for (Component* c : components) {
			c->UnloadGraphics();
		}
	}

	void Entity::ReloadGraphics() {
        OnReloadGraphics();
		for (Component* c : components) {
			c->ReloadGraphics();
		}
	}

	void Entity::Begin() {
        bInit = true;
		OnBegin();
		for (Component* c : components) {
			c->Begin();
		}
	}

	void Entity::End() {
		if (bInit) {
			OnEnd();
			for (Component* c : components) {
                //c.second->DeAttached(nullptr);
				c->End();
			}
		}
		bInit = false;
		Clean();
	}

	void Entity::Update(Timestep timestep)
	{
		OnUpdate(timestep);
		for (Component* c : components) {
			if (c->GetShouldUpdate()) {
				c->Update(timestep);
			}
		}
		/*
		for (std::pair<uint64_t, Ref<Component>> c : components) {
			if (c.second->GetShouldUpdate()) {
				c.second->Update(timestep);
			}
		}
		*/
	}

	void Entity::Destroy() {
		End();
		SceneManager::GetCurrentScene()->DestroyEntity(this);
		
	}

	void Entity::Clean()
	{
		//entt::registry& r = SceneManager::GetCurrentScene()->GetRegistry();
		//r.remove_all((entt::entity)go_hash);
		for (Component* c : components) {
			delete c;
		}
		components.clear();
	}

	const Vector3f Entity::GetPosition() {
		return transform->GetPosition();
	}

	const Vector3f Entity::GetRotation() {
		return transform->GetRotation();
	}

	const Vector3f Entity::GetScale() {
		return transform->GetScale();
	}

	void Entity::SetPosition(const Vector3f& position)
	{
		TransformData transData = { transform->GetPosition(), transform->GetRotation(), transform->GetScale() };
		transform->SetPosition(position);
		for (std::pair<Component*, std::function<void(Transform*, TransformData)>> pair : m_TransformCallback) {
			pair.second(transform, transData);
		}
	}

	void Entity::SetScale(const Vector3f& scale)
	{
		TransformData transData = { transform->GetPosition(), transform->GetRotation(), transform->GetScale() };
		transform->SetScale(scale);
		for (std::pair<Component*, std::function<void(Transform*, TransformData)>> pair : m_TransformCallback) {
			pair.second(transform, transData);
		}
	}

	void Entity::SetRotation(const Vector3f& rot)
	{
		TransformData transData = { transform->GetPosition(), transform->GetRotation(), transform->GetScale() };
		transform->SetRotation(rot);
		for (std::pair<Component*, std::function<void(Transform*, TransformData)>> pair : m_TransformCallback) {
			pair.second(transform, transData);
		}
	}


	void Entity::AddTransformCallback(Component* component, std::function<void(Transform*, TransformData)> func)
	{
		m_TransformCallback[component] = func;
	}

	void Entity::RemoveTransformCallback(Component* component)
	{
		m_TransformCallback.erase(component);
	}

}
