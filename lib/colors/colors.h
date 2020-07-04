#pragma once

#include <string>

class Color {
    public:
        short red;
        short green;
        short blue;
        Color();
        Color(const std::string hex_color);
        Color(const char* hex_color);
        Color(const int color);
        friend bool operator == (Color a, Color b);
        friend bool operator != (Color a, Color b);
};

namespace colors {
    static Color BLACK = Color(0x000000);
    static Color WHITE = Color(0xFFFFFF);
    static Color ORANGE = Color(0xFFFFFF);
    struct channels {
        short red;
        short green;
        short blue;
    };
    int normalise(int channel);
    channels gamma(Color color);
}