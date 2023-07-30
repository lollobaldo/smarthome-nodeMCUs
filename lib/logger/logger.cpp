#include <telemetry.h>
#include <utils.h>

#include "logger.h"

#ifndef CLIENT_NAME
    #error message "CLIENT_NAME is not defined"
#endif

#define LOGS_CHANNEL "logs/" CLIENT_NAME

static const char* loglevel2String(LogLevel ll) {
    switch(ll) {
        case LogLevel::INFO: return "INF";
        case LogLevel::LOG: return "LOG";
        case LogLevel::WARN: return "WRN";
        case LogLevel::ERROR: return "ERR";
        default: return "";
    }
}

Logger::Logger(const char* p) {
    prefix = p;
};

void Logger::info(const char* message) {
    return log(LogLevel::INFO, message);
}

void Logger::log(const char* message) {
    return log(LogLevel::LOG, message);
}

void Logger::warn(const char* message) {
    return log(LogLevel::WARN, message);
}

void Logger::error(const char* message) {
    return log(LogLevel::ERROR, message);
}

void Logger::log(const LogLevel logLevel, const char* message) {
    const char* ll = loglevel2String(logLevel);
    DebugPrint(ll);
    DebugPrint(" [");
    DebugPrint(prefix);
    DebugPrint("] ");
    DebugPrintln(message);
    telemetry::sendLog(loglevel2String(logLevel), prefix, message);
}
