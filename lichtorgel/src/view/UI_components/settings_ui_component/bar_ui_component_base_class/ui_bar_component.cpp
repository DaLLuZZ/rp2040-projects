#include "ui_bar_component.h"

using namespace std;

ui_bar_component::ui_bar_component(std::string label, float max, uGUI _gui, int16_t _x, int16_t _y, int16_t _size_x, int16_t _size_y)
        : ui_component(_gui, _x, _y, _size_x, _size_y), display_name(std::move(label)), max_value(max) {

}

void ui_bar_component::update() {
    auto color_to_use = isFocused ? FRAME_COLOR_FOCUSED : FRAME_COLOR_UNFOCUSED;
    _parent.FillFrame(xc - TICK_LENGTH, yc - TICK_LENGTH, xc + TICK_LENGTH, yc + TICK_LENGTH, color_to_use);
}

void ui_bar_component::reset_marker() {
   _parent.FillFrame(xc - TICK_LENGTH, yc - TICK_LENGTH, xc + TICK_LENGTH, yc + TICK_LENGTH, DEFAULT_BACKGROUND_COLOR);
}
