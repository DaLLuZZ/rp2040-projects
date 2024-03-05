// Color.h
#ifndef COLOR_H
#define COLOR_H

#include <cstdint>
#include "uGUI.h"

class Color {
private:
    uint8_t r, g, b; // 8-bit values for red, green, and blue

public:
    // Constructor that takes RGB values
    Color(uint8_t red, uint8_t green, uint8_t blue);
    Color(uint8_t rgb[3]);

    // Method to convert the color to a 32-bit number (0xRRGGBB format)
    uint32_t toHex() const;
    operator UG_COLOR () const;

};

#endif // COLOR_H
