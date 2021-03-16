#include "gepch.h"
#include "Public/Platform/Scripting/NativeScriptComponent.h"


namespace GEngine {

	NativeScriptComponent::NativeScriptComponent(Entity* e, const std::string& clazz) : Component(e), object(ScriptApi::CreateObject_Native(clazz, e))
	{

	}

	NativeScriptComponent::~NativeScriptComponent()
	{
		ScriptApi::DestroyObject_Native(object);
	}

	void NativeScriptComponent::UnloadGraphics()
	{
		try {
			object.CallFunction<void>("UnloadGraphics");
		}
		catch (std::exception& e) {

		}
	}

	void NativeScriptComponent::ReloadGraphics()
	{
		try {
			object.CallFunction<void>("ReloadGraphics");
		}
		catch (std::exception& e) {

		}
	}

	void NativeScriptComponent::OnBegin()
	{
		try {
			object.CallFunction<void>("OnBegin");
		}
		catch (std::exception& e) {

		}
	}

	void NativeScriptComponent::OnEnd()
	{
		try {
			object.CallFunction<void>("OnEnd");
		}
		catch (std::exception& e) {

		}
	}

	void NativeScriptComponent::OnUpdate(Timestep timestep)
	{
		try {
			object.CallFunction<void>("OnUpdate", timestep);
		}
		catch (std::exception& e) {

		}
	}

}