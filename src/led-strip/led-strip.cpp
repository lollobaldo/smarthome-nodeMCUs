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


const char* channelColor = "lights/leds";
const char* channelBrightness = "lights/leds/brightness";


int rgbPins[3] = {D5, D6, D7};

void setColor(Color color);


float brightness = 1;
Color lastColor(colors::BLACK);
unique_ptr<ProgramMode> programMode(new SolidColor(colors::BLACK));


void callback(char* topic, byte* payload, unsigned int length) {
    DebugPrint("Message arrived in topic ");
    DebugPrint(topic);
    DebugPrint(": ");
    // add string terminator
    char* command = (char*) payload;
    command[length] = '\0';
    DebugPrintln(command);

    if (strcmp(topic, channelColor)) {
        // Split commands based on first character
        ProgramMode* newProgramMode;
        switch(*command++) {
            case '#':
                newProgramMode = new SolidColor(Color (command));
                break;
            case '!':
                newProgramMode = new BlinkColor(Color (command));
                break;
            case '~':
                newProgramMode = new FadeColor(Color (command));
                break;
            case 'R':
                newProgramMode = new JumpRainbow();
                break;
            case 'r':
                newProgramMode = new FadeRainbow();
                break;
            case '%':
                newProgramMode = new Fade(colors::string2vector(string (command)));
                break;
            default:
                break;
        }
        programMode.reset(newProgramMode);
        brightness = 1;
        mqtt::client.publish(channelBrightness, "1");
    } else if (strcmp(topic, channelBrightness)) {
        brightness = strtod(command, NULL) / 100;
    }
}

void setColor(Color color) {
    // Multiply by 4 to account for NodeMCU increased resolution (range 0-1023)
    // Make it 4.01 to map 255 -> 1023
    colors::channels gamma_corrected = colors::gamma(colors::brightness(color, brightness));
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
    mqtt::setup(clientName, channelColor, callback);
}

void loop() {
    // DebugPrintln("Loop1");
    wifi::loop();
    mqtt::loop();
    mainLoop();
}
