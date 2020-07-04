#pragma once

#include <colors.h>

class ProgramMode {
    public:
        virtual Color nextColor() = 0;
};

class SolidColor : public ProgramMode {
    public:
        SolidColor(Color color);
        Color nextColor() override;
    private:
        Color color;
};
class BlinkColor : public ProgramMode {
    public:
        BlinkColor(Color color, int interval=500);
        Color nextColor() override;
    private:
        Color color;
        int interval;
        unsigned long lastColor;
};