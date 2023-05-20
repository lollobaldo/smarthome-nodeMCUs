#include <mqtt.h>
#include <utils.h>

#include "logger.h"

static char* loglevel2String(LogLevel ll) {
    switch(ll) {
        case LogLevel::INFO: return "INF: ";
        case LogLevel::LOG: return "LOG: ";
        case LogLevel::WARN: return "WRN: ";
        case LogLevel::ERROR: return "ERR: ";
        default: return "";
    }
}

Logger::Logger(const char* t) {
    topic = t;
};

void Logger::log(LogLevel ll, const char* m, bool r) {
    const char* message = concat(loglevel2String(ll), m);
    return log(message, r);
}

void Logger::log(LogLevel ll, const char* m) {
    const char* message = concat(loglevel2String(ll), m);
    return log(message, true);
}

void Logger::info(const char* m) {
    const char* message = concat(loglevel2String(LogLevel::INFO), m);
    return log(message, true);
}

void Logger::log(const char* m) {
    const char* message = concat(loglevel2String(LogLevel::LOG), m);
    return log(message, true);
}

void Logger::warn(const char* m) {
    const char* message = concat(loglevel2String(LogLevel::WARN), m);
    return log(message, true);
}

void Logger::error(const char* m) {
    const char* message = concat(loglevel2String(LogLevel::ERROR), m);
    return log(message, true);
}

void Logger::log(const char* message, bool retained) {
    if (mqtt::client.connected()) {
        DebugPrint("[");
        DebugPrint(topic);
        DebugPrint("]");
        DebugPrintln(message);
        mqtt::client.publish(topic, message, retained);
    } else {
        DebugPrintln("Client not connected");
    }
}

// // Deprecated. Backward compatibility only
// namespace logger {
//     bool log(LogLevel ll, const char* t, const char* m, bool r) {
//         const char* message = concat(loglevel2String(ll), m);
//         return log(t, message, r);
//     }

//     bool log(LogLevel ll, const char* m) {
//         const char* message = concat(loglevel2String(ll), m);
//         return log(mqtt::cLog, message, true);
//     }

//     bool log(const char* m) {
//         const char* message = concat(loglevel2String(LogLevel::INFO), m);
//         return log(mqtt::cLog, message, true);
//     }

//     bool log(const char* topic, const char* message, bool retained) {
//         if (mqtt::client.connected()) {
//             DebugPrint("[");
//             DebugPrint(topic);
//             DebugPrint("]");
//             DebugPrintln(message);
//             return mqtt::client.publish(topic, message, retained);
//         } else {
//             DebugPrintln("Client not connected");
//             return false;
//         }
//     }
// }
