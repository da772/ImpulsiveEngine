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

		inline virtual Ref<Entity> GetEntity() {
			return entity.lock();
		}
		virtual void SetEntity(Weak<Entity>  e);

		inline virtual Entity* GetEntityPtr() {
			return entity.lock().get();
		}
		virtual void SetEntityPtr(Entity*  e);





		Vector3 GetEntityPosition();

		inline virtual void OnAttached(Ref<Entity> entity) { };
		inline virtual void DeAttached(Ref<Entity> entity) { };

		virtual Ref<Component> GetParentComponent() { return nullptr; };

		virtual Component* GetSelf() { return static_pointer_cast<Component>(self.lock()).get(); }
		virtual void SetSelf(Ref<Component> r) { }

		Component();
		virtual ~Component();
		bool m_hasComponentParent = false;
		static int refCount;
		
		inline const char* GetTag() { return m_tag; }
		inline void SetTag(const char* tag) { m_tag = tag; }

	protected:
		virtual void OnBegin() {};
		virtual void OnEnd() {};
		virtual void OnUpdate(Timestep timestep) {};
		
		const char* m_tag = "Default";
	
		Weak<Entity> entity;
		bool bInit = false;
		bool bUpdates = false;

	};
}
#endif;