#pragma once
#ifndef GE_SCENE_DEFINE
#define GE_SCENE_DEFINE
#include "Public/Core/Core.h"
#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Application/GameObject.h"


namespace GEngine {

	class Camera;
	class Entity;
	class Event;

	class GE_API Scene {

	public:
		Scene(const char* id, Camera* camera);

		virtual ~Scene();

		inline const char* GetId() const { return id.c_str(); }
		inline void SetId(const char* _id) { }
		Camera* GetCamera() const;
		void SetCamera(Camera* _camera);
		template<typename E = Entity>
		inline E* CreateEntity(uint64_t entity = 0) {
			if (entity == 0) entity = Factory::NextHash();
			E* e = new E(entity);
			entities[entity] = e;
			if (b_init) {
				e->Begin();
			}
			return e;
		}
		Entity* DestroyEntity(Entity* actor);
		void Begin();
		void Update(Timestep timestep);
		void RemoveAllEntities();
		void End();
		bool IsLoaded();
		void Load();
		void Unload();
		void Pause(bool b);
		void UnloadGraphics();
		void ReloadGraphics();

		bool IsInitalized() { return b_init; }

		inline virtual void OnEvent(Event& e) {};
		inline virtual void OnUpdate(Timestep timestep) {};
		inline virtual void OnBegin() {};
		inline virtual void OnEnd() {};
		inline virtual void OnLoad() {};
		inline virtual void OnUnload() {};
		inline virtual void OnImGuiRender() {};

		inline virtual bool GetLoaded() { return b_loaded; }
		inline virtual void SetLoaded(bool bLoaded) {  }

		inline const std::unordered_map<uint64_t, Entity*>& GetEntities() const { return entities; }

		
	protected:
		std::string id;
		Camera* camera;
		bool b_loaded = false;
		bool b_init = false;
		std::unordered_map<uint64_t, Entity*> entities;
		bool b_paused = false;

	private:

	};





}

#endif
