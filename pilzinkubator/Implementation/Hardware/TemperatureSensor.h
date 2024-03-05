//
// Created by steffen on 25.02.24.
//

#ifndef IMPLEMENTATION_TEMPERATURESENSOR_H
#define IMPLEMENTATION_TEMPERATURESENSOR_H

#include "ISensor.h"
#include "../Sensors/dht.h"

class TemperatureSensor : public ISensor {

    dht& sensor;

public:
    explicit TemperatureSensor(String name, dht& sensor) : ISensor(std::move(name)), sensor(sensor) {}
    double getValue() override;
};


#endif //IMPLEMENTATION_TEMPERATURESENSOR_H
