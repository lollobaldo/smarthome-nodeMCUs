#include <mqtt.h>
#include <utils.h>
#include <wifi.h>

#include <Arduino.h>
#include <PubSubClient.h>

#include <string>
using namespace std;

const char* clientName = "ESP--leds-strip";

const char* channelInput = "lights/leds";

int rgbPins[3] = {D5, D6, D7};

extern const uint8_t gamma8[];
void setColor(long color);

void callback(char* topic, byte* payload, unsigned int length) {
    DebugPrint("Message arrived in topic ");
    DebugPrint(topic);
    DebugPrint(": ");
    //string to hex
    payload[length] = '\0';
    DebugPrintln((char*) payload);
    // if (payload[0] == '!') {
    //     ESP.restart();
    // }
    // Ignore '#'
    payload++;
    DebugPrintln("Inside fnc_ColorChange attempted to change color");
    // Get rid of '#' and convert it to integer
    const char* trial = "ff0000";
    DebugPrintln(trial);
    payload[6] = '\0';
    DebugPrintln((char*)payload);
    long number0 = strtol(trial, NULL, 16);
    DebugPrint("Converting to long: ");
    DebugPrintln(number0);
    long number = strtol((char*)payload, NULL, 16);
    DebugPrint("Converting to long: ");
    DebugPrintln(number);
    setColor(number);
    DebugPrintln("-----------------------");
}

void setColor(long color) {
    int r = color >> 16;
    int g = color >> 8 & 0xFF;
    int b = color & 0xFF;

    int length = snprintf( NULL, 0, "Setting led strip to rgb(%d, %d, %d)", r, g, b);
    char* str = (char*) malloc( length + 1 );
    snprintf( str, length + 1, "Setting led strip to rgb(%d, %d, %d)", r, g, b);
    mqtt::client.publish(mqtt::cLog, str);

    // Multiply by 4 to account for NodeMCU increased resolution (range 0-1023)
    // Make it 4.01 to map 255 -> 1023
    r = round(4.01 * pgm_read_byte(&gamma8[r]));
    g = round(4.01 * pgm_read_byte(&gamma8[g]));
    b = round(4.01 * pgm_read_byte(&gamma8[b]));

    analogWrite(rgbPins[0], r);
    analogWrite(rgbPins[1], g);
    analogWrite(rgbPins[2], b);
}

void setup() {
    Serial.begin(115200);
    DebugPrintln("Booting");
    wifiSetup(clientName);
    mqtt::setup(clientName, channelInput, callback);
}

void loop() {
    wifiLoop();
    mqtt::loop();
}

const uint8_t PROGMEM gamma8[] = {
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
      1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
      2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
      5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
     10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
     17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
     25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
     37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
     51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
     69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
     90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
    115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
    144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
    177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
    215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
