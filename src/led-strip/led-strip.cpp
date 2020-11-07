#include <colors.h>
#include <logger.h>
#include <modes.h>
#include <mqtt.h>
#include <utils.h>
#include <wifi.h>

#include <Arduino.h>
#include <PubSubClient.h>

#include <string>
using namespace std;


#ifndef CLIENT_NAME
    #define CLIENT_NAME "ESP--anonymous"
#endif
const char* clientName = "ESP--lights-leds-"CLIENT_NAME;

const char* channelLog = "logs/lights/leds";

const char* channel0 = "lights/leds/"CLIENT_NAME;
const char* channel1 = "lights/leds";
const char* channel2 = "lights";
// const char* channelBrightness = "lights/leds/brightness";

vector<const char*> subscriptions{ channel0, channel1, channel2 };

int rgbPins[3] = {D5, D6, D7};

void setColor(Color color);

float brightness = 1;
unique_ptr<ProgramMode> programMode(new SolidColor(colors::BLACK));

Color lastColor(colors::BLACK);

void changeMode(char* command) {
    logger::log(LogLevel::INFO, channelLog, concat("Changing mode to: ", command));

    // Select commands based on first character
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
            DebugPrintln("In default");
            DebugPrintln(command);
            logger::log(LogLevel::WARN, channelLog, concat("Mode not supported: ", command));
            return;
    }
    programMode.reset(newProgramMode);
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


    if (true) {
        DebugPrintln("In change mode");
        changeMode(command);
    // } else if (channel == channelBrightness) {
        // DebugPrintln("In brightness mode");
        // brightness = strtod(command, NULL) / 100;
    } else {
        DebugPrint("Unsupported topic: ");
        DebugPrintln(topic);
    }

    // Deallocate memory
    delete command;
}

void setColor(Color color) {
    // Multiply by 4 to account for NodeMCU increased resolution (range 0-1023)
    // Make it 4.01 to map 255 -> 1023
    colors::channels gamma_corrected = colors::gamma(color);
    // colors::channels gamma_corrected = colors::gamma(colors::brightness(color, brightness));
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
        DebugPrintln("Set new color");
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
