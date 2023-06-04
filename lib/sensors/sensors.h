#pragma once

#include "Timer.h"
#include "utils.h"

#include <Arduino.h>

namespace sensors {
    struct SensorSettings {
        String sensorType;
        String sensorId;
        int probingInterval;
        int sensorPin;
    };
}

class Sensor {
    public:
        Sensor(const sensors::SensorSettings &s, Timer &timer);
        void takeAndSendMeasurement();
    private:
        String sensorType;
        String sensorId;
        int probingInterval;
        int sensorPin;
        void probe(int* ret);
};
