#include <utils.h>

#include "modes.h"

#ifdef UNIT_TEST
    #include <chrono>
    using namespace std::chrono;
    #define millis() (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count())
#else
    #include <Arduino.h>
#endif

// Solid
SolidColor::SolidColor(Color c) {
    color = c;
};

Color SolidColor::nextColor() {
    return color;
}

// BLINK
BlinkColor::BlinkColor(Color c, int i) {
    color = c;
    interval = i;
    lastColor = millis();
};

Color BlinkColor::nextColor() {
    int delta = millis() - lastColor;
    if (delta < interval) {
        return color;
    }
    if (delta < 2*interval) {
        return colors::BLACK;
    }
    lastColor = millis();
    return color;
}