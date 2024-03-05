#include "led_settings_bar.h"

using namespace std;

led_settings_bar::led_settings_bar(std::string label, float max, uGUI _gui, int _x, int _y, int _size_x, int _size_y)
    : ui_bar_component(std::move(label), max, _gui, _x, _y, _size_x, _size_y) { }

void led_settings_bar::setup() {
    _parent.FontSelect(&FONT_5X8);
    _parent.FontSetHSpace(0);

    //// Left tick label
    _parent.PutString(x + BORDER_SIZE - 3, yc + 2, to_string((int) min_value).c_str());

    //// Right tick label
    int right_label_offset = to_string((int) max_value).length() * 5;
    _parent.PutString(x + size_x - right_label_offset - BORDER_SIZE, yc + 2, to_string((int) max_value).c_str());

    //// Place the display label
    _parent.PutString(x + size_x + 3, yc - 4, display_name.c_str());
    update();
}

//// Update the bar
void led_settings_bar::update() {
    //// Redraw the bar
    _parent.DrawLine(x0, yc, x1, yc, DEFAULT_TEXT_COLOR);
    //// Reset the old marker and draw the new one
    ui_bar_component::update();
}

float led_settings_bar::get_value() {
    float ret = (xc - x0) / (float) (x1 - x0) * range + min_value;
    return ret;
}
