//
// Created by steffen on 07.10.23.
//

#include "Button.h"

bool Button::pressed() const{
    return isPressed;
}

bool Button::triggered() const {
    return wasPressed;
}

void Button::update() {
    if(!sw_pin)
    {
        if(debounce_count > 1)
            isPressed = true;
        else
            debounce_count++;
    }
    else
    {
        if(debounce_count > 1)
        {
            wasPressed = true;
        }
        else
        {
            wasPressed = false;
        }
        isPressed = false;
        debounce_count = 0;
    }
}
