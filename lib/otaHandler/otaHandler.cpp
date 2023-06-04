#include "commands.h"

#include <ota.h>

#ifndef VERSION
    #error "VERSION is not defined"
#endif

#define RELEASE_URL "https://github.com/lollobaldo/smarthome-nodeMCUs/releases/latest"

#define FIRMWARE_NAME "firmware_" CLIENT_NAME ".bin"

namespace ota {
    void setup(const char* hostname) {
        init_ota(VERSION, FIRMWARE_NAME, true);
        commands::registerCommand("OTA", [](){handle_ota(RELEASE_URL);});
    }
}
