#pragma once
#include "reflection/reflection.hpp"
#include "../Scripts/TestScript.h"
class TestScript_Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("TestScript",{"TestScript",{
}, {
		{"TestScript",{"TestScript","TestScript",static_cast<refl::store::uproperty_type>(1),{{static_cast<::refl::store::uproperty_type>(2),"Entity*"} },[](void* ptr, std::vector<void*> args) {return (void*)new TestScript(*(Entity**)args[0]);} }},
		{"~TestScript",{"~TestScript","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) {TestScript* p = (TestScript*)ptr; delete p; return nullptr;} }},
		{"UnloadGraphics",{"UnloadGraphics","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::UnloadGraphics))(); return nullptr; } }},
		{"ReloadGraphics",{"ReloadGraphics","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::ReloadGraphics))(); return nullptr; } }},
		{"OnBegin",{"OnBegin","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::OnBegin))(); return nullptr; } }},
		{"OnEnd",{"OnEnd","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::OnEnd))(); return nullptr; } }},
		{"OnUpdate",{"OnUpdate","void",static_cast<refl::store::uproperty_type>(3),{{static_cast<::refl::store::uproperty_type>(1),"Timestep"} },[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::OnUpdate))(*(Timestep*)args[0]); return nullptr; } }},
		{"TestFunc",{"TestFunc","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::TestFunc))(); return nullptr; } }},
		{"NewFunct",{"NewFunct","void",static_cast<refl::store::uproperty_type>(3),{{static_cast<::refl::store::uproperty_type>(5),"int"} },[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::NewFunct))(*(int*)args[0]); return nullptr; } }}}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("TestScript");
	}
};
