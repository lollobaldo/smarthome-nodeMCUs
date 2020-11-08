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
    const char* ssids[] = { WIFI_SSIDS };
    const char* passwords[] = { WIFI_PSWS };

    ESP8266WiFiMulti multiclient;

    void setup_wifi(const char* hostname) {
        int networkCount = sizeof(ssids)/sizeof(ssids[0]);
        for (int i = 0; i < networkCount; i++) {
            multiclient.addAP(ssids[i], passwords[i]);
        }
        DebugPrintln("Connecting to wifi");
        WiFi.hostname(hostname);
        WiFi.mode(WIFI_STA);
        while (multiclient.run(connectTimeoutMs) != WL_CONNECTED) {
            delay(500);
            DebugPrint(".");
        }
        randomSeed(micros());
        DebugPrintln("");
        DebugPrintln("WiFi connected");
        DebugPrintln("IP address: ");
        DebugPrintln(WiFi.localIP());
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
