#include "gepch.h"

#include "Public/Core/Application/Entity.h"
#include "Public/Core/Application/Component.h"
#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Application/Components/TransformComponent.h"

#include "Public/Core/Util/ThreadPool.h"
#include "Public/Core/Util/Factory.h"

namespace GEngine {


	Entity::Entity(const uint64_t& id, const std::string& s)
	{
		go_hash = id;
		if (s.size() > 0) go_tag = s;
		transform = AddComponent<Transform>();
		GE_CORE_DEBUG("ENTITY HASH: {0}", Factory::HashToString(go_hash));
	}

	Entity::~Entity()
	{
		Factory::RemoveHash(go_hash);
		if (comp_hashes.size() > 0) {
			for (auto h : comp_hashes) {
				Factory::RemoveHash(h);
			}
		}
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

	uint64_t Entity::GetHash()
	{
		if (comp_hashes.size() > 0) {
			uint64_t h = comp_hashes.front();
			comp_hashes.pop_front();
			return h;
		}
		return Factory::NextHash();
	}

	void Entity::RemoveHash(const uint64_t& h)
	{
		Factory::RemoveHash(h);
	}

	void Entity::AddHash(const uint64_t& h)
	{
		comp_hashes.push_front(h);
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
