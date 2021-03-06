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

namespace logger {
    bool log(LogLevel ll, const char* t, const char* m, bool r) {
        const char* message = concat(loglevel2String(ll), m);
        return log(t, message, r);
    }

    bool log(LogLevel ll, const char* m) {
        const char* message = concat(loglevel2String(ll), m);
        return log(mqtt::cLog, message, true);
    }

    bool log(const char* m) {
        const char* message = concat(loglevel2String(LogLevel::INFO), m);
        return log(mqtt::cLog, message, true);
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
