#ifndef MIKROCONTROLLER_LICHTORGEL_LED_SETTINGS_BAR_H
#define MIKROCONTROLLER_LICHTORGEL_LED_SETTINGS_BAR_H

#include "bar_ui_component_base_class/ui_bar_component.h"
#include <string>
#include <utility>
#include <font_5x8.h>

class led_settings_bar : public ui_bar_component  {
public:
    led_settings_bar(std::string label, float max, uGUI gui, int x, int y, int size_x, int size_y);
    void setup() override;
    void update() override;
    float get_value();
};

#endif //MIKROCONTROLLER_LICHTORGEL_LED_SETTINGS_BAR_H
