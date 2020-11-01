#include <array-utils.h>
#include <utils.h>

#include <FastLED.h>

#include "animations.h"
using namespace std;

#ifdef UNIT_TEST
    #include <chrono>
    using namespace std::chrono;
    #define millis() (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count())
#else
    #include <Arduino.h>
#endif

// SOLID
SolidColor::SolidColor(CRGB ls[], int l, CHSV c) {
    leds = ls;
    length = l;
    color = c;
    fillAll(leds, length, c);
    FastLED.show();
};

inline bool SolidColor::generateLeds(unsigned long millis) {
    return false;
};

// RAINBOW
SolidRainbow::SolidRainbow(CRGB ls[], int l) {
    leds = ls;
    length = l;
    float step = 255 / length;
    for (int i = 0; i < length; i++) {
        leds[i] = CHSV(i*step, 255, 255);
    }
    FastLED.show();
};

inline bool SolidRainbow::generateLeds(unsigned long millis) {
    return false;
};

// JUMP
Jump::Jump(CRGB ls[], int l, vector<CHSV> cs, int p) {
    leds = ls;
    length = l;
    colors = cs;
    period = p;
    start = millis();
};

bool Jump::generateLeds(unsigned long millis) {
    int i = ((millis - start) / period) % colors.size();
    fillAll(leds, length, colors[i]);
    return true;
};

BlinkColor::BlinkColor(CRGB ls[], int l, CHSV c, int p)
    : Jump(ls, l, {c, CHSV(0, 0, 0)}, p) {};

// SCROLL
Scroll::Scroll(CRGB ls[], int l, vector<CHSV> cs, int p) {
    leds = ls;
    length = l;
    colors = cs;
    period = p;
    start = millis();
};

bool Scroll::generateLeds(unsigned long millis) {
    int k = ((millis - start) % period) * colors.size() / period;
    for (int i = 0; i < length; i++) {
        leds[i] = colors[(k + i) % colors.size()];
    }
    return true;
};

ScrollRainbow::ScrollRainbow(CRGB ls[], int l, int p)
    : Scroll(ls, l, generateRainbow(l), p) {};
