#include "gepch.h"

#include "Public/Core/Application/Scene.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Util/ThreadPool.h"
#include "Public/Core/Application/SceneManager.h"
#include "Public/Core/Events/Event.h"




namespace GEngine {

	Scene::Scene(const char* id, Camera* camera) : id(id), camera(camera)
	{
		Load();
	}

	Scene::~Scene()
	{
		Unload();
		GE_CORE_DEBUG("SCENE DESTROYED");
	}

	GEngine::Camera* Scene::GetCamera() const
	{
		return camera;
	}

	void Scene::SetCamera(Camera* _camera)
	{
		camera = _camera;
	}

	Entity* Scene::DestroyEntity(Entity* actor) {
		entities.erase(actor->GetHash());
		delete actor;
		return nullptr;
	}

	void Scene::Begin()
	{
		if (!b_loaded)
			Load();
		b_init = true;
		OnBegin();
		for (std::pair<ObjectHash, Entity*> e : entities) {
			if (!e.second->IsInitialized())
				e.second->Begin();
		}
	}

	void Scene::Update(Timestep timestep)
	{
		if (b_init) {
			OnUpdate(timestep);
			for (std::pair<ObjectHash, Entity*> e : entities) {
                if (!e.second) {
                    GE_CORE_ASSERT(false, "ENTITY INVALID");
                    continue;
                }
				if (e.second->GetShouldUpdate())
					e.second->Update(timestep);
			}
			/* UPDATE COMPONENTS */

		}
	}

	void Scene::UnloadGraphics() {
		for (std::pair<ObjectHash, Entity*> e : entities) {
			e.second->UnloadGraphics();
		}
	}

	void Scene::ReloadGraphics() {
		for (std::pair<ObjectHash, Entity*> e : entities) {
			e.second->ReloadGraphics();
		}
	}

	void Scene::RemoveAllEntities() {
		for (std::pair<ObjectHash, Entity*> e : entities) {
			e.second->End();
		}
		for (std::pair<ObjectHash, Entity*> e : entities) {
			delete e.second;
		}

		entities.clear();
	}

	void Scene::End()
	{
		OnEnd();
		RemoveAllEntities();
		b_init = false;
		if (b_loaded)
			Unload();
	}

	bool Scene::IsLoaded()
	{
		return b_loaded;
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
		if (SceneManager::GetCurrentScene() == this) {
			b_paused = b;
			SceneManager::Pause(b_paused);
		}
	}


}
