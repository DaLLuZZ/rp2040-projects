//
// Created by steffen on 19.01.24.
//

#ifndef IMPLEMENTATION_ONOFFSYSTEM_H
#define IMPLEMENTATION_ONOFFSYSTEM_H

#include "gpio_rp2040.h"
#include <utility>

#include "IActor.h"

class OnOffSystem : public IActor {

    uint32_t pin;
    gpio_rp2040_pin control;
    bool isActive = false;

public:
    explicit OnOffSystem(String name, uint32_t pin) : IActor(std::move(name)), pin(pin), control(pin) {
        control.gpioMode(GPIO::OUTPUT);
        control = isActive;
    }
    void setState(bool val) override;
    bool getState() const override;
};


#endif //IMPLEMENTATION_ONOFFSYSTEM_H
