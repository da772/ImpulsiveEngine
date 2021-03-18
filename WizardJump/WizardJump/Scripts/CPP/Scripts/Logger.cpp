#include "Logger.h"


std::string Logger::loggerName = "Native";
std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

std::shared_ptr<spdlog::logger> Logger::SetupLogs(std::vector<spdlog::sink_ptr> sinks)
{
#ifdef GE_HOT_RELOAD
	if (sinks.size() > 0) {
		Logger::logger = std::make_shared<spdlog::logger>(loggerName, std::begin(sinks), std::end(sinks));
		spdlog::register_logger(Logger::logger);
	}
#endif
	return Logger::logger;
}
