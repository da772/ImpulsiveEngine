#pragma once
#include "reflection/reflection.hpp"
#include "../Scripts/Logger.h"
class Logger_Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("Logger",{"Logger",{
}, {
		{"Logger",{"Logger","Logger",static_cast<refl::store::uproperty_type>(1),{},[](void* ptr, std::vector<void*> args) {return (void*)new Logger();} }},
		{"~Logger",{"~Logger","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) {Logger* p = (Logger*)ptr; delete p; return nullptr;} }},
		{"SetupLogs",{"SetupLogs","std::shared_ptr<spdlog::logger>",static_cast<refl::store::uproperty_type>(1),{{static_cast<::refl::store::uproperty_type>(1),"std::vector<spdlog::sink_ptr>"} },[](void* ptr, std::vector<void*> args) {std::shared_ptr<spdlog::logger> v = (*((Logger*)ptr).*(&Logger::SetupLogs))(*(std::vector<spdlog::sink_ptr>*)args[0]); std::shared_ptr<spdlog::logger>* _ptr = new std::shared_ptr<spdlog::logger>(v); return (void*)_ptr; } }}}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("Logger");
	}
};
