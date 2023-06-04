#include "sensors.h"

#include "telemetry.h"
#include "Timer.h"
#include "utils.h"

#include <Arduino.h>

#include <algorithm>

Sensor::Sensor(const sensors::SensorSettings &s, Timer &timer) {
    DebugPrint("Creating sensor instance for ");
    DebugPrintln(s.sensorType);
    sensorType = s.sensorType;
    sensorId = s.sensorId;
    probingInterval = s.probingInterval;
    sensorPin = s.sensorPin;
    pinMode(sensorPin, INPUT);
    timer.every(probingInterval, [this](){ this->takeAndSendMeasurement(); });
};

void Sensor::probe(int* ret) {
    DebugPrint("Probing pin ");
    DebugPrintln(sensorPin);
    int values[5];
    for (int i = 0; i < 5; i++) {
        values[i] = analogRead(sensorPin);
        delay(100);
    }

    DebugPrint("Values: ");
    DebugPrint(values[0]);
    DebugPrint(", ");
    DebugPrint(values[1]);
    DebugPrint(", ");
    DebugPrint(values[2]);
    DebugPrint(", ");
    DebugPrint(values[3]);
    DebugPrint(", ");
    DebugPrintln(values[4]);

    ret[0] = (values[0] + values[1] + values[2]) / 3;
    ret[2] = (values[0] + values[1] + values[2] + values[3] + values[4]) / 5;

    std::sort(std::begin(values), std::begin(values) + 3);
    ret[1] = values[1];
    std::sort(std::begin(values), std::end(values));
    ret[3] = values[3];

    DebugPrint("Data: ");
    DebugPrint(ret[0]);
    DebugPrint(", ");
    DebugPrint(ret[1]);
    DebugPrint(", ");
    DebugPrint(ret[2]);
    DebugPrint(", ");
    DebugPrintln(ret[3]);
}

void Sensor::takeAndSendMeasurement() {
    int values[4] = {0};
    probe(values);
    Point point("sensors");
    point.addTag("sensorType", sensorType);
    point.addTag("sensorId", sensorId);
    point.addField("mean3", values[0]);
    point.addField("median3", values[1]);
    point.addField("mean5", values[2]);
    point.addField("median5", values[3]);
    telemetry::sendPoint(point);
}
