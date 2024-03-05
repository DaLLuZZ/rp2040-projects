//
// Created by steffen on 19.01.24.
//

#include "HumiditySensor.h"

double HumiditySensor::getValue() {
    return sensor.getHumidity();
}