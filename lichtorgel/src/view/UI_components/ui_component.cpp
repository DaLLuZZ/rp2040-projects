#include "ui_component.h"

ui_component::ui_component(uGUI _gui, int _x, int _y, int _size_x, int _size_y) :
    _parent(_gui), x(_x), y(_y), size_x(_size_x), size_y(_size_y) { }

//// Calculate a color by a percental value on the color scale
uint32_t ui_component::calculateRainbowColor(float input) {
    uint8_t r, g, b;
    if (input < 0.33f) {
        r = 255;
        g = static_cast<uint8_t>(255 * input * 3);
        b = 0;
    } else if (input < 0.67f) {
        r = static_cast<uint8_t>(255 * (1.0f - (input - 0.33f) * 3));
        g = 255;
        b = 0;
    } else {
        r = 0;
        g = static_cast<uint8_t>(255 * (1.0f - (input - 0.67f) * 3));
        b = 255;
    }
    return (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | static_cast<uint32_t>(b);
}
