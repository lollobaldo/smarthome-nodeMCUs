#include "Colour.h"

class Colour {
    private:
        short red;
        short green;
        short blue;

    public:
        Colour(const long colour) {
            red = colour >> 16;
            green = colour >> 8 & 0xFF;
            blue = colour & 0xFF;
        }
};
