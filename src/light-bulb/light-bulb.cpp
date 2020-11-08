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
const char* clientName = "ESP--lights-bulbs-"CLIENT_NAME;

const char* channel0 = "lights/bulbs/"CLIENT_NAME;
const char* channel1 = "lights/bulbs";
const char* channel2 = "lights";
// const char* channelBrightness = "lights/leds/brightness";

vector<const char*> subscriptions{ channel0, channel1, channel2 };


// White lights use one pin for brightness (W)
//  and one for temperature (T) (2700-6500K)
const uint8_t pin_W = 5;
const uint8_t pin_T = 13;
const uint8_t pin_R = 4;
const uint8_t pin_G = 12;
const uint8_t pin_B = 14;

float brightness = 1;
unique_ptr<ProgramMode> programMode(new SolidColor(colors::BLACK));

Color lastColor(colors::BLACK);

static void changeMode(char* command) {
    logger::log(concat("New mode: ", command));
    // Select commands based on first character
    ProgramMode* newProgramMode;
    switch(*command++) {
        case 'W':
            newProgramMode = new SolidColor(colors::BLACK);
            analogWrite(pin_W, 1023);
            analogWrite(pin_T, 0);
            break;
        case 'N':
            newProgramMode = new SolidColor(colors::BLACK);
            analogWrite(pin_W, 1023);
            analogWrite(pin_T, 512);
            break;
        case 'C':
            newProgramMode = new SolidColor(colors::BLACK);
            analogWrite(pin_W, 1023);
            analogWrite(pin_T, 1023);
            break;
        case '/':
            newProgramMode = new SolidColor(colors::BLACK);
            analogWrite(pin_W, 0);
            break;
        case '#':
            newProgramMode = new SolidColor(Color (command));
            break;
        // case '!':
        //     newProgramMode = new BlinkColor(Color (command));
        //     break;
        // case '~':
        //     newProgramMode = new FadeColor(Color (command));
        //     break;
        // case 'R':
        //     newProgramMode = new JumpRainbow();
        //     break;
        // case 'r':
        //     newProgramMode = new FadeRainbow();
        //     break;
        // case '%':
        //     newProgramMode = new Fade(colors::string2vector(string (command)));
        //     break;
        default:
            DebugPrintln("In default");
            DebugPrintln(command);
            logger::log(LogLevel::WARN, concat("Mode not supported: ", command));
            return;
    }
    programMode.reset(newProgramMode);
}

void callback(char* topic, byte* payload, unsigned int length) {
    DebugPrint("[");
    DebugPrint(topic);
    DebugPrint("] ");
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
    } else {
        DebugPrint("Unsupported topic: ");
        DebugPrintln(topic);
    }

    // Deallocate memory
    delete command;
}

static inline void setColor(Color color) {
    // Multiply by 4 to account for NodeMCU increased resolution (range 0-1023)
    // Make it 4.01 to map 255 -> 1023
    colors::channels gamma_corrected = colors::gamma(color);
    // colors::channels gamma_corrected = colors::gamma(colors::brightness(color, brightness));
    short r = round(4.01 * gamma_corrected.red);
    short g = round(4.01 * gamma_corrected.green);
    short b = round(4.01 * gamma_corrected.blue);

    analogWrite(pin_R, r);
    analogWrite(pin_G, g);
    analogWrite(pin_B, b);
}

static inline void mainLoop() {
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
