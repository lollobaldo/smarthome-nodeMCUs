#include <utils.h>
#include <wifi.h>

#include <Arduino.h>
#include <PubSubClient.h>

#include <string>
using namespace std;

#include "mqtt.h"

#ifndef MQTT_PSW
    #define MQTT_PSW "(MQTT_PSW not defined)"
#endif

namespace mqtt {
    const char* server = "mqtt.flespi.io";
    const int port = 1883;
    const char* name;
    const char* user = MQTT_PSW;
    const char* password = "";

    const char* cDebug = "debug";
    const char* cInput;
    const char* cLog;

    const char* connectMessage;
    const char* willTopic;
    const char* willMessage;
    byte willQoS = 0;
    boolean willRetain = true;

    PubSubClient client(wifi::client);

    boolean connect() {
        return client.connect(name, user, password, willTopic, willQoS, willRetain, willMessage);
    }

    void reconnect() {
        // Loop until we're reconnected
        while (!client.connected()) {
            DebugPrint("Attempting MQTT connection...");
            // Attempt to connect
            if (client.connect(name, user, password)) {
                DebugPrintln("connected");
                // Once connected, publish an announcement and resubscribe
                client.publish(cLog, connectMessage);
                client.subscribe(cInput);
            } else {
                DebugPrint("failed, rc=");
                DebugPrint(client.state());
                DebugPrintln(" try again in 5 seconds");
                // Wait 3 seconds before retrying
                delay(3000);
            }
        }
    }

    void setup(const char* clientName, const char* channelInput, callbackType callback) {
        name = clientName;
        cInput = channelInput;
        connectMessage = concat(clientName, " connected");
        willMessage = concat(clientName, " disconnected");
        cLog = concat("logs/", channelInput);
        client.setServer(server, port);
        client.setCallback(callback);
    }

    void loop() {
        if (!client.connected()) {
            reconnect();
        }
        client.loop();
    }
}
