#pragma once
#include <GEngine.h>

UCLASS()
class Logger {
public:
	UCONSTRUCTOR()
	inline Logger() {};
	~Logger() {};


	UFUNCTION()
	std::shared_ptr<spdlog::logger> SetupLogs(std::vector<spdlog::sink_ptr> sinks);
    
    UFUNCTION()
    void DestroyLogs();

	static std::shared_ptr<spdlog::logger> logger;
private:
	static std::string loggerName;
	


};

#ifdef GE_LOADED_DLL

#define GE_LOG_TRACE(...)	   Logger::logger->trace(__VA_ARGS__)
#define GE_LOG_DEBUG(...)	   Logger::logger->debug(__VA_ARGS__)
#define GE_LOG_INFO(...)	   Logger::logger->info(__VA_ARGS__)	 
#define GE_LOG_WARN(...)	   Logger::logger->warn(__VA_ARGS__)	 
#define GE_LOG_ERROR(...)	   Logger::logger->error(__VA_ARGS__)
#define GE_LOG_FATAL(...)	   Logger::logger->fatal(__VA_ARGS__)


#endif 
