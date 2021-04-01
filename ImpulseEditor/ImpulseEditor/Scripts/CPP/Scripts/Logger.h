#pragma once
#include <GEngine.h>
#include "reflection/reflection.hpp"


UCLASS()
class Logger {
public:
	UCONSTRUCTOR()
	inline Logger() {};
	~Logger() {};

};

#ifdef GE_LOADED_DLL

#define GE_LOG_TRACE(...) GEngine::ScriptApi::NativeLog(0,__VA_ARGS__)
#define GE_LOG_DEBUG(...) GEngine::ScriptApi::NativeLog(1,__VA_ARGS__)
#define GE_LOG_INFO(...) GEngine::ScriptApi::NativeLog(2,__VA_ARGS__)
#define GE_LOG_WARN(...) GEngine::ScriptApi::NativeLog(3,__VA_ARGS__)
#define GE_LOG_ERROR(...) GEngine::ScriptApi::NativeLog(4,__VA_ARGS__)
#define GE_LOG_FATAL(...) GEngine::ScriptApi::NativeLog(5,__VA_ARGS__)

#endif
