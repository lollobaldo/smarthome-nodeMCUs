#include "mqtt.h"

#include <logger.h>
#include <utils.h>

#if defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>
#elif defined(ESP32)
    #include <WiFi.h>
    #include <WebServer.h>
#else
    #error "Unsupported platform"
#endif

#include <unordered_map>
#include <string>

#ifndef MQTT_USER
    #error "MQTT_USER is not defined"
#endif

#define MQTT_SERVER "mqtt.flespi.io"
#define MQTT_PORT 1883
#define MQTT_PASSWORD ""

#ifndef CLIENT_NAME
    #error message "CLIENT_NAME is not defined"
#endif

#define LOGS_CHANNEL "logs/" CLIENT_NAME

namespace mqtt {
    Logger logger("mqtt.cpp");
    
    const char* clientName;

    std::unordered_map<std::string, callbackPointer_t> callbacks;

    const char* connectMessage;
    const char* willTopic;
    const char* willMessage;

    constexpr byte willQoS = 0;
    constexpr boolean willRetain = true;

    WiFiClient wifiClient;
    PubSubClient client(wifiClient);

    boolean connect() {
        DebugPrint("Connecting to MQTT as ");
        DebugPrint(clientName);
        DebugPrint("... ");
        return client.connect(clientName, MQTT_USER, MQTT_PASSWORD, LOGS_CHANNEL, 0, true, willMessage);
    }

    void reconnect() {
        // Loop until we're reconnected
        while (!client.connected()) {
            DebugPrint("Attempting MQTT connection...");
            // Attempt to connect
            if (connect()) {
                DebugPrintln("connected");
                logger.log("MQTT connected");
                // Once connected, publish an announcement and resubscribe
                // client.publish(cLog, connectMessage);
                // for (auto &t: topics) {
                //     client.subscribe(t);
                // }
            } else {
                DebugPrint("ERROR, rc: ");
                DebugPrint(client.state());
                DebugPrintln(". Will try again in 3 seconds");
                // Wait 3 seconds before retrying
                delay(3000);
            }
        }
    }

    void handleCallback(char* topic, byte* payload, unsigned int length) {
        DebugPrint("Routing message in topic: ");
        DebugPrintln(topic);
        std::string topic_s(topic);
        if (callbacks.find(topic_s) == callbacks.end()) {
            DebugPrint("No callback for topic ");
            DebugPrintln(topic);
            return;
        }
        const char* message = copyFromBytes(payload, length);
        callbacks[topic](topic, message);
    }

    void subscribe(const char* topic, callbackPointer_t callback) {
        DebugPrint("Subscribing to topic: ");
        DebugPrintln(topic);
        std::string topic_s(topic);
        callbacks[topic_s] = callback;
        client.subscribe(topic);
    }

    void setup(const char* cn) {
        clientName = cn;
        connectMessage = concat("INF: ", clientName, " connected.");
        willMessage = concat("ERR: ", clientName, " disconnected");
        client.setServer(MQTT_SERVER, MQTT_PORT);
        client.setCallback(handleCallback);
        reconnect();
    }

    void loop() {
        if (!client.connected()) {
            reconnect();
        }
        client.loop();
    }
}
