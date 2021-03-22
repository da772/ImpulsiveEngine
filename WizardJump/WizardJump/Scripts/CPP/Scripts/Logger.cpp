#include "Logger.h"

#include <iostream>

std::string Logger::loggerName = "Native";
std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

std::shared_ptr<spdlog::logger> Logger::SetupLogs(std::vector<spdlog::sink_ptr> sinks)
{
#ifdef GE_LOADED_DLL
	if (sinks.size() > 0) {
		Logger::logger = std::make_shared<spdlog::logger>(loggerName, std::begin(sinks), std::end(sinks));
		spdlog::register_logger(Logger::logger);
	}
#endif
	return Logger::logger;
}

void Logger::DestroyLogs() {
    std::cout << "DESTROYING LOGS" << std::endl;
    spdlog::drop(loggerName);
    Logger::logger = nullptr;
}
