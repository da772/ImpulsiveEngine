#include "gepch.h"

#include "Public/Core/Application/Component.h"
#include "Public/Core/Application/Entity.h"


namespace GEngine {

	Component::Component(Entity* e) : GameObject(e->GetHash()), m_entity(e)
	{
		go_tag = "Component";
		GE_CORE_DEBUG("COMPONENT HASH: {0}", Factory::HashToString(go_hash));
	}

	Component::~Component()
	{
		m_entity->RemoveHash(go_hash);
	}

	Entity* Component::GetEntity() const {
		return m_entity;
	}

	void Component::Destroy()
	{
		m_entity->RemoveComponent(this);
	}
	
	void Component::Begin()
	{
		if (bInit)
			return;
		//GE_CORE_ASSERT(!bInit, "Component already initialized");
		bInit = true;
        OnBegin();

	}

	void Component::Update(Timestep timestep)
	{
		OnUpdate(timestep);
	}

	void Component::End()
	{
		if (bInit)
			OnEnd();
		bInit = false;
	}




}
