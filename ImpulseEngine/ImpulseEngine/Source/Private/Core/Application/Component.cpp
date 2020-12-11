#include "gepch.h"

#include "Public/Core/Application/Component.h"
#include "Public/Core/Application/Entity.h"


namespace GEngine {

	int Component::refCount = 0;

	void Component::Destroy()
	{
		entity.lock()->RemoveComponent(static_pointer_cast<GEngine::Component>(self.lock()));
	}

	void Component::SetEntity(Weak<Entity> e)
	{
		if (entity.lock() == nullptr) {
			this->entity = e;
			return;
		}
		//GE_CORE_ASSERT(false, "Entity already set");
	}

	void Component::SetEntityPtr(Entity* e)
	{
		if (entity.lock() == nullptr) {
			this->entity = Ref<Entity>(e);
			return;
		}

		GE_CORE_ASSERT(false, "Entity already set");
	}


	
	void Component::Begin()
	{
		if (bInit)
			return;
		GE_CORE_ASSERT(!bInit, "Component already initialized");
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

	GEngine::Vector3f Component::GetEntityPosition()
	{
		return entity.lock()->GetEntityPosition();
	}

	Component::Component()
	{
		Component::refCount++;
	}

	Component::~Component()
	{
		End();
		Component::refCount--;
	}

}
