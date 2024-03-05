//
// Created by steffen on 19.01.24.
//

#include "MinMaxController.h"

bool MinMaxController::control(double processValue) {
    if(invertControl)
    {
        if(!controlValue && processValue > maxValue) {
            controlValue = true;
        }
        if(controlValue && processValue < minValue) {
            controlValue = false;
        }
    }
    else
    {
        if(controlValue && processValue > maxValue) {
            controlValue = false;
        }
        if(!controlValue && processValue < minValue) {
            controlValue = true;
        }
    }
    return controlValue;
}
