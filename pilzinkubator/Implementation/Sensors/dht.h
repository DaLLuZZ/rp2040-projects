//
// Created by steffen on 16.02.24.
//

#ifndef IMPLEMENTATION_DHT_H
#define IMPLEMENTATION_DHT_H

#include "i2c_rp2040.h"

class dht {

    i2c_rp2040 i2c;
    uint16_t address = 0x38;

    double temperature, humidity;

public:
    dht(uint16_t sda, uint16_t scl);

    [[nodiscard]] double getTemperature() const { return temperature; }
    [[nodiscard]] double getHumidity() const { return humidity; }
    void updateSensor();
};


#endif //IMPLEMENTATION_DHT_H
