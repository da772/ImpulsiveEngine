#pragma once
#include "reflection/reflection.hpp"
#include "../Scripts/ExampleScript.h"
class ExampleScript_Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("ExampleScript",{"ExampleScript",{
}, {
		{"ExampleScript",{"ExampleScript","ExampleScript",static_cast<refl::store::uproperty_type>(1),{},[](void* ptr, std::vector<void*> args) {return (void*)new ExampleScript();} }},
		{"~ExampleScript",{"~ExampleScript","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) {ExampleScript* p = (ExampleScript*)ptr; delete p; return nullptr;} }},
		{"TestFunction",{"TestFunction","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) {ExampleScript::TestFunction(); return nullptr; } }}}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("ExampleScript");
	}
};
