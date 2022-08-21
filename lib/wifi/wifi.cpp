#include <utils.h>

#include "wifi.h"

#ifndef WIFI_SSID
    #define WIFI_SSID ""
#endif

#ifndef WIFI_PSW
    #define WIFI_PSW ""
#endif

#ifndef OTA_PSW
    #define OTA_PSW ""
#endif

// WiFi connect timeout per AP. Increase when connecting takes longer.
const uint32_t connectTimeoutMs = 500;

namespace wifi {
    // const char* ssids[] = { WIFI_SSIDS };
    // const char* passwords[] = { WIFI_PSWS };

    // ESP8266WiFiMulti multiclient;
    WiFiManager wm;

    void setup_wifi(const char* hostname) {
        WiFi.mode(WIFI_STA);
        WiFi.hostname(hostname);
        wm.setHostname(hostname);

        bool res;
        res = wm.autoConnect("AutoConnectAP","nodemcu"); // password protected ap
        if(!res) {
            DebugPrintln("Failed to connect.");
            // ESP.restart();
        } 
        else {
            DebugPrintln("WiFi connected");
            DebugPrintln("IP address: ");
            DebugPrintln(WiFi.localIP());
        }
    }

    void setup_OTA(const char* hostname) {
        ArduinoOTA.setHostname(hostname);

        ArduinoOTA.setPassword(OTA_PSW);

        ArduinoOTA.onStart([]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH) {
                type = "sketch";
            } else { // U_FS
                type = "filesystem";
            }
            DebugPrintln("Start updating " + type);
        });
        ArduinoOTA.onEnd([]() {
            DebugPrintln("\nEnd");
        });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            // DebugPrintln("Progress: %u%%\r", (progress / (total / 100)));
        });
        ArduinoOTA.onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) {
                DebugPrintln("Auth Failed");
            } else if (error == OTA_BEGIN_ERROR) {
                DebugPrintln("Begin Failed");
            } else if (error == OTA_CONNECT_ERROR) {
                DebugPrintln("Connect Failed");
            } else if (error == OTA_RECEIVE_ERROR) {
                DebugPrintln("Receive Failed");
            } else if (error == OTA_END_ERROR) {
                DebugPrintln("End Failed");
            }
        });
        ArduinoOTA.begin();
    }

    void setup(const char* hostname) {
        setup_wifi(hostname);
        setup_OTA(hostname);
    }

    void loop() {
        ArduinoOTA.handle();
    }
}
