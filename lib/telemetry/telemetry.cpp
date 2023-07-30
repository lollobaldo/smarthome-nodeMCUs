#include "telemetry.h"
#include "utils.h"
#include "wifiHandler.h"

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
    const char* clientName;

    Point logPoint("logs");
    Point getLogPoint() {
        logPoint.clearTags();
        logPoint.clearFields();

        return logPoint;
    }

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

    void sendLog(const String &logLevel, const String &module, const String &msg) {
        Point point = getLogPoint();
        point.addTag("device", clientName);
        point.addTag("level", logLevel);
        point.addTag("module", module);
        point.addField("msg", msg);
        sendPoint(point);
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

    void sendBootTelemetry() {
        Point point("devices");
        point.addTag("device", clientName);
        point.addField("ip", wifi::ip().toString());
        point.addField("mac", wifi::mac());
        point.addField("flash_size", ESP.getFlashChipSize());
        point.addField("flash_speed", ESP.getFlashChipSpeed());
        
        // ESP8266 only API
#if defined(ESP8266)
        point.addField("flash_id", ESP.getFlashChipId());
        point.addField("free_heap", ESP.getFreeHeap());
        point.addField("last_boot_reason", ESP.getResetReason());
        point.addField("vcc", ESP.getVcc());
#endif
        sendPoint(point);
    }

    void setup(const char* cn) {
        clientName = cn;
        client.setInsecure();
        // Check server connection
        if(!client.validateConnection()) {
            DebugPrint("InfluxDB connection failed: ");
            DebugPrintln(client.getLastErrorMessage());
            return;
        }
        DebugPrint("Connected to InfluxDB: ");
        DebugPrintln(client.getServerUrl());
        sendBootTelemetry();
    }
}
