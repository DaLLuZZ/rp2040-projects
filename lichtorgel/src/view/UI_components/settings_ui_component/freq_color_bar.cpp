#include <algorithm>
#include "freq_color_bar.h"

using namespace std;

freq_color_bar::freq_color_bar(std::string label, float max, uGUI _gui, int _x, int _y, int _size_x, int _size_y)
        : ui_bar_component(std::move(label), max, _gui, _x, _y, _size_x, _size_y) { }

void freq_color_bar::setup() {
    _parent.FontSelect(&FONT_5X8);
    _parent.FontSetHSpace(0);

    //// Place the label
    _parent.PutString(x1 + BORDER_SIZE, yc - 4, display_name.c_str());
    //// Update redraws the bar
    update();
}

void freq_color_bar::update() {
    //// Reset the color bar
    draw_color_spectrum(x0, x1, yc);
    //// Draw the new marker
    ui_bar_component::update();
}

//// Draw the whole rainbow color spektrum from pixel x0 to pixel x1
void freq_color_bar::draw_color_spectrum(int x_start, int x_end, int y) {
    int range = x_end - x_start;
    for(int i = x_start; i < x_end; i++) {
        float val = (float) (i - x_start) / (float) range;
        _parent.DrawPixel(i, y, calculateRainbowColor(val));
    }
}

unsigned int freq_color_bar::get_value() {
    float v = (xc - x0) / (float) (x1 - x0);
    return calculateRainbowColor(v);
}
