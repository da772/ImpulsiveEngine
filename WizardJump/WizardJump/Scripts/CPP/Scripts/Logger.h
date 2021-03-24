#pragma once
#include <GEngine.h>


using lFunc = std::function<void(uint8_t, std::string)>;

UCLASS()
class Logger {
public:
	UCONSTRUCTOR()
	inline Logger() {};
	~Logger() {};
    
	UPROPERTY()
	lFunc log;
	
	UFUNCTION()
	void SetFunctionPtr();

	UFUNCTION()
	void RegisterLog_Core(std::shared_ptr<spdlog::logger> logger);
	UFUNCTION()
	void RegisterLog_Client(std::shared_ptr<spdlog::logger> logger);

	UFUNCTION()
	void RemoveLog(std::shared_ptr<spdlog::logger> logger);

	static std::function<void(uint8_t, std::string)> s_log;

};

#ifdef GE_LOADED_DLL

#define GE_LOG_TRACE(x) Logger::s_log(0,x)
#define GE_LOG_DEBUG(x) Logger::s_log(1,x)
#define GE_LOG_INFO(x) Logger::s_log(2,x)
#define GE_LOG_WARN(x) Logger::s_log(3,x)
#define GE_LOG_ERROR(x) Logger::s_log(4,x)
#define GE_LOG_FATAL(x) Logger::s_log(5,x)

#endif 
