#include "gepch.h"
#include "Public/Core/Application/Log.h"

#pragma warning(push, 0)       
#ifdef GE_PLATFORM_ANDROID
#include "spdlog/sinks/android_sink.h"
#else
#include "spdlog/sinks/stdout_color_sinks.h"
#endif
#pragma warning(pop)


namespace GEngine {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
    std::shared_ptr<spdlog::logger> Log::s_NativeLogger;
#ifdef GE_CONSOLE_APP
	ImGuiAppLog* Log::s_ImGuiLog = new ImGuiAppLog();
#endif

	std::vector<spdlog::sink_ptr> Log::sinks;

	void Log::Init()
	{
#ifdef GE_PLATFORM_ANDROID
		sinks.push_back(std::make_shared<spdlog::sinks::android_sink_st>());
#else
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
#endif
#ifdef GE_CONSOLE_APP
		sinks.push_back(std::make_shared<ImGui_sink_mt>(*s_ImGuiLog));
#endif

		for (spdlog::sink_ptr s : sinks) {
			s->set_pattern("[%T:%e] %^%l%n: %v%$");
			s->set_level(spdlog::level::trace);
		}

		s_CoreLogger = std::make_shared<spdlog::logger>("GEngine", begin(sinks), end(sinks));
		s_ClientLogger = std::make_shared<spdlog::logger>("App", begin(sinks), end(sinks));
        s_NativeLogger = std::make_shared<spdlog::logger>("Native", begin(sinks), end(sinks));

		s_ClientLogger->set_level(spdlog::level::trace);
		s_CoreLogger->set_level(spdlog::level::trace);
        s_NativeLogger->set_level(spdlog::level::trace);

	}

	void Log::RegisterLog(std::shared_ptr < spdlog::logger > l)
	{
		spdlog::register_logger(l);
	}

}
