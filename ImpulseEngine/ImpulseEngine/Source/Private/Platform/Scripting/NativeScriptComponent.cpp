#include "gepch.h"
#include "Public/Platform/Scripting/NativeScriptComponent.h"


namespace GEngine {

	NativeScriptComponent::NativeScriptComponent(Entity* e, const std::string& clazz) : Component(e), object(ScriptApi::GetReflector_Native()->CreateUClass(clazz, e))
	{
		component = (Component*)object.data();
        if (component) {
            isValid = true;
        }
		bUpdates = component->GetDoesUpdate();
	}

	NativeScriptComponent::~NativeScriptComponent()
	{
		ScriptApi::GetReflector_Native()->DestroyUClass(object);
        GE_CORE_DEBUG("NATIVE SCRIPT DESTROYED");
	}

	void NativeScriptComponent::UnloadGraphics()
	{
        if (isValid)
            component->UnloadGraphics();
	}

	void NativeScriptComponent::ReloadGraphics()
	{
        if (isValid)
            component->ReloadGraphics();
	}

	void NativeScriptComponent::OnBegin()
	{
        if (isValid)
            component->Begin();
	}

	void NativeScriptComponent::OnEnd()
	{
        if (isValid) {
            GE_CORE_DEBUG("NATIVE SCRIPT ON END");
            component->End();
        }
	}

	void NativeScriptComponent::OnUpdate(Timestep timestep)
	{
        if (isValid)
            component->Update(timestep);
	}

}
