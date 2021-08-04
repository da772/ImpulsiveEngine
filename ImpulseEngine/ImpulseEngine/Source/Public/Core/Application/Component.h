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
		virtual std::string Serialize(int indent = 0) { return SerializeIndent(indent)+"<NULL></NULL>"; };
		void Destroy();
		Entity* GetEntity() const;

		inline ObjectHash IsAttatched() const { return bAttatched; };
		inline void SetAttatched(ObjectHash b) { bAttatched = b; }

	public:
		virtual void OnBegin() {};
		virtual void OnEnd() {};
		virtual void OnUpdate(Timestep timestep) {}

	protected:
		std::string SerializeIndent(int i) const;
	protected:
		Entity* m_entity;
		bool bInit = false;
		bool bUpdates = false;
		
		ObjectHash bAttatched;

	};
}
#endif