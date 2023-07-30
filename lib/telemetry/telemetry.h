#pragma once

#include <InfluxDbClient.h>

#include <Arduino.h>

namespace telemetry {

    void sendPoint(Point point);
    void sendLog(const String &logLevel, const String &module, const String &msg);
    void sendSensorValue(const String &name, float value, int decimalPlaces = 2);
    void sendSensorValue(const String &name, int value);
    void sendSensorValue(const String &name, const String &value);

    void setup(const char* clientName);
}
