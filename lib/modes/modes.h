#pragma once

#include <colors.h>

class ProgramMode {
    public:
        virtual Color nextColor(unsigned long millis) = 0;
};

class SolidColor : public ProgramMode {
    public:
        SolidColor(Color color);
        Color nextColor(unsigned long millis) override;
    private:
        Color color;
};
class BlinkColor : public ProgramMode {
    public:
        BlinkColor(Color color, int interval=1000);
        Color nextColor(unsigned long millis) override;
    private:
        Color color;
        int interval;
        unsigned long lastColor;
};
class FadeColor : public ProgramMode {
    public:
        FadeColor(Color color, int interval=1000);
        Color nextColor(unsigned long millis) override;
    private:
        Color color;
        int interval;
        unsigned long lastColor;
};
