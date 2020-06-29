#ifdef UNIT_TEST
    #define PROGMEM
    #define pgm_read_byte(a) *(a)
    // #define uint8_t __uint128_t
#else
    #include <Arduino.h>
#endif

#include <string>

class Color {
    public:
        short red;
        short green;
        short blue;
        Color();
        Color(const std::string hex_color);
        Color(const char* hex_color);
        Color(const long color);
};

namespace colors {
    struct channels {
        short red;
        short green;
        short blue;
    };
    int normalise(int channel);
    channels gamma(Color color);
}