#include <commands.h>
#include <logger.h>
#include <mqtt.h>
#include <otaHandler.h>
#include <sensors.h>
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
#define CLIENT_NAME_FULL "ESP--plants-" CLIENT_NAME

constexpr int probingTime = 0.5*60*1000L; // 30 seconds
constexpr int sensorPin = A0;

const sensors::SensorSettings sensorSettings = {"soil_moisture", "p1", probingTime, sensorPin};

// constexpr const char* channelOutput = "plants";


constexpr int maxVoltage = 3.0;

// esp analogReads returns 0-1023 => 3.0 : maxVoltageMultiplier = 3.3 : 1023
constexpr float maxVoltageMultiplier = maxVoltage * 1023 / 3.3;

Timer t;
Logger logger("plants.cpp");
const int ledPin = D4;

Sensor sensor({"soil_moisture", "p1", probingTime, sensorPin}, t);

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);
    pinMode(sensorPin, OUTPUT);
    DebugPrintln("Booting");
    wifi::setup(CLIENT_NAME_FULL);
    mqtt::setup(CLIENT_NAME);
    commands::setup(CLIENT_NAME);
    telemetry::setup(CLIENT_NAME);
    ota::setup(CLIENT_NAME);
    t.oscillate(ledPin, 3000, HIGH);
}

void loop() {
    mqtt::loop();
    t.update();
}
