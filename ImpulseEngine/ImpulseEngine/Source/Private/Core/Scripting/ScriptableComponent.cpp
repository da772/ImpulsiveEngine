#include "gepch.h"
#include "Public/Core/Scripting/ScriptableComponent.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Application/Component.h"

namespace GEngine {

	ScriptableComponent::~ScriptableComponent()
	{
		GE_CORE_DEBUG("DESTROYING SCRIPTABLE COMPONENT");
	}

	GEngine::Entity* ScriptableComponent::GetParent()
	{
		return m_parentEntity;
	}

	void ScriptableComponent::SetParent(Entity* e)
	{	
		m_parentEntity = e;
	}

	void ScriptableComponent::SetParentComponent(Ref<Component> c)
	{
		c->SetTag(GetTag());
		m_parentComponent = c;
	}


}