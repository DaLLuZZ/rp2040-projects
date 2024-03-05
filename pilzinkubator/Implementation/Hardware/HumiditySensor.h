//
// Created by steffen on 19.01.24.
//

#ifndef IMPLEMENTATION_HUMIDITYSENSOR_H
#define IMPLEMENTATION_HUMIDITYSENSOR_H

#include "ISensor.h"
#include "../Sensors/dht.h"

class HumiditySensor : public ISensor {

    dht& sensor;

public:
    explicit HumiditySensor(String name, dht& sensor) : ISensor(std::move(name)), sensor(sensor) {}
    double getValue() override;
};


#endif //IMPLEMENTATION_HUMIDITYSENSOR_H
