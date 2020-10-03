#include <mqtt.h>
#include <utils.h>

#include "logger.h"

char* loglevel2String(LogLevel ll) {
    switch(ll) {
        case LogLevel::INFO: return "INF: ";
        case LogLevel::LOG: return "LOG: ";
        case LogLevel::WARN: return "WRN: ";
        case LogLevel::ERROR: return "ERR: ";
        default: return "";
    }
}

namespace logger {
    bool log(LogLevel ll, const char* topic, const char* m) {
        const char* message = concat(loglevel2String(ll), m);
        return log(topic, message, true);
    }

    bool log(const char* topic, const char* message) {
        return log(topic, message, false);
    }

    bool log(const char* topic, const char* message, bool retained) {
        if (mqtt::client.connected()) {
            DebugPrint("[");
            DebugPrint(topic);
            DebugPrint("]");
            DebugPrintln(message);
            return mqtt::client.publish(topic, message, retained);
        } else {
            DebugPrintln("Client not connected");
            return false;
        }
    }
}
