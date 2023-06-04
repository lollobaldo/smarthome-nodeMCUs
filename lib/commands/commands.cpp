#include "commands.h"

#include <mqtt.h>
#include <utils.h>

#include <unordered_map>
#include <string>

namespace commands {
    constexpr const char* rebootCommand = "REBOOT";


    std::unordered_map<std::string, callbackPointer_t> callbacks;

    void reboot() {
        ESP.restart();
    }

    void handleCallback(const char* topic, const char* command) {
        DebugPrint("Executing command: ");
        DebugPrintln(command);
        std::string command_s(command);
        if (callbacks.find(command_s) == callbacks.end()) {
            DebugPrint("No callback for command ");
            DebugPrintln(command);
            return;
        }
        callbacks[command_s]();
    }

    void registerCommand(const char* command, callbackPointer_t callback) {
        DebugPrint("Registering command: ");
        DebugPrintln(command);
        std::string command_s(command);
        callbacks[command_s] = callback;
    }

    void setup(const char* clientName) {
        const char* commandsChannel = concat(clientName, "/commands");
        mqtt::subscribe(commandsChannel, &handleCallback);
        registerCommand(rebootCommand, &reboot);
    }
}
