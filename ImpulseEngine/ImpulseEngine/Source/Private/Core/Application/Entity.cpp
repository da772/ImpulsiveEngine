#include "gepch.h"

#include "Public/Core/Application/Entity.h"
#include "Public/Core/Application/Component.h"
#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Application/Components/TransformComponent.h"
#include "Public/Core/Application/Scene.h"
#include "Public/Core/Scripting/ScriptableComponent.h"
#include "Public/Core/Util/ThreadPool.h"
#include "Public/Core/Util/Factory.h"
#include "Public/Core/Scripting/ScriptObject.h"

namespace GEngine {

	Entity::Entity()
	{
		Entity::refCount++;
		transform = CreateGameObject<Transform>();
		AddComponent(transform);
	}

	Entity::Entity(Vector3f position)
	{
		Entity::refCount++;
		transform = CreateGameObject<Transform>();
		transform->SetPosition(position);
		AddComponent(transform);
	}

	int Entity::refCount = 0;

	Entity::~Entity()
	{
		End();
		Entity::refCount--;
        self.reset();
	}



	bool Entity::AddComponent(Ref<Component> component) {
		if (component) {
			if (component->m_hasComponentParent) {
				component = component->GetParentComponent();
			}

			if (components.find(component->hash) == components.end()) {
				Ref<Entity> e = std::static_pointer_cast<Entity>(self.lock());
				component->SetEntity(e);
				components[component->hash] = component;
				component->OnAttached(e);
				if (bInit) {
					component->Begin();
				}
				return true;
			}
			GE_CORE_ASSERT(false, "Component is NULL");
			return false;
		}
		GE_CORE_ASSERT(false, "Component already added");
		return false;
	}

	bool Entity::RemoveComponent(Ref<Component> component)
	{
		if (component != nullptr) {
			if (component->m_hasComponentParent) {
				component = component->GetParentComponent();
			}
			std::unordered_map<uint64_t, Ref<Component>>::iterator it = components.find(component->hash);
			if (it != components.end()) {
				component->End();
				component->DeAttached(static_pointer_cast<Entity>(self.lock()));
				ThreadPool::AddMainThreadFunction([this, it]() {components.erase(it); });
				return true;
			}
			GE_CORE_ASSERT(false, "Component is NULL");
			return false;
		}
		GE_CORE_ASSERT(false, "Invalid Component");
		return false;

	}

	std::vector<GEngine::Ref<GEngine::Component>> Entity::GetComponentsByTag(std::string tag)
	{
		std::vector<Ref<Component>> v;
		for (std::pair<uint64_t, Ref<Component>> c : components) {
			if (c.second->GetTag() == tag) {
				v.push_back(c.second);
			}
		}

		return v;
	}

	bool Entity::AddComponent_ptr(Component* component)
	{
		if (!component->GetSelf()) {
			component->hash = Factory::NextHash();
			Ref<Entity> e = std::static_pointer_cast<Entity>(self.lock());
			component->SetEntity(e);
			Ref<Component> comp = make_shared<Component>(*component);
			component->self = comp;
			return AddComponent(comp);
		}
		return AddComponent(static_pointer_cast<Component>(component->GetSelf()->self.lock()));
	}

	void Entity::UnloadGraphics() {
        OnUnloadGraphics();
		for (std::pair<uint64_t, Ref<Component>> c : components) {
			c.second->UnloadGraphics();
		}
	}

	void Entity::ReloadGraphics() {
        OnReloadGraphics();
		for (std::pair<uint64_t, Ref<Component>> c : components) {
			c.second->ReloadGraphics();
		}
	}

	bool Entity::RemoveComponent_ptr(Component* component)
	{
		return RemoveComponent(static_pointer_cast<Component>(component->GetSelf()->self.lock()));
	}


	void Entity::Begin() {
        bInit = true;
		OnBegin();
		for (std::pair<uint64_t, Ref<Component>> c : components) {
			c.second->Begin();
		}
	}

	void Entity::End() {
		if (bInit) {
			OnEnd();
			for (std::pair<uint64_t, Ref<Component>> c : components) {
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
		for (std::pair<uint64_t, Ref<Component>> c : components) {
			if (c.second->GetShouldUpdate()) {
				c.second->Update(timestep);
			}
		}
	}

	void Entity::Destroy() {
		End();
		if (scene.lock() != nullptr)
			scene.lock()->RemoveEntity(std::static_pointer_cast<Entity>(self.lock()));
		scene.reset();
	}

	void Entity::Clean()
	{
		for (std::pair<uint64_t, Ref<Component>> c : components) {
			c.second = nullptr;
		}
		components.clear();
		//scene.reset();
	}

	const Vector3f Entity::GetEntityPosition() {
		return transform->GetPosition();
	}

	const Vector3f Entity::GetEntityRotation() {
		return transform->GetRotation();
	}

	const Vector3f Entity::GetEntityScale() {
		return transform->GetScale();
	}

	void Entity::SetEntityPosition(const Vector3f& position)
	{
		TransformData transData = { transform->GetPosition(), transform->GetRotation(), transform->GetScale() };
		transform->SetPosition(position);
		for (std::pair<Weak<Component>, std::function<void(Ref<Transform>, TransformData)>> pair : m_TransformCallback) {
			pair.second(transform, transData);
		}
	}

	void Entity::SetEntityScale(const Vector3f& scale)
	{
		TransformData transData = { transform->GetPosition(), transform->GetRotation(), transform->GetScale() };
		transform->SetScale(scale);
		for (std::pair<Weak<Component>, std::function<void(Ref<Transform>, TransformData)>> pair : m_TransformCallback) {
			pair.second(transform, transData);
		}
	}

	void Entity::SetEntityRotation(const Vector3f& rot)
	{
		TransformData transData = { transform->GetPosition(), transform->GetRotation(), transform->GetScale() };
		transform->SetRotation(rot);
		for (std::pair<Weak<Component>, std::function<void(Ref<Transform>, TransformData)>> pair : m_TransformCallback) {
			pair.second(transform, transData);
		}
	}

	GEngine::Ref<ScriptVector3> Entity::GetEntityPositionScript()
	{
		Vector3f pos = transform->GetPosition();
		return make_shared<ScriptVector3>(pos);
	}

	GEngine::Ref<ScriptVector3> Entity::GetEntityRotationScript()
	{
		Vector3f pos = transform->GetRotation();
		return make_shared<ScriptVector3>(pos);
	}

	GEngine::Ref<ScriptVector3> Entity::GetEntityScaleScript()
	{
		Vector3f pos = transform->GetScale();
		return make_shared<ScriptVector3>(pos);
	}

	void Entity::SetEntityPositionScript(Ref<ScriptVector3> position)
	{
		SetEntityPosition(position->GetGlm());
	}

	void Entity::SetEntityScaleScript(Ref<ScriptVector3> scale)
	{
		SetEntityScale(scale->GetGlm());
	}

	void Entity::SetEntityRotationScript(Ref<ScriptVector3> rot)
	{
		SetEntityRotation(rot->GetGlm());
	}

	void Entity::UpdateComponentPosition()
	{
	}

	void Entity::AddTransformCallback(Ref<Component> component, std::function<void(Ref<Transform>, TransformData)> func)
	{
		m_TransformCallback[component] = func;
	}

	void Entity::RemoveTransformCallback(Ref<Component> component)
	{
		m_TransformCallback.erase(component);
	}

}
