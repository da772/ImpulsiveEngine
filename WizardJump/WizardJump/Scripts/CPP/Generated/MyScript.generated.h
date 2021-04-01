#pragma once
#include "reflection/reflection.hpp"
#include "../Scripts/TestScript.h"
class MyScript_Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("MyScript",{"MyScript",{
		{"id",{"id","int",static_cast<refl::store::uproperty_type>(5),refl::store::offsetOf(&MyScript::id) }}}, {
		{"MyScript",{"MyScript","MyScript",static_cast<refl::store::uproperty_type>(1),{{static_cast<::refl::store::uproperty_type>(2),"Entity*"} },[](void* ptr, std::vector<void*> args) {return (void*)new MyScript(*(Entity**)args[0]);} }},
		{"~MyScript",{"~MyScript","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) {MyScript* p = (MyScript*)ptr; delete p; return nullptr;} }},
		{"GetId",{"GetId","int",static_cast<refl::store::uproperty_type>(5),{},[](void* ptr, std::vector<void*> args) {int v = (*((MyScript*)ptr).*(&MyScript::GetId))(); int* _ptr = new int(v); return (void*)_ptr; } }},
		{"GetIdPtr",{"GetIdPtr","int*",static_cast<refl::store::uproperty_type>(2),{},[](void* ptr, std::vector<void*> args) {int* v = (*((MyScript*)ptr).*(&MyScript::GetIdPtr))(); return (void*)v; } }}}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("MyScript");
	}

};
