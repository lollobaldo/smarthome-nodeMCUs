#include <animations.h>
#include <logger.h>
#include <mqtt.h>
#include <utils.h>
#include <wifi.h>

#include <Arduino.h>
#include <FastLED.h>
#include <PubSubClient.h>

#include <string>
using namespace std;

const char* clientName = "ESP--plants";

#define LED_PIN 5
#define LEDS_COUNT 30

#define CHIPSET WS2812B
#define COLOR_ORDER GRB

CRGB leds[LEDS_COUNT + 1];
int ledsCount = LEDS_COUNT;

#define BRIGHTNESS 128

const char* channelCommands = "lights/leds";
const char* channelLog = "logs/ws-strip";

unique_ptr<Animation> animation(new SolidColor(leds, ledsCount, CHSV(0, 0, 0)));

void sense() {
    // animation.reset(new SolidColor(leds, ledsCount, CHSV(200, 255, 255)));
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
    if (animation->generateLeds(millis())) {
        FastLED.show();
    }
}

void setup() {
    Serial.begin(115200);
    DebugPrintln("Booting");
    wifi::setup(clientName);
    mqtt::setup(clientName, channelCommands, callback);
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, ledsCount);
    FastLED.setMaxPowerInVoltsAndMilliamps(5,800);
    animation.reset(new ScrollRainbow(leds, ledsCount));

    FastLED.show();
}

void loop() {
    wifi::loop();
    mqtt::loop();
    mainLoop();
}
