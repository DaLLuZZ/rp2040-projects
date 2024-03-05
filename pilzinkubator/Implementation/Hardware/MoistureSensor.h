//
// Created by steffen on 19.01.24.
//

#ifndef IMPLEMENTATION_MOISTURESENSOR_H
#define IMPLEMENTATION_MOISTURESENSOR_H

#include "adc_rp2040.h"
#include "ISensor.h"

class MoistureSensor : public ISensor{

    adc_rp2040_channel sensorpin;

public:
    explicit MoistureSensor(String name, uint16_t analogpin) : ISensor(std::move(name)), sensorpin(analogpin) {}
    double getValue() override;
};


#endif //IMPLEMENTATION_MOISTURESENSOR_H
