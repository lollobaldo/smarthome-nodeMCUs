#pragma once

#include <colors.h>

#include <FastLED.h>

#include <vector>

class Animation {
    public:
        virtual bool generateLeds(unsigned long millis) = 0;
    protected:
        CRGB* leds;
        int length;
};

class SolidColor : public Animation {
    public:
        SolidColor(CRGB leds[], int length, CHSV color);
        bool generateLeds(unsigned long millis) override;
    private:
        CHSV color;
};

class SolidRainbow : public Animation {
    public:
        SolidRainbow(CRGB leds[], int length);
        bool generateLeds(unsigned long millis) override;
};

class Jump : public Animation {
    public:
        Jump(CRGB leds[], int length, std::vector<CHSV> colors, int period=1000);
        bool generateLeds(unsigned long millis) override;
    private:
        std::vector<CHSV> colors;
        int period;
        unsigned long start;
};

class BlinkColor : public Jump {
    public:
        BlinkColor(CRGB leds[], int length, CHSV color, int period=1000);
};

class Scroll : public Animation {
    public:
        Scroll(CRGB leds[], int length, std::vector<CHSV> colors, int period=1000);
        bool generateLeds(unsigned long millis) override;
    private:
        std::vector<CHSV> colors;
        int period;
        unsigned long start;
};

class ScrollRainbow : public Scroll {
    public:
        ScrollRainbow(CRGB leds[], int length, int period=2000);
};

