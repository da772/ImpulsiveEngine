#pragma once

#include "Public/Core/Core.h"
#pragma warning(push, 0)
#pragma GCC diagnostic push
#ifdef GE_PLATFORM_MACOSX
#pragma GCC diagnostic ignored "-Wdefaulted-function-deleted"
#endif
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma GCC diagnostic pop
#pragma warning(pop)
#ifdef GE_CONSOLE_APP
#include "Public/Core/ImGui/ImGuiSink.h"
#endif


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
		inline static void SetCoreLogger(std::shared_ptr<spdlog::logger> l) {
			s_CoreLogger = l;
		}
		inline static void SetClientLogger(std::shared_ptr<spdlog::logger> l) {
			s_ClientLogger = l;
		}

		inline static std::vector<spdlog::sink_ptr>& GetSinks() { return sinks; }
		#ifdef GE_CONSOLE_APP
		inline static ImGuiAppLog* GetImGuiLog() { return s_ImGuiLog; }
		#endif
		static void RegisterLog(std::shared_ptr < spdlog::logger > l);

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
        static std::shared_ptr<spdlog::logger> s_NativeLogger;
		#ifdef GE_CONSOLE_APP
		static ImGuiAppLog* s_ImGuiLog;
		#endif
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
#define GE_CORE_TRACE(...) GE_LOG_TRACE(...)   
#define GE_CORE_DEBUG(...) GE_LOG_DEBUG(...)   
#define GE_CORE_INFO(...)  GE_LOG_INFO(...)   
#define GE_CORE_WARN(...)  GE_LOG_WARN(...)
#define GE_CORE_ERROR(...) GE_LOG_ERROR(...)
#define GE_CORE_FATAL(...) GE_LOG_FATAL(...)
#endif
