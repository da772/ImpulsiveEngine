#pragma once
#ifndef GE_ENTITY_DEFINE
#define GE_ENTITY_DEFINE
#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Application/GameObject.h"


namespace GEngine {

	class Scene;
	class Component;
	class Transform;
	class ScriptableComponent;
	struct TransformData;
	struct ScriptVector3;

	class GE_API Entity : public GameObject {

	public:
		Entity(glm::vec3 position);
		virtual ~Entity();
		std::string m_tag = "Entity";
		template <class C = Component>
		inline std::vector<Ref<Component>> FindComponentOfType() {
			std::vector<Ref<Component>> c;
            for (std::pair<uint32_t, Ref<Component>> _c : components) {
                if (std::dynamic_pointer_cast<C>(_c.second) != nullptr) {
                    c.push_back(_c.second);
				}
			}
			return c;
		}

		bool AddComponent(Ref<Component> component);
		bool RemoveComponent(Ref<Component> component);

		std::vector<Ref<Component>> GetComponentsByTag(std::string tag);

		void UnloadGraphics();
		void ReloadGraphics();

		bool AddComponent_ptr(Component* component);
		bool RemoveComponent_ptr(Component* component);

		void Update(Timestep timestep);

		void Begin();

		void End();

		/**
		 * Warning: Should only be called by scene class
		 */
		inline void SetScene(Weak<Scene> scene) {
			this->scene = scene;
		}

		inline Ref<Scene> GetScene() {
			return this->scene.lock();
		}

		inline void SetShouldUpdate(bool b) {
			bUpdates = b;
		}

		inline bool GetShouldUpdate() {
			return bUpdates && bInit;
		}

		inline bool IsInitialized() {
			return bInit;
		}

		inline Ref<Transform> GetEntityTransformComponent() {
			return transform;
		};

        const Vector3 GetEntityPosition();
        const Vector3 GetEntityRotation();
        const Vector3 GetEntityScale();
		void SetEntityPosition(const Vector3& position);
		void SetEntityScale(const Vector3& scale);
		void SetEntityRotation(const Vector3& rot);

		Ref<ScriptVector3> GetEntityPositionScript();
		Ref<ScriptVector3> GetEntityRotationScript();
		Ref<ScriptVector3> GetEntityScaleScript();
		void SetEntityPositionScript(Ref<ScriptVector3> position);
		void SetEntityScaleScript(Ref<ScriptVector3> scale);
		void SetEntityRotationScript(Ref<ScriptVector3> rot);

		void AddTransformCallback(Ref<Component> c, std::function<void(Ref<Transform>, TransformData)> func);
		void RemoveTransformCallback(Ref<Component> c);

		void Destroy();
		

		Entity();

		inline const std::unordered_map<uint64_t, Ref<Component>>& GetComponents() const { return components; }

		static int refCount;

	private:
		void Clean();
		Ref<Transform> transform;
		std::unordered_map<Ref<Component>, std::function<void(Ref<Transform>, TransformData)>> m_TransformCallback;



	protected:
		void UpdateComponentPosition();
		
		Weak<Scene> scene;

		std::unordered_map<uint64_t, Ref<Component>> components;

		inline virtual void OnBegin() {};
		inline virtual void OnEnd() {}
		virtual void OnUpdate(Timestep timestep) {};
		bool bUpdates = true;
		bool bInit = false;




	};
}
#endif
