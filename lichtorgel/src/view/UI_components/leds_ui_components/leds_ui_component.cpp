#include "leds_ui_component.h"

leds_ui_component::leds_ui_component(uGUI _gui, int _x, int _y, int _size_x, int _size_y)
    : ui_component(_gui, _x, _y, _size_x, _size_y) { }

void leds_ui_component::setup() {
    //// Frame
    _parent.DrawFrame(x, y, x + size_x, y + size_y, FRAME_COLOR_UNFOCUSED);
}

void leds_ui_component::update() {
    //// Filling the grid by its colors
    auto colors = led_controller.get_colors();
    for(int i = 0; i < LED_COUNT; i++) {
        set_led_color(i, colors[i]);
    }
}

void leds_ui_component::set_led_color(int index, unsigned int color) {
    int x_pos0 = (int) (index * led_step_size) + x;
    int x_pos1 = (int) (index + 1) * led_step_size + x;
    _parent.FillFrame(x_pos0, y, x_pos1, y + size_y, color);
}
