#pragma once
#ifndef GE_ENTITY_DEFINE
#define GE_ENTITY_DEFINE
#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Application/GameObject.h"
#include "Public/Core/Application/Scene.h"
#include "Public/Core/Application/SceneManager.h"


namespace GEngine {

	class Scene;
	class Component;
	class Transform;
	class ScriptableComponent;
	struct TransformData;
	struct ScriptVector3;

	class GE_API Entity : public GameObject {

	public:
		Entity(const ObjectHash&, const std::string& s = "");
		virtual ~Entity();

		template<class C, typename ... Args>
		inline C* AddComponent(Args&& ... args) {
			C* c = new C(this, std::forward<Args>(args)...);
			components[c->GetHash()] = c;
			if (bInit) {
				c->Begin();
			}
			return c;
		}


		template<class C, typename ... Args>
		inline C* AddComponentHash(const ObjectHash& hash, Args&& ... args) {
			AddHash(hash);
			C* c = new C(this, std::forward<Args>(args)...);
			components[hash] = c;
			if (bInit) {
				c->Begin();
			}
			return c;
		}


		template<class C>
		inline C* RemoveComponent(C* component)
		{
			if (components.find(component->GetHash()) != components.end()) {
				if (bInit)
					component->End();
				components.erase(component->GetHash());
				delete component;
			}
			return nullptr;
		}

		void UnloadGraphics();
		void ReloadGraphics();

		inline Transform* GetTransform() const { return transform; }
		void AddChild(Entity* e);
		void RemoveChild(Entity* e);
		void SetParent(Entity* e);
		inline const std::unordered_map<ObjectHash, Entity*>& GetChildren() const { return m_Children; }
		inline Entity* GetParent() const { return m_parent; }
        
        virtual void OnUnloadGraphics() {};
        virtual void OnReloadGraphics() {};

		void Update(Timestep timestep);
		void Begin();
		void End();

		inline void SetShouldUpdate(bool b) {
			bUpdates = b;
		}

		inline bool GetShouldUpdate() {
			return bUpdates && bInit;
		}

		inline bool IsInitialized() {
			return bInit;
		}

		
		void Destroy();

		ObjectHash GetNextHash();
		void RemoveHash(const ObjectHash& h);
		void AddHash(const ObjectHash& h);

		inline const std::unordered_map<ObjectHash, Component*>& GetComponents() const { return components; }

	private:
		void Clean();
		bool isParent(Entity* e);
		
		


	protected:
		std::unordered_map<ObjectHash, Component*> components;
		std::unordered_map<ObjectHash, Entity*> m_Children;
		Entity* m_parent = nullptr;
		Transform* transform;
		inline virtual void OnBegin() {};
		inline virtual void OnEnd() {}
		virtual void OnUpdate(Timestep timestep) {};
		bool bUpdates = true;
		bool bInit = false;
		std::list<ObjectHash> comp_hashes;




	};
}
#endif
