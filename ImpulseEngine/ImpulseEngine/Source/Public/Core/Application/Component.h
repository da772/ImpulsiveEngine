#pragma once
#ifndef GE_COMPONENT_DEFINE
#define GE_COMPONENT_DEFINE

#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Application/GameObject.h"

namespace GEngine {

	class Entity;

	class GE_API Component : public GameObject {


	public:
		Component(Entity* e);
		virtual ~Component();

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
		inline bool GetDoesUpdate() const {
			return bUpdates;
		}
		void Destroy();
		Entity* GetEntity() const;

	public:
		virtual void OnBegin() {};
		virtual void OnEnd() {};
		virtual void OnUpdate(Timestep timestep) {};
	protected:
		Entity* m_entity;
		bool bInit = false;
		bool bUpdates = false;
		bool is_component = true;

	};
}
#endif