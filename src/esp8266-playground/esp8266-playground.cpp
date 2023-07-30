#include <commands.h>
#include <logger.h>
#include <mqtt.h>
#include <otaHandler.h>
#include <telemetry.h>
#include <utils.h>
#include <wifiHandler.h>

#include <Arduino.h>
#include <PubSubClient.h>
#include <Timer.h>

#include <string>

#ifndef CLIENT_NAME
    #error message "CLIENT_NAME is not defined"
#endif
#define CLIENT_NAME_FULL "ESP8266--plaground-" CLIENT_NAME

Timer t;
Logger logger("esp8266-playground.cpp");

void setup() {
    Serial.begin(115200);
    DebugPrintln("Booting");
    wifi::setup(CLIENT_NAME_FULL);
    telemetry::setup(CLIENT_NAME);
    mqtt::setup(CLIENT_NAME);
    commands::setup(CLIENT_NAME);
    ota::setup(CLIENT_NAME);
    logger.log("Setup completed");
}

void loop() {
    mqtt::loop();
    t.update();
}
