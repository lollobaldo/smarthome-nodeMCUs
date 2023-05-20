#include "wifiHandler.h"

#include <utils.h>

#include <ota.h>
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

#ifndef AP_NAME
    #error "AP_NAME is not defined"
#endif

#ifndef AP_PSWD
    #define AP_PSWD "AP123456"
#endif

#define VERSION "0.0.1"
//#define RELEASE_URL "https://github.com/your_username/your_repo/releases/latest"
#define RELEASE_URL "http://192.168.0.4:8120/esp32playground/"

// WiFi connect timeout per AP. Increase when connecting takes longer.
const uint32_t connectTimeoutMs = 500;

namespace wifi {
    // const char* ssids[] = { WIFI_SSIDS };
    // const char* passwords[] = { WIFI_PSWS };

    // ESP8266WiFiMulti multiclient;
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
            DebugPrintln("WiFi connected");
            DebugPrintln("IP address: ");
            DebugPrintln(WiFi.localIP());
        }
    }

    void setup_OTA(const char* hostname) {
        init_ota(VERSION);
    }

    void setup(const char* hostname) {
        setup_wifi(hostname);
        setup_OTA(hostname);
    }

    void loop() {
        // ArduinoOTA.handle();
        handle_ota(RELEASE_URL);
    }
}
