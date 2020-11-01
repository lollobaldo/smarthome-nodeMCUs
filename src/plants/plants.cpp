#include <logger.h>
#include <mqtt.h>
#include <utils.h>
#include <wifi.h>

#include <Arduino.h>
#include <PubSubClient.h>

#include <string>
using namespace std;

const char* clientName = "ESP--plants";

const int sensorPin = A0;

const long probingTime = 0.5*60*1000L;

const char* channelCommands = "plants/commands";
const char* channelLog = "logs/plants";
const char* channelOutput = "plants";

void sense() {
    // 0-1023
    int sensor_value= analogRead(sensorPin);
    float output = sensor_value * 100 / 1023;
    DebugPrint(F("Moisture: "));
    DebugPrint(output);

    // Convert to char* and publish. NOTE: 2sf.
    char ch[7];
    snprintf(ch, sizeof ch, "%f", output);
    mqtt::client.publish(channelOutput, ch, true);
}

void callback(char* topic, byte* payload, unsigned int length) {
    DebugPrint("Message arrived in topic ");
    DebugPrint(topic);
    DebugPrint(": ");
    // add string terminator
    char* message = (char*) payload;
    message[length] = '\0';
    DebugPrintln(message);

    // Copy topic and payload as they get overwritten
    const string channel = string(topic);
    // const string command = string(message);
    char* command = new char[strlen(message) + 1];
    strncpy(command, message, strlen(message) + 1 );


    if (channel == channelCommands) {
        DebugPrintln("Command received");
        sense();
    }

    // Deallocate memory
    delete command;
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
    mqtt::setup(clientName, channelCommands, callback);
}

void loop() {
    wifi::loop();
    mqtt::loop();
    mainLoop();
}
