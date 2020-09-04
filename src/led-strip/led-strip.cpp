#include <colors.h>
#include <modes.h>
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

void setColor(Color color);


Color lastColor(colors::BLACK);

unique_ptr<ProgramMode> programMode(new SolidColor(colors::BLACK));


void callback(char* topic, byte* payload, unsigned int length) {
    DebugPrint("Message arrived in topic ");
    DebugPrint(topic);
    DebugPrint(": ");
    // add string terminator
    payload[length] = '\0';
    DebugPrintln((char*) payload);

    // Split commands based on first character
    switch(*payload++) {
        case '#':
            programMode.reset(new SolidColor(Color ((char*) payload)));
            break;
        case '!':
            programMode.reset(new BlinkColor(Color ((char*) payload)));
            break;
        case '~':
            programMode.reset(new FadeColor(Color ((char*) payload)));
            break;
        case 'R':
            programMode.reset(new JumpRainbow());
            break;
        case 'r':
            programMode.reset(new FadeRainbow());
            break;
        default:
            break;
    }
}

void setColor(Color color) {
    // Multiply by 4 to account for NodeMCU increased resolution (range 0-1023)
    // Make it 4.01 to map 255 -> 1023
    colors::channels gamma_corrected = colors::gamma(color);
    short r = round(4.01 * gamma_corrected.red);
    short g = round(4.01 * gamma_corrected.green);
    short b = round(4.01 * gamma_corrected.blue);

    analogWrite(rgbPins[0], r);
    analogWrite(rgbPins[1], g);
    analogWrite(rgbPins[2], b);
}

inline void mainLoop() {
    Color next = programMode->nextColor(millis());
    if (lastColor != next) {
        lastColor = next;
        setColor(next);
    }
}

void setup() {
    Serial.begin(115200);
    DebugPrintln("Booting");
    wifi::setup(clientName);
    mqtt::setup(clientName, channelInput, callback);
}

void loop() {
    // DebugPrintln("Loop1");
    wifi::loop();
    mqtt::loop();
    mainLoop();
}
