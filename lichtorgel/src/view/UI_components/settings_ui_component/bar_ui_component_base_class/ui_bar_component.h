#ifndef MIKROCONTROLLER_LICHTORGEL_UI_BAR_COMPONENT_H
#define MIKROCONTROLLER_LICHTORGEL_UI_BAR_COMPONENT_H

#include "../../ui_component.h"
#include <utility>
#include <font_5x8.h>
#include <string>

class ui_bar_component : public ui_component {
public:
    ui_bar_component(std::string label, float max, uGUI gui, int16_t x, int16_t y, int16_t size_x, int16_t size_y);
    virtual void setup() = 0;
    void update() override;

    void increase(){
        reset_marker();
        xc += 1;
        if (xc > x1) {
            xc = x1;
        }
        update();
    }

    void decrease() {
        reset_marker();
        xc -= 1;
        if (xc < x0) {
            xc = x0;
        }
        update();
    }

    void activate() {
        isFocused = true;
        update();
    }

    void deactivate() {
        isFocused = false;
        update();
    }

protected:
    std::string display_name;
    float min_value = 0;
    float max_value;
    float range = max_value - min_value;
    bool isFocused = false;

    int16_t x0 = x + BORDER_SIZE;
    int16_t x1 = x + size_x - BORDER_SIZE;
    int16_t xc = x + (size_x - BORDER_SIZE) / 2;
    int16_t yc = y + size_y / 2;

    void reset_marker();
};


#endif //MIKROCONTROLLER_LICHTORGEL_UI_BAR_COMPONENT_H
