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

namespace wifi {
    WiFiManager wm;

    IPAddress ip() {
        return WiFi.localIP();
    }

    String mac() {
        return WiFi.macAddress();
    }

    void setup(const char* hostname) {
        wm.setHostname(hostname);

        bool res;
        res = wm.autoConnect(AP_NAME, AP_PSWD);
        if(!res) {
            DebugPrintln("Failed to connect.");
            wm.resetSettings();
        } 
        else {
            DebugPrint("WiFi connected. IP address: ");
            DebugPrintln(WiFi.localIP());
        }
    }
}
