//
// Created by steffen on 25.02.24.
//

#include "TemperatureSensor.h"

double TemperatureSensor::getValue() {
    return sensor.getTemperature();
}
