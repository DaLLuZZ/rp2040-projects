//
// Created by steffen on 07.10.23.
//

#ifndef DOORLOCK_BUTTON_H
#define DOORLOCK_BUTTON_H

#include "gpio_rp2040.h"
#include "task.h"

class Button {

    gpio_rp2040_pin sw_pin;
    bool wasPressed = false;
    bool isPressed = false;

    uint32_t debounce_count = 0;

public:
    explicit Button(uint32_t PIN) : sw_pin(PIN)
    {
        sw_pin.gpioMode(GPIO::INPUT | GPIO::PULLUP);
    }
    [[nodiscard]] bool pressed() const;
    [[nodiscard]] bool triggered() const;
    void update();
};


#endif //DOORLOCK_BUTTON_H
