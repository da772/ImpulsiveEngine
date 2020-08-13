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

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		inline static ImGuiAppLog* GetImGuiLog() { return s_ImGuiLog; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static ImGuiAppLog* s_ImGuiLog;
		static std::vector<spdlog::sink_ptr> sinks;

	};


}
#ifndef GE_DIST
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
#else
#define GE_CORE_TRACE(...)    
#define GE_CORE_DEBUG(...)    
#define GE_CORE_INFO(...)     
#define GE_CORE_WARN(...)     
#define GE_CORE_ERROR(...)    
#define GE_CORE_FATAL(...)    

// Client log macros
#define GE_LOG_TRACE(...)	  
#define GE_LOG_DEBUG(...)	
#define GE_LOG_INFO(...)	  
#define GE_LOG_WARN(...)	  
#define GE_LOG_ERROR(...)	  
#define GE_LOG_FATAL(...)	  
#endif
