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

	class GE_API Scene : public GameObject {

	public:
		Scene(const char* id, Camera* camera);

		virtual ~Scene();

		template <class C = Entity>
		inline std::vector<Ref<Entity>> FindComponentOfType() {
			std::vector<Ref<Entity>> c;
			for (std::pair<uint32_t, Ref<Entity>> _c : entities) {
                if (dynamic_cast<C*>(_c.second.get()) != nullptr) {
                    c.push_back(_c.second);
				}
			}
			return c;
		}
		inline const char* GetId() const { return id.c_str(); }
		inline void SetId(const char* _id) { }
		inline Camera* GetCamera() const { return camera; }
		inline void SetCamera(Camera* _camera) { camera = _camera; }
		bool AddEntity(Ref<Entity> actor);
		bool RemoveEntity(Ref<Entity> actor);
		void Begin();
		void Update(Timestep timestep);
		void RemoveAllEntities();
		void End();
		bool IsLoaded();
		void LoadAsync();
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

		
	protected:
		std::string id;
		Camera* camera;
		bool b_loaded = false;
		bool b_init = false;
		std::unordered_map<uint64_t, Ref<Entity>> entities;
		bool b_paused = false;

	private:
		void Clean();

	};





}

#endif
