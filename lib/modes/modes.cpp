#include <utils.h>

#include "modes.h"
using namespace colors;

#ifdef UNIT_TEST
    #include <chrono>
    using namespace std::chrono;
    #define millis() (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count())
#else
    #include <Arduino.h>
#endif

// SOLID
SolidColor::SolidColor(Color c) {
    color = c;
};

Color SolidColor::nextColor(unsigned long millis) {
    return color;
};

// // JUMP
// Jump::Jump(std::vector<byte>& c, int i=1000) {
//     colors = c;
//     interval = i;
//     start = millis();
// };

// Color Jump::nextColor(unsigned long millis) {
//     int i = ((millis - start) / interval) % colors.size();
//     return colors[i];
// };

// BLINK
BlinkColor::BlinkColor(Color c, int i) {
    color = c;
    interval = i;
    lastColor = millis();
};

Color BlinkColor::nextColor(unsigned long millis) {
    int delta = millis - lastColor;
    if (delta < interval) {
        return color;
    }
    if (delta < 2*interval) {
        return colors::BLACK;
    }
    lastColor = millis;
    return color;
};

// FADE
FadeColor::FadeColor(Color c, int i) {
    color = c;
    interval = i;
    lastColor = millis();
};

Color FadeColor::nextColor(unsigned long millis) {
    int delta = (millis - lastColor) % interval;
    double p = 2 * abs(interval/2 - delta) / (double) interval;
    return fade(color, p);
};

// BLINK RAINBOW
BlinkRainbow::BlinkRainbow(int i) {
    interval = i;
    start = millis();
};

Color BlinkRainbow::nextColor(unsigned long millis) {
    int i = ((millis - start) / interval) % RAINBOW.size();
    return RAINBOW[i];
};
