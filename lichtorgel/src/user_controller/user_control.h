#ifndef MIKROCONTROLLER_LICHTORGEL_USER_CONTROL_H
#define MIKROCONTROLLER_LICHTORGEL_USER_CONTROL_H

#include <gpio_rp2040.h>

#define PIN_BUTTON_DECREASE 16  // Left S1 button or upper button on extension board
#define PIN_BUTTON_INCREASE 17  // Right S2 button or lower button on extension board
#define PIN_BUTTON_NEXT 19 // Joystick push in button

class user_control {
public:
    user_control();
    gpio_rp2040_pin button_next{PIN_BUTTON_NEXT};
    gpio_rp2040_pin button_decrease{PIN_BUTTON_DECREASE};
    gpio_rp2040_pin button_increase{PIN_BUTTON_INCREASE};
};

#endif //MIKROCONTROLLER_LICHTORGEL_USER_CONTROL_H
