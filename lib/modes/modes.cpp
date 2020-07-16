#include <utils.h>

#include "modes.h"
using namespace std;
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

// JUMP

Jump::Jump(vector<Color> cs, int i) {
    colors = cs;
    interval = i;
    start = millis();
};

Color Jump::nextColor(unsigned long millis) {
    int i = ((millis - start) / interval) % colors.size();
    return colors[i];
};

BlinkColor::BlinkColor(Color c, int i)
    : Jump({c, colors::BLACK}, i) {};

JumpRainbow::JumpRainbow(int i)
    : Jump(colors::RAINBOW, i) {};

// FADE

Fade::Fade(vector<Color> cs, int i) {
    colors = cs;
    interval = i;
    start = millis();
};

Color Fade::nextColor(unsigned long millis) {
    int delta = (millis - start) / interval;
    Color c1 = atWithOverflow(colors, delta);
    Color c2 = atWithOverflow(colors, delta+1);
    // Get decimal part of delta, divide by interval
    double p = ((millis - start) % interval) / (double) interval;
    return fade(c1, c2, 1 - p);
};

FadeColor::FadeColor(Color c, int i)
    : Fade({c, colors::BLACK}, i) {};

FadeRainbow::FadeRainbow(int i)
    : Fade(colors::RAINBOW, i) {};


// Color FadeColor::nextColor(unsigned long millis) {
//     int delta = (millis - lastColor) % interval;
//     double p = 2 * abs(interval/2 - delta) / (double) interval;
//     return fade(color, p);
// };

// // BLINK
// BlinkColor::BlinkColor(Color c, int i) {
//     color = c;
//     interval = i;
//     lastColor = millis();
// };

// // BlinkColor::BlinkColor(Color c, int i)
// //     : Jump(std::vector<Color> { c, colors::BLACK };, i) {}

// Color BlinkColor::nextColor(unsigned long millis) {
//     int delta = millis - lastColor;
//     if (delta < interval) {
//         return color;
//     }
//     if (delta < 2*interval) {
//         return colors::BLACK;
//     }
//     lastColor = millis;
//     return color;
// };

// // FADE
// FadeColor::FadeColor(Color c, int i) {
//     color = c;
//     interval = i;
//     lastColor = millis();
// };

// Color FadeColor::nextColor(unsigned long millis) {
//     int delta = (millis - lastColor) % interval;
//     double p = 2 * abs(interval/2 - delta) / (double) interval;
//     return fade(color, p);
// };

// // BLINK RAINBOW
// BlinkRainbow::BlinkRainbow(int i) {
//     interval = i;
//     start = millis();
// };

// Color BlinkRainbow::nextColor(unsigned long millis) {
//     int i = ((millis - start) / interval) % RAINBOW.size();
//     return RAINBOW[i];
// };
