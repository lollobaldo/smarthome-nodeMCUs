#include <commands.h>
#include <logger.h>
#include <mqtt.h>
#include <otaHandler.h>
#include <telemetry.h>
#include <utils.h>
#include <wifiHandler.h>

#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>
#include <PubSubClient.h>
#include <Timer.h>

#include <string>

#ifndef CLIENT_NAME
    #error message "CLIENT_NAME is not defined"
#endif
#define CLIENT_NAME_FULL "ESP--sensors-" CLIENT_NAME

constexpr int probingInterval = 0.5*60*1000L; // 30 seconds
constexpr int dhtPin = D5;
constexpr int dhtType = DHT22;
constexpr const char* sensorId = "s1";

DHT_Unified dht(dhtPin, dhtType);

Timer timer;
Logger logger("sensors.cpp");

void probe_temperature() {
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    DebugPrint(F("Temperature: "));
    DebugPrint(event.temperature);
    DebugPrintln(F("°C"));
    Point point("sensors");
    point.addTag("sensorType", "temperature");
    point.addTag("sensorId", sensorId);
    point.addField("temperature", event.temperature);
    telemetry::sendPoint(point);
}

void probe_humidity() {
    sensors_event_t event;
    dht.humidity().getEvent(&event);
    DebugPrint(F("Humidity: "));
    DebugPrint(event.relative_humidity);
    DebugPrintln(F("%"));

    Point point("sensors");
    point.addTag("sensorType", "humidity");
    point.addTag("sensorId", sensorId);
    point.addField("humidity", event.relative_humidity);
    telemetry::sendPoint(point);
}

void setup() {
    Serial.begin(115200);
    DebugPrintln("Booting");
    wifi::setup(CLIENT_NAME_FULL);
    mqtt::setup(CLIENT_NAME);
    commands::setup(CLIENT_NAME);
    telemetry::setup(CLIENT_NAME);
    ota::setup(CLIENT_NAME);
    dht.begin();
    timer.every(probingInterval, probe_temperature);
    timer.every(probingInterval, probe_humidity);

    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println(F("------------------------------------"));
    Serial.println(F("Temperature Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
    Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
    Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
    Serial.println(F("------------------------------------"));
    // Print humidity sensor details.
    dht.humidity().getSensor(&sensor);
    Serial.println(F("Humidity Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
    Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
    Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
    Serial.println(F("------------------------------------"));
}

void loop() {
    mqtt::loop();
    timer.update();
}
