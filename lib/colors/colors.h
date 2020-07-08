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
        Color(const int red, const int green, const int blue);
        std::string toString();
        friend bool operator == (Color a, Color b);
        friend bool operator != (Color a, Color b);
        friend std::ostream& operator << (std::ostream &out, Color const& c);
};

namespace colors {
    static Color BLACK = Color(0x000000);
    static Color WHITE = Color(0xFFFFFF);
    static Color RED = Color(0xFF0000);
    static Color ORANGE = Color(0xFF7F00);
    static Color YELLOW = Color(0xFFFF00);
    static Color GREEN = Color(0x00FF00);
    static Color BLUE = Color(0x0000FF);
    static Color INDIGO = Color(0x2E2B5F);
    static Color VIOLET = Color(0x8B00FF);

    static Color RAINBOW[] = {
        RED, ORANGE, YELLOW,
        GREEN, BLUE, INDIGO, VIOLET
    };

    struct channels {
        short red;
        short green;
        short blue;
    };

    int normalise(int channel);
    Color fade(Color c, double p);
    channels gamma(Color color);
}