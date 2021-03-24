#include "Logger.h"

std::function<void(uint8_t, std::string)> Logger::s_log = nullptr;

void Logger::SetFunctionPtr() {
    if (s_log == nullptr) {
        s_log = log;
    } else {
        s_log = nullptr;
    }
}
