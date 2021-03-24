#pragma once
#include "reflection/reflection.hpp"
#include "../Scripts/Logger.h"
class Logger_Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("Logger",{"Logger",{
		{"log",{"log","lFunc",static_cast<refl::store::uproperty_type>(1),refl::store::offsetOf(&Logger::log) }}}, {
		{"Logger",{"Logger","Logger",static_cast<refl::store::uproperty_type>(1),{},[](void* ptr, std::vector<void*> args) {return (void*)new Logger();} }},
		{"~Logger",{"~Logger","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) {Logger* p = (Logger*)ptr; delete p; return nullptr;} }},
		{"SetFunctionPtr",{"SetFunctionPtr","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((Logger*)ptr).*(&Logger::SetFunctionPtr))(); return nullptr; } }},
		{"RegisterLog_Core",{"RegisterLog_Core","void",static_cast<refl::store::uproperty_type>(3),{{static_cast<::refl::store::uproperty_type>(1),"std::shared_ptr<spdlog::logger>"} },[](void* ptr, std::vector<void*> args) { (*((Logger*)ptr).*(&Logger::RegisterLog_Core))(*(std::shared_ptr<spdlog::logger>*)args[0]); return nullptr; } }},
		{"RegisterLog_Client",{"RegisterLog_Client","void",static_cast<refl::store::uproperty_type>(3),{{static_cast<::refl::store::uproperty_type>(1),"std::shared_ptr<spdlog::logger>"} },[](void* ptr, std::vector<void*> args) { (*((Logger*)ptr).*(&Logger::RegisterLog_Client))(*(std::shared_ptr<spdlog::logger>*)args[0]); return nullptr; } }},
		{"RemoveLog",{"RemoveLog","void",static_cast<refl::store::uproperty_type>(3),{{static_cast<::refl::store::uproperty_type>(1),"std::shared_ptr<spdlog::logger>"} },[](void* ptr, std::vector<void*> args) { (*((Logger*)ptr).*(&Logger::RemoveLog))(*(std::shared_ptr<spdlog::logger>*)args[0]); return nullptr; } }}}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("Logger");
	}
};
