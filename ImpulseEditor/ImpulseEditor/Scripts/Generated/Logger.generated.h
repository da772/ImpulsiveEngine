#pragma once
#include "reflection/reflection.hpp"
#include "../Scripts/Logger.h"
class Logger_Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("Logger",{"Logger",{
}, {
		{"Logger",{"Logger","Logger",static_cast<refl::store::uproperty_type>(1),{},[](void* ptr, std::vector<void*> args) {return (void*)new Logger();} }},
		{"~Logger",{"~Logger","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) {Logger* p = (Logger*)ptr; delete p; return nullptr;} }}}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("Logger");
	}
};
