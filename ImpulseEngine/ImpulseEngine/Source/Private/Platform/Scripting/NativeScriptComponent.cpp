#include "gepch.h"
#include "Public/Platform/Scripting/NativeScriptComponent.h"


namespace GEngine {

	NativeScriptComponent::NativeScriptComponent(Entity* e, const std::string& clazz) : Component(e), object(ScriptApi::GetReflector_Native()->CreateUClass(clazz, e))
	{
		component = (Component*)object.data();
		bUpdates = component->GetDoesUpdate();
	}

	NativeScriptComponent::~NativeScriptComponent()
	{
		ScriptApi::GetReflector_Native()->DestroyUClass(object);
	}

	void NativeScriptComponent::UnloadGraphics()
	{
		component->UnloadGraphics();
	}

	void NativeScriptComponent::ReloadGraphics()
	{
		component->ReloadGraphics();
	}

	void NativeScriptComponent::OnBegin()
	{
		component->Begin();
	}

	void NativeScriptComponent::OnEnd()
	{
		component->End();
	}

	void NativeScriptComponent::OnUpdate(Timestep timestep)
	{
		component->Update(timestep);
	}

}