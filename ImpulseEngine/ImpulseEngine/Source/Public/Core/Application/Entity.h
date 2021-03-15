#pragma once
#ifndef GE_ENTITY_DEFINE
#define GE_ENTITY_DEFINE
#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Application/GameObject.h"
#include "Public/Core/Application/Scene.h"
#include "Public/Core/Application/SceneManager.h"
#include "entt.hpp"


namespace GEngine {

	class Scene;
	class Component;
	class Transform;
	class ScriptableComponent;
	struct TransformData;
	struct ScriptVector3;

	class GE_API Entity : public GameObject {

	public:
		Entity(const uint32_t&);
		virtual ~Entity();

		template<class C, typename ... Args>
		inline C* AddComponent(Args&& ... args) {
			entt::registry& r = SceneManager::GetCurrentScene()->GetRegistry();
			//r.emplace<C>((entt::entity)go_hash, std::forward<Args>(args)...);
			C* c = new C(std::forward<Args>(args)...);
			components.insert(c);
			if (bInit) {
				c->Begin();
			}
			return c;
		}


		template<class C>
		inline C* RemoveComponent(C* component)
		{
			if (bInit)
				component->End();
			components.erase(component);
			//entt::registry& r = SceneManager::GetCurrentScene()->GetRegistry();
			//r.remove<C>((entt::entity)go_hash);
			delete component;
			return nullptr;
		}

		void UnloadGraphics();
		void ReloadGraphics();
        
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


        const Vector3f GetPosition();
        const Vector3f GetRotation();
        const Vector3f GetScale();
		void SetPosition(const Vector3f& position);
		void SetScale(const Vector3f& scale);
		void SetRotation(const Vector3f& rot);

		void AddTransformCallback(Component*, std::function<void(Transform*, TransformData)> func);
		void RemoveTransformCallback(Component* c);
		void Destroy();

		inline const const std::set<Component*>& GetComponents() const { return components; }

		static int refCount;

	private:
		void Clean();
		std::unordered_map<Component*, std::function<void(Transform*, TransformData)>> m_TransformCallback;



	protected:
		std::set<Component*> components;
		Transform* transform;
		inline virtual void OnBegin() {};
		inline virtual void OnEnd() {}
		virtual void OnUpdate(Timestep timestep) {};
		bool bUpdates = true;
		bool bInit = false;




	};
}
#endif
