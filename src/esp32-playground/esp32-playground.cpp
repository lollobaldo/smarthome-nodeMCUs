#include <logger.h>
#include <mqtt.h>
#include <utils.h>
#include <wifiHandler.h>

#include <Arduino.h>

#include <string>

#ifndef CLIENT_NAME
    #error "CLIENT_NAME is not defined"
#endif
const char* clientName = "ESP32--playground-" CLIENT_NAME;

const char* channelCommands = "playground/commands";
const char* channelLogs = "logs/playground/" CLIENT_NAME;
const char* channelOutput = "playground/" CLIENT_NAME;

std::vector<const char*> subscriptions{ channelCommands };

const int sensorPin = A0;
const int LED = 33;
const long probingTime = 0.05*60*1000L; // 3 seconds

bool ledState = false;

Logger logger = Logger(channelLogs);

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
  logger.log(LogLevel::INFO, "Sensing...");
  ledState = !ledState;
  digitalWrite(LED, ledState);
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
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
    wifi::setup(clientName);
    mqtt::setup(clientName, subscriptions, callback);
}

void loop() {
    wifi::loop();
    mqtt::loop();
    mainLoop();
}
