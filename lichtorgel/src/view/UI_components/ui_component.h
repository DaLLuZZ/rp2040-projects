#ifndef LICHTORGEL_PROJEKT_MC_UI_COMPONENT_H
#define LICHTORGEL_PROJEKT_MC_UI_COMPONENT_H

#include <uGUI.h>
#include "../ui_constants.h"

class ui_component {
public:
    ui_component(uGUI gui, int x, int y, int size_x, int size_y);
    virtual void setup() = 0;
    virtual void update() = 0;
    static uint32_t calculateRainbowColor(float input);
protected:
    uGUI _parent;
    int x, y;
    int size_x, size_y;
};

#endif //LICHTORGEL_PROJEKT_MC_UI_COMPONENT_H
