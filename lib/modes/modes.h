#pragma once

#include <colors.h>

#include <vector>

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

class Jump : public ProgramMode {
    public:
        Jump(std::vector<Color> colors, int interval=1000);
        Color nextColor(unsigned long millis) override;
    private:
        std::vector<Color> colors;
        int interval;
        unsigned long start;
};

class BlinkColor : public Jump {
    public:
        BlinkColor(Color color, int interval=1000);
};

class JumpRainbow : public Jump {
    public:
        JumpRainbow(int interval=1000);
};

class Fade : public ProgramMode {
    public:
        Fade(std::vector<Color> colors, int interval=1000);
        Color nextColor(unsigned long millis) override;
    private:
        std::vector<Color> colors;
        int interval;
        unsigned long start;
};

class FadeColor : public Fade {
    public:
        FadeColor(Color color, int interval=1000);
};

class FadeRainbow : public Fade {
    public:
        FadeRainbow(int interval=1000);
};


// class BlinkColor : public ProgramMode {
//     public:
//         BlinkColor(Color color, int interval=1000);
//         Color nextColor(unsigned long millis) override;
//     private:
//         Color color;
//         int interval;
//         unsigned long lastColor;
// };
// class FadeColor : public ProgramMode {
//     public:
//         FadeColor(Color color, int interval=1000);
//         Color nextColor(unsigned long millis) override;
//     private:
//         Color color;
//         int interval;
//         unsigned long lastColor;
// };
// class BlinkRainbow : public ProgramMode {
//     public:
//         BlinkRainbow(int interval=1000);
//         Color nextColor(unsigned long millis) override;
//     private:
//         int interval;
//         unsigned long start;
// };
