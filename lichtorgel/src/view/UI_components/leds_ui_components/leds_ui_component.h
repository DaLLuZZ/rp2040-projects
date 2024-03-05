#ifndef LICHTORGEL_PROJEKT_MC_LEDS_UI_COMPONENT_H
#define LICHTORGEL_PROJEKT_MC_LEDS_UI_COMPONENT_H

#include <uGUI.h>
#include <arm_math_types.h>
#include "../ui_component.h"
#include "../../../led_controller/leds.h"

class leds_ui_component : public ui_component{
public:
    leds_ui_component(uGUI _gui, int _x, int _y, int _size_x, int _size_y);

    void setup() override;
    void update() override;

    void set_data(q15_t d [512]) {
        led_controller.set_data(d);
    }

    void update_led_controller_data(int region_size, float color_randomness, unsigned int lfc, unsigned int mfc, unsigned int hfc) {
        led_controller.set_settings(region_size, color_randomness, lfc, mfc, hfc);
    }

private:
    leds led_controller;
    double led_step_size = (float) size_x / LED_COUNT;

    void set_led_color(int index, unsigned int color);

};

#endif //LICHTORGEL_PROJEKT_MC_LEDS_UI_COMPONENT_H
