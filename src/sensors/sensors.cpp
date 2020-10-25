#include <logger.h>
#include <mqtt.h>
#include <utils.h>
#include <wifi.h>

#include <Arduino.h>
#include <DHT.h>
#include <PubSubClient.h>

#include <string>
using namespace std;

const char* clientName = "ESP--sensors";

const int dhtPin = D4;
const int dhtType = DHT11;
DHT dht(dhtPin, dhtType);

const long probingTime = 1*60*1000L;

const char* channelCommands = "sensors/commands";
const char* channelLog = "logs/sensors";
const char* channelTemperature = "sensors/temperature";
const char* channelHumidity = "sensors/humidity";
const char* channelHeatIndex = "sensors/heatIndex";

void sense() {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    DebugPrint(h);
    DebugPrint(t);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }

    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    DebugPrint(F("Humidity: "));
    DebugPrint(h);
    DebugPrint(F("%  Temperature: "));
    DebugPrint(t);
    DebugPrint(F("°C Heat index: "));
    DebugPrint(hic);
    DebugPrintln(F("°F"));

    // Convert to char* and publish. NOTE: 2sf.
    char ch[7];
    snprintf(ch, sizeof ch, "%f", h);
    mqtt::client.publish(channelHumidity, ch, true);
    char ct[7];
    snprintf(ct, sizeof ct, "%f", t);
    mqtt::client.publish(channelTemperature, ct, true);
    char chic[7];
    snprintf(chic, sizeof chic, "%f", hic);
    mqtt::client.publish(channelHeatIndex, chic, true);
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
    dht.begin();
}

void loop() {
    wifi::loop();
    mqtt::loop();
    mainLoop();
}
