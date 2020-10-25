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
const uint32_t connectTimeoutMs = 1000;

namespace wifi {
    const char* ssids[] = { WIFI_SSIDS };
    const char* passwords[] = { WIFI_PSWS };

    ESP8266WiFiMulti multiclient;

    void setup_wifi(const char* hostname) {
        delay(10);
        int networkCount = sizeof(ssids)/sizeof(ssids[0]);
        for (int i = 0; i < networkCount; i++) {
            multiclient.addAP(ssids[i], passwords[i]);
        }
        Serial.print("Connecting to wifi");
        WiFi.hostname(hostname);
        WiFi.mode(WIFI_STA);
        while (multiclient.run(connectTimeoutMs) != WL_CONNECTED) {
            delay(1000);
            Serial.print(".");
        }
        randomSeed(micros());
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
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
            Serial.println("Start updating " + type);
        });
        ArduinoOTA.onEnd([]() {
            Serial.println("\nEnd");
        });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        });
        ArduinoOTA.onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) {
                Serial.println("Auth Failed");
            } else if (error == OTA_BEGIN_ERROR) {
                Serial.println("Begin Failed");
            } else if (error == OTA_CONNECT_ERROR) {
                Serial.println("Connect Failed");
            } else if (error == OTA_RECEIVE_ERROR) {
                Serial.println("Receive Failed");
            } else if (error == OTA_END_ERROR) {
                Serial.println("End Failed");
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
