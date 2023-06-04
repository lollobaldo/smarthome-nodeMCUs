#include "telemetry.h"
#include "utils.h"

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#ifndef INFLUXDB_TOKEN
    #error "INFLUXDB_TOKEN is not defined"
#endif

#ifndef INFLUXDB_ORG
    #error "INFLUXDB_ORG is not defined"
#endif

#define INFLUXDB_URL "https://eu-central-1-1.aws.cloud2.influxdata.com"
#define INFLUXDB_BUCKET "smarthome"

#define TZ_INFO "UTC1"

namespace telemetry {
    InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

    Point sensorPoint("sensors");
    Point getSensorPoint() {
        sensorPoint.clearTags();
        sensorPoint.clearFields();

        return sensorPoint;
    }

    void sendPoint(Point point) {
        DebugPrint("Writing: ");
        DebugPrintln(point.toLineProtocol());
        // Write point
        if (!client.writePoint(point)) {
            DebugPrint("InfluxDB write failed: ");
            DebugPrintln(client.getLastErrorMessage());
        }
    }

    void sendSensorValue(const String &name, float value, int decimalPlaces /*=2*/) {
        Point point = getSensorPoint();
        point.addTag("sensor", name);
        point.addField(name, value, decimalPlaces);
        sendPoint(point);
    }
    
    void sendSensorValue(const String &name, int value) {
        Point point = getSensorPoint();
        point.addTag("sensor", name);
        point.addField(name, value);
        sendPoint(point);
    }

    void sendSensorValue(const String &name, const String &value) {
        Point point = getSensorPoint();
        point.addTag("sensor", name);
        point.addField(name, value);
        sendPoint(point);
    }

    void setup(const char* clientName) {
        client.setInsecure();
        // Check server connection
        if (client.validateConnection()) {
            DebugPrint("Connected to InfluxDB: ");
            DebugPrintln(client.getServerUrl());
        } else {
            DebugPrint("InfluxDB connection failed: ");
            DebugPrintln(client.getLastErrorMessage());
        }
    }
}
