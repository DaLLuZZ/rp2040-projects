//
// Created by steffen on 19.01.24.
//

#include "OnOffSystem.h"

bool OnOffSystem::getState() const {
    return isActive;
}

void OnOffSystem::setState(bool val) {
    isActive = val;
    control.gpioWrite(isActive);
}

