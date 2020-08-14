#pragma once

#include "Public/Core/Application/Component.h"

namespace GEngine {

	class Entity;

	class ScriptableComponent : public Component {

	public:
		ScriptableComponent() { GE_CORE_DEBUG("CREATING SCRIPTABLE COMPONENT"); };
		~ScriptableComponent();

		Entity* GetParent();

		void SetParent(Entity* e);

		void SetParentComponent(Ref<Component> c);

		virtual Ref<Component> GetParentComponent() override;

		bool DoesUpdate() {
			return m_doesUpdate;
		}

		void SetDoesUpdate(bool b) {
			m_doesUpdate = b;
		}


		virtual void SetTag(std::string tag) override;

		virtual Component* GetSelf() override { return static_pointer_cast<Component>(m_parentComponent.lock()).get(); }
		virtual void SetSelf(Ref<Component> r) { }


		operator Ref<Component>() { return m_parentComponent.lock(); };

	private:
		bool m_doesUpdate = true;
		Entity* m_parentEntity;
		Weak<Component> m_parentComponent;





	};
}