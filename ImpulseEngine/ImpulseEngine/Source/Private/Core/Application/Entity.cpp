#include "gepch.h"

#include "Public/Core/Application/Entity.h"
#include "Public/Core/Application/Component.h"
#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Application/Components/TransformComponent.h"

#include "Public/Core/Util/ThreadPool.h"
#include "Public/Core/Util/Factory.h"

namespace GEngine {


	Entity::Entity(const ObjectHash& id, const std::string& s, const ObjectHash& trans) : GameObject(id)
	{
		is_component = false;
		if (s.size() > 0) go_tag = s;
		if (trans.isValid())
			transform = AddComponentHash<Transform>(trans);
		else 
			transform = AddComponent<Transform>();

	}

	Entity::~Entity()
	{
		Factory::RemoveHash(go_hash);
		if (comp_hashes.size() > 0) {
			for (const auto& h : comp_hashes) {
				Factory::RemoveHash(h);
			}
		}

	}

	void Entity::UnloadGraphics() {
        OnUnloadGraphics();
		for (const auto& c: components) {
			c.second->UnloadGraphics();
		}
	}

	void Entity::ReloadGraphics() {
        OnReloadGraphics();
		for (const auto& c : components) {
			c.second->ReloadGraphics();
		}
	}

	void Entity::AddChild(Entity* e)
	{
		if (e == this) return;
		if (isParent(e)) return;
		
		if (e) {
			if (e->GetParent()) {
				e->GetParent()->RemoveChild(e);	
			}

			m_Children[e->GetHash()] = e;
			e->m_parent = this;
			GetTransform()->AddTransformCallback(e->GetHash(), [e](Transform* trans, TransformData data) {
				e->GetTransform()->SetRelativePosition(trans->GetWorldPosition());
				e->GetTransform()->SetRelativeRotation(trans->GetWorldRotation());
				e->GetTransform()->SetRelativeScale(trans->GetWorldScale());
			});
			e->GetTransform()->SetRelativePosition(GetTransform()->GetWorldPosition());
			e->GetTransform()->SetRelativeRotation(GetTransform()->GetWorldRotation());
			e->GetTransform()->SetRelativeScale(GetTransform()->GetWorldScale());
		}
	}

	void Entity::SetParent(Entity* e)
	{
		if (e == this) return;
		if (e == GetParent()) return;
		if (e && e->GetParent() == this) return;
		if (GetParent() && !e) {
			GetParent()->RemoveChild(this);
			return;
		}
		e->AddChild(this);
	
	}

	void Entity::RemoveChild(Entity* e)
	{
		if (e == this) return;
		if (e->GetParent() == this) {
			GetTransform()->RemoveTransformCallback(e->GetHash());
			m_Children.erase(e->GetHash());
			e->m_parent = nullptr;
			e->GetTransform()->ResetRelativeTransform();
		}
	}



	void Entity::Begin() {
        bInit = true;
		OnBegin();
		for (const auto& c : components) {
			c.second->Begin();
		}
	}

	void Entity::End() {
		if (bInit) {
			OnEnd();
			for (const auto& c : components) {
                //c.second->DeAttached(nullptr);
				c.second->End();
			}
		}
		bInit = false;
		Clean();
	}

	void Entity::Update(Timestep timestep)
	{
		OnUpdate(timestep);
		for (const auto& c : components) {
			if (c.second->GetShouldUpdate()) {
				c.second->Update(timestep);
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
		for (const auto& p : m_Children) {
			p.second->Destroy();
		}
		End();
		SceneManager::GetCurrentScene()->DestroyEntity(this);
		
	}

	ObjectHash Entity::GetNextHash()
	{
		if (comp_hashes.size() > 0) {
			ObjectHash h = comp_hashes.front();
			comp_hashes.pop_front();
			return h;
		}
		return Factory::NextHash();
	}

	void Entity::RemoveHash(const ObjectHash& h)
	{
		Factory::RemoveHash(h);
	}

	void Entity::AddHash(const ObjectHash& h)
	{
		comp_hashes.push_front(h);
	}

	GEngine::ObjectHash Entity::PopHash()
	{
		if (comp_hashes.size() > 0) {
			ObjectHash h = comp_hashes.front();
			comp_hashes.pop_front();
			return h;
		}

		return ObjectHash();
	}

	void Entity::Clean()
	{
		//entt::registry& r = SceneManager::GetCurrentScene()->GetRegistry();
		//r.remove_all((entt::entity)go_hash);
		for (const auto& c : components) {
			if (c.second == transform) continue;
			delete c.second;
		}

		delete transform;
		transform = nullptr;
		components.clear();
	}

	bool Entity::isParent(Entity* e)
	{
		Entity* p = GetParent();

		while (p) {
			if (p == e) return true;
			p = p->GetParent();
		}

		return false;

	}

}
