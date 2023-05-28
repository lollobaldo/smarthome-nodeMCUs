#include "wifiHandler.h"

#include <utils.h>

#include <ota.h>
#include <Timer.h>
#include <WiFiManager.h>

#if defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>
#elif defined(ESP32)
    #include <WiFi.h>
    #include <WebServer.h>
#else
    #error "Unsupported platform"
#endif

#ifndef CLIENT_NAME
    #error message "CLIENT_NAME is not defined"
#endif

#ifndef AP_NAME
    #error "AP_NAME is not defined"
#endif

#ifndef AP_PSWD
    #define AP_PSWD "AP123456"
#endif

#ifndef VERSION
    #error "VERSION is not defined"
#endif

#define RELEASE_URL "https://github.com/lollobaldo/smarthome-nodeMCUs/releases/latest"

#define FIRMWARE_NAME "firmware_" CLIENT_NAME ".bin"
#define OTA_POOLING_INTERVAL 30000

namespace wifi {
    // const char* ssids[] = { WIFI_SSIDS };
    // const char* passwords[] = { WIFI_PSWS };

    // ESP8266WiFiMulti multiclient;
    Timer t;
    WiFiManager wm;

    void setup_wifi(const char* hostname) {
        // WiFi.mode(WIFI_STA);
        // WiFi.hostname(hostname);
        wm.setHostname(hostname);

        bool res;
        res = wm.autoConnect(AP_NAME, AP_PSWD); // password protected ap
        if(!res) {
            DebugPrintln("Failed to connect.");
            wm.resetSettings();
        } 
        else {
            DebugPrint("WiFi connected. IP address: ");
            DebugPrintln(WiFi.localIP());
        }
    }

    void handle_OTA() {
        handle_ota(RELEASE_URL);
    }

    void setup_OTA(const char* hostname) {
        init_ota(VERSION, FIRMWARE_NAME, true);
        t.every(OTA_POOLING_INTERVAL, handle_OTA, 5);
        DebugPrint("Started OTA pooling every ");
        DebugPrint(OTA_POOLING_INTERVAL);
        DebugPrintln("ms");
    }

    void setup(const char* hostname) {
        setup_wifi(hostname);
        setup_OTA(hostname);
    }

    void loop() {
        t.update();
    }
}
