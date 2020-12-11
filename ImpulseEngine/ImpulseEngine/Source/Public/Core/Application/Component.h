#pragma once
#ifndef GE_COMPONENT_DEFINE
#define GE_COMPONENT_DEFINE

#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Application/GameObject.h"

namespace GEngine {

	class Entity;

	class Component : public GameObject {


	public:
	
		void Begin();
		void Update(Timestep timestep);
		void End();

		virtual void UnloadGraphics() {};
		virtual void ReloadGraphics() {};

		inline bool IsInitialized() {
			return bInit;
		}

		inline void SetShouldUpdate(bool b) {
			bUpdates = b;
		}

		inline bool GetShouldUpdate() {
			return bUpdates && bInit;
		}

		Ref<Entity> GetEntity() {
			return entity.lock();
		}
		void SetEntity(Weak<Entity>  e);

		Entity* GetEntityPtr() {
			return entity.lock().get();
		}
		void SetEntityPtr(Entity*  e);




		Vector3f GetEntityPosition();

		virtual void OnAttached(Ref<Entity> entity) { };
		virtual void DeAttached(Ref<Entity> entity) { };

		Ref<Component> GetParentComponent() { return nullptr; };

		Component* GetSelf() { return static_pointer_cast<Component>(self.lock()).get(); }
		void SetSelf(Ref<Component> r) { }

		Component();
		virtual ~Component();
		bool m_hasComponentParent = false;
		static int refCount;
		
		const std::string& GetTag() { return m_tag; }
		void SetTag(const std::string& tag) { m_tag = tag; }
		std::string m_tag = "Component";
		void Destroy();

	protected:
		virtual void OnBegin() {};
		virtual void OnEnd() {};
		virtual void OnUpdate(Timestep timestep) {};
	
		Weak<Entity> entity;
		bool bInit = false;
		bool bUpdates = false;

	};
}
#endif