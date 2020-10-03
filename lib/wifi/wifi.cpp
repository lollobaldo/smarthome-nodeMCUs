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

namespace wifi {
    const char* ssid = WIFI_SSID;
    const char* password = WIFI_PSW;

    WiFiClient client;

    void setup_wifi(const char* hostname) {
        delay(10);
        Serial.println();
        Serial.print("Connecting to ");
        Serial.println(ssid);
        WiFi.mode(WIFI_STA);
        WiFi.hostname(hostname);
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
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
