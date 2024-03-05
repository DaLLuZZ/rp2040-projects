//
// Created by steffen on 19.01.24.
//

#include "MoistureSensor.h"
#include <cmath>

// Calibration
#define MOISTURE_ZERO 3660.0
#define MOISTURE_HUNDRED 3881.0


double MoistureSensor::getValue() {
    uint16_t raw = sensorpin.adcReadRaw(); // Value between 0..4095
    double value = ((4095-raw) - MOISTURE_ZERO) / (MOISTURE_HUNDRED - MOISTURE_ZERO);
    return value * 100.0; // Output in %
}