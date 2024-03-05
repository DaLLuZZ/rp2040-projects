//
// Created by steffen on 16.02.24.
//

#include <task.h>
#include <cstdio>
#include <cmath>
#include "dht.h"

dht::dht(uint16_t sda, uint16_t scl) : i2c(0, sda, scl, 0) {
    task::sleep(100);

    i2c.setSpeed(50000);

    uint8_t txbuf = 0x71;
    i2c.i2cWrite(address, &txbuf, 1);
    uint8_t rxbuf[2];
    i2c.i2cRead(address, rxbuf, 2);
    if((rxbuf[0] & 0x18) != 0x18) {
        printf("Initialization of DHT necessary!!!\n");
    }
    else
    {
        printf("DHT Initialization finished!\n");
    }
    task::sleep(10);
}

void dht::updateSensor() {
    uint8_t txbuf[] = {0xAC, 0x33, 0x00};
    i2c.i2cWrite(address, txbuf, 3);
    task::sleep(80);
    uint8_t rxbuf[7];
    i2c.i2cRead(address, rxbuf, 1);
    if(!(rxbuf[0] & (1 << 7))) {
        // Measurement completed
        i2c.i2cRead(address, rxbuf, 7);

        uint32_t hum_bits = (rxbuf[1] << 12) | (rxbuf[2] << 4) | (rxbuf[3] >> 4);
        uint32_t temp_bits = ((rxbuf[3] & 0b00001111) << 16) | (rxbuf[4] << 8) | rxbuf[5];

        temperature = (temp_bits / std::pow(2, 20)) * 200 - 50; // In Celsius
        humidity = (hum_bits / std::pow(2,20)) * 100; // in %
    }
}