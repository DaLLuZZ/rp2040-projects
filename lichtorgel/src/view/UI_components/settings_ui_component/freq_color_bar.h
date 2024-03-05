#ifndef MIKROCONTROLLER_LICHTORGEL_FREQ_COLOR_BAR_H
#define MIKROCONTROLLER_LICHTORGEL_FREQ_COLOR_BAR_H

#include "bar_ui_component_base_class/ui_bar_component.h"
#include <string>
#include <utility>
#include <font_5x8.h>

class freq_color_bar : public ui_bar_component {
public:
    freq_color_bar(std::string label, float max, uGUI gui, int x, int y, int size_x, int size_y);
    void setup() override;
    void update() override;
    unsigned int get_value();
private:
    void draw_color_spectrum(int x_start, int x_end, int y);
};

#endif //MIKROCONTROLLER_LICHTORGEL_FREQ_COLOR_BAR_H
