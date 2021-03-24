#include "Logger.h"

std::function<void(uint8_t, std::string)> Logger::s_log = nullptr;

void Logger::SetFunctionPtr() {
	if (s_log == nullptr) {
		s_log = log;
	}
	else {
		s_log = nullptr;
	}
}

void Logger::RegisterLog_Core(std::shared_ptr<spdlog::logger> logger)
{
	spdlog::register_logger(logger);
	logger->set_level(spdlog::level::trace);
	GEngine::Log::SetCoreLogger(logger);
}

void Logger::RegisterLog_Client(std::shared_ptr<spdlog::logger> logger)
{
	spdlog::register_logger(logger);
	logger->set_level(spdlog::level::trace);
	GEngine::Log::SetClientLogger(logger);
}


void Logger::RemoveLog(std::shared_ptr<spdlog::logger> logger)
{
	spdlog::drop(logger->name());
	GEngine::Log::SetClientLogger(nullptr);
	GEngine::Log::SetCoreLogger(nullptr);
}
