#pragma once

#include "Public/Core/Core.h"
#pragma warning(push, 0)   
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(push, 0)   
#include "Public/Core/ImGui/ImGuiSink.h"


namespace GEngine {

	class ImGuiAppLog;

	class GE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { 
			return s_CoreLogger;
		}
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { 
			return s_ClientLogger;
		}
        inline static std::shared_ptr<spdlog::logger> GetNativeLogger() {
            return s_NativeLogger;
        }
		inline static std::vector<spdlog::sink_ptr>& GetSinks() { return sinks; }
		inline static ImGuiAppLog* GetImGuiLog() { return s_ImGuiLog; }
		static void RegisterLog(std::shared_ptr < spdlog::logger > l);

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
        static std::shared_ptr<spdlog::logger> s_NativeLogger;
		static ImGuiAppLog* s_ImGuiLog;
		static std::vector<spdlog::sink_ptr> sinks;

	};


}

#ifndef GE_LOADED_DLL
// Core log macros
#define GE_CORE_TRACE(...)    ::GEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GE_CORE_DEBUG(...)    ::GEngine::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define GE_CORE_INFO(...)     ::GEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GE_CORE_WARN(...)     ::GEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GE_CORE_ERROR(...)    ::GEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GE_CORE_FATAL(...)    ::GEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define GE_LOG_TRACE(...)	      ::GEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GE_LOG_DEBUG(...)		  ::GEngine::Log::GetClientLogger()->debug(__VA_ARGS__)
#define GE_LOG_INFO(...)	      ::GEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define GE_LOG_WARN(...)	      ::GEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GE_LOG_ERROR(...)	      ::GEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define GE_LOG_FATAL(...)	      ::GEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)

#define GE_NATIVE_TRACE(...)          ::GEngine::Log::GetNativeLogger()->trace(__VA_ARGS__)
#define GE_NATIVE_DEBUG(...)          ::GEngine::Log::GetNativeLogger()->debug(__VA_ARGS__)
#define GE_NATIVE_INFO(...)          ::GEngine::Log::GetNativeLogger()->info(__VA_ARGS__)
#define GE_NATIVE_WARN(...)          ::GEngine::Log::GetNativeLogger()->warn(__VA_ARGS__)
#define GE_NATIVE_ERROR(...)          ::GEngine::Log::GetNativeLogger()->error(__VA_ARGS__)
#define GE_NATIVE_FATAL(...)          ::GEngine::Log::GetNativeLogger()->fatal(__VA_ARGS__)

#endif


#ifdef GE_LOADED_DLL
// Client log macros
#define GE_CORE_TRACE(...)    
#define GE_CORE_DEBUG(...)    
#define GE_CORE_INFO(...)     
#define GE_CORE_WARN(...)     
#define GE_CORE_ERROR(...)    
#define GE_CORE_FATAL(...) 
#endif
