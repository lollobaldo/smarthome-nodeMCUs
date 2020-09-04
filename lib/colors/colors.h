#pragma once

#include <utils.h>

#include <string>
#include <vector>

class Color {
    public:
        byte red;
        byte green;
        byte blue;
        Color();
        Color(const std::string& hex_color);
        Color(const char* hex_color);
        Color(const int& color);
        Color(const byte& red, const byte& green, const byte& blue);
        std::string toString();
        friend bool operator == (const Color& a, const Color& b);
        friend bool operator != (const Color& a, const Color& b);
        friend std::ostream& operator << (std::ostream &out, const Color& c);
};

namespace colors {
    static Color BLACK = Color(0x000000);
    static Color WHITE = Color(0xFFFFFF);
    static Color RED = Color(0xFF0000);
    static Color ORANGE = Color(0xFF7700);
    static Color YELLOW = Color(0xFFCC00);
    static Color GREEN = Color(0x00FF00);
    static Color AQUAMARINE = Color(0x00FFBB);
    static Color BLUE = Color(0x0000FF);
    static Color INDIGO = Color(0x2E2B5F);
    static Color PURPLE = Color(0x9900FF);
    static Color VIOLET = Color(0xFF00FF);

    // static std::vector<Color> RAINBOW_THREE = {
    //     RED, GREEN, BLUE,
    // };

    static std::vector<Color> RAINBOW = {
        RED, ORANGE, YELLOW,
        GREEN, BLUE, VIOLET,
    };

    // static std::vector<Color> FULL_RAINBOW = {
    //     RED, ORANGE, YELLOW, GREEN,
    //     AQUAMARINE, BLUE, PURPLE, VIOLET,
    // };

    struct channels {
        byte red;
        byte green;
        byte blue;
    };

    byte normalise(const byte& channel);
    Color brightness(const Color& color, const double& percentage);
    Color fade(const Color& color1, const Color& color2, const double& percentage);
    channels gamma(const Color& color);
    channels gamma(const Color& color, float brightness);
    std::vector<Color> string2vector(const std::string& s);
}