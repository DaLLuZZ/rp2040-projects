#include "user_control.h"

user_control::user_control() {
    //// Setup GPIO modes
    button_next.gpioMode(GPIO::INPUT | GPIO::PULLUP);
    button_decrease.gpioMode(GPIO::INPUT | GPIO::PULLUP);
    button_increase.gpioMode(GPIO::INPUT | GPIO::PULLUP);
}
