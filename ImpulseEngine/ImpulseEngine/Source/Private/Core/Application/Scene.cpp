#include "gepch.h"

#include "Public/Core/Application/Scene.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Util/ThreadPool.h"
#include "Public/Core/Application/SceneManager.h"
#include "Public/Core/Events/Event.h"


namespace GEngine {

	Scene::Scene(const char* id, Camera* camera) : id(id), camera(camera)
	{

	}

	Scene::~Scene()
	{
		Unload();
	}

	bool Scene::AddEntity(Ref<Entity> entity)
	{
		if (entity) {
			std::unordered_map<uint64_t, Ref<Entity> >::const_iterator it = entities.find(entity->hash);
			if (it == entities.end()) {
				entities[entity->hash] = entity;
				entity->SetScene(std::static_pointer_cast<Scene>(self.lock()));
				if (b_init) {
					entity->Begin();
				}
				return true;
			}
		}
		GE_CORE_ASSERT(false, "Invalid Entity");
		return false;
	}

	bool Scene::RemoveEntity(Ref<Entity> entity)
	{
		if (entity) {
			std::unordered_map<uint64_t, Ref<Entity> >::const_iterator it = entities.find(entity->hash);
			if (it != entities.end()) {
				entities.erase(it);
				entity.reset();
				entity = nullptr;
				return true; 
			}
			GE_CORE_ASSERT(false, "Entity not found");
			return false;
		}

		GE_CORE_ASSERT(false, "Invalid Entity");
		return false;
	}

	void Scene::Begin()
	{
		if (!b_loaded)
			Load();
		OnBegin();
		for (std::pair<u64, Ref<Entity>> e : entities) {
			e.second->Begin();
		}
		b_init = true;
	}

	void Scene::Update(Timestep timestep)
	{
		if (b_init) {
			OnUpdate(timestep);
			for (std::pair<u64, Ref<Entity>> e : entities) {
				if (e.second->GetShouldUpdate())
					e.second->Update(timestep);
			}
		}
	}

	void Scene::RemoveAllEntities() {
		for (std::pair<u64, Ref<Entity>> e : entities) {
			RemoveEntity(e.second);
		}
	}

	void Scene::End()
	{
		OnEnd();
		for (std::pair<u64, Ref<Entity>> e : entities) {
			e.second->End();
		}
		Clean();
		b_init = false;
		if (b_loaded)
			Unload();
	}

	bool Scene::IsLoaded()
	{
		return b_loaded;
	}

	void Scene::LoadAsync()
	{
	}

	void Scene::Load()
	{
		if (b_loaded)
			return;
		OnLoad();
		b_loaded = true;
	}

	void Scene::Unload()
	{
		if (!b_loaded)
			return;
		OnUnload();
		b_loaded = false;
	}

	void Scene::Pause(bool b)
	{
		if (SceneManager::GetCurrentScene() == this->self.lock()) {
			b_paused = b;
			SceneManager::Pause(b_paused);
		}
	}
	void Scene::Clean()
	{
		for (std::pair<u64, Ref<Entity>> e : entities) {
			e.second = nullptr;
		}
		entities.clear();
	}

}