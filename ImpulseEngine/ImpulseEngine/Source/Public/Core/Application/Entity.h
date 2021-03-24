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
		Entity(const uint64_t&, const std::string& s = "");
		virtual ~Entity();

		template<class C, typename ... Args>
		inline C* AddComponent(Args&& ... args) {
			C* c = new C(this, std::forward<Args>(args)...);
			components.insert(c);
			if (bInit) {
				c->Begin();
			}
			return c;
		}


		template<class C>
		inline C* RemoveComponent(C* component)
		{
			if (components.find(component) != components.end()) {
				if (bInit)
					component->End();
				components.erase(component);
				delete component;
			}
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

		uint64_t GetHash();
		void RemoveHash(const uint64_t& h);
		void AddHash(const uint64_t& h);

		inline const std::set<Component*>& GetComponents() const { return components; }

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
		std::list<uint64_t> comp_hashes;




	};
}
#endif
