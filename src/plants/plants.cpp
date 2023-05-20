#include <logger.h>
#include <mqtt.h>
#include <utils.h>
#include <wifi.h>

#include <Arduino.h>
#include <PubSubClient.h>

#include <string>
// using namespace std;


#ifndef CLIENT_NAME
    #define CLIENT_NAME "ESP--anonymous"
#endif
const char* clientName = "ESP--plants-" CLIENT_NAME;

const char* channelCommands = "plants/commands";
const char* channelLogs = "logs/plants/" CLIENT_NAME;
const char* channelOutput = "plants/" CLIENT_NAME;

std::vector<const char*> subscriptions{ channelCommands };

const int sensorPin = A0;
const long probingTime = 0.5*60*1000L; // 30 seconds

void callback(char* topic, byte* payload, unsigned int length) {
    DebugPrint("Message arrived in topic ");
    DebugPrint(topic);
    DebugPrint(": ");
    // add string terminator
    char* message = (char*) payload;
    message[length] = '\0';
    DebugPrintln(message);
}

void sense() {
    // 0-1023
    int sensor_value= analogRead(sensorPin);
    float output = sensor_value * 100 / 1023;
    DebugPrint(F("Moisture: "));
    DebugPrintln(output);

    // Convert to char* and publish. NOTE: 2sf.
    char ch[7];
    snprintf(ch, sizeof ch, "%f", output);
    mqtt::client.publish(channelOutput, ch, true);
}

unsigned long lastRun = millis() ;
inline void mainLoop() {
  if (millis() - lastRun >= probingTime) {
    sense();
    lastRun = millis();
  }
}

void setup() {
    Serial.begin(115200);
    DebugPrintln("Booting");
    wifi::setup(clientName);
    mqtt::setup(clientName, subscriptions, callback);
}

void loop() {
    wifi::loop();
    mqtt::loop();
    mainLoop();
}
