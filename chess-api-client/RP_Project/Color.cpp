//
// Created by janni on 17.02.2024.
//


#include "Color.h"

Color::Color(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}
Color::Color(uint8_t rgb[3]) : r(rgb[0]), g(rgb[1]), b(rgb[2]) {}

// toHex method implementation
uint32_t Color::toHex() const {
    return (r << 16) + (g << 8) + b;
}

Color::operator UG_COLOR() const {
    return (r << 16) + (g << 8) + b;
}
