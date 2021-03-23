#include "Logger.h"

std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

std::shared_ptr<spdlog::logger> Logger::SetupLogs(std::shared_ptr<spdlog::logger> l)
{
#ifdef GE_LOADED_DLL
    Logger::logger = l;
#endif
	return Logger::logger;
}

void Logger::DestroyLogs() {
    Logger::logger = nullptr;
}
