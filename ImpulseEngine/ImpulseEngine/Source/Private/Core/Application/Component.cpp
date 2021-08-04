#include "gepch.h"

#include "Public/Core/Application/Component.h"
#include "Public/Core/Application/Entity.h"


namespace GEngine {

	Component::Component(Entity* e) : GameObject(e->GetNextHash()), m_entity(e)
	{
		go_tag = "Component";
		is_component = true;
		GE_CORE_DEBUG("COMPONENT HASH: {0}", Factory::HashToString(go_hash));
	}

	Component::~Component()
	{
		
	}

	Entity* Component::GetEntity() const {
		return m_entity;
	}

	std::string Component::SerializeIndent(int _i) const
	{
		std::string indent = "";
		for (int i = 0; i < _i; i++) {
			indent += "\t";
		}
		return indent;
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
