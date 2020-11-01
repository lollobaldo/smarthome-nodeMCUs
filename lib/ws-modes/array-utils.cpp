#include <utils.h>

#include "array-utils.h"

void fillAll(CRGB leds[], int length, CHSV color) {
    for (int i = 0; i < length; i++) {
        leds[i] = color;
    }
}

std::vector<CHSV> generateRainbow(int length) {
    float step = 255 / (length - 1);
    std::vector<CHSV> ret;
    for (int i = 0; i < length; i++) {
        ret.push_back(CHSV(i*step, 255, 255));
    }
    return ret;
}