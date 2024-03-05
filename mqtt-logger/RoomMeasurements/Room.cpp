#include <cmath>
#include <cstdio>
#include "Room.h"


#define LIGHT_SENSOR 0x44
#define TEMPERATURE_SENSOR 0x40

Room::Room() {
    // Set speed to 100kHz
    i2c.setSpeed(400000);

    // Configure the light sensor
    // Write the configuration register of the OPT3001: 0x01 = config register, 0xce = MSB (see data sheet), 0x10 = LSB (see data sheet)
    uint8_t lightBuf[] = {0x01, 0xce, 0x10};
    i2c.i2cWrite(LIGHT_SENSOR, lightBuf, 3);

    // Set the config register value to 0 (='Result')
    lightBuf[0] = 0x00;
    i2c.i2cWrite(LIGHT_SENSOR, lightBuf, 1);

    // Configure the temperature sensor
    // 0x01 = Register for the temperature that should be read next
    uint8_t temperatureBuf[] = {0x01};
    i2c.i2cWrite(TEMPERATURE_SENSOR, temperatureBuf, 1);
}

double Room::getBrightness() {
    // Read value from i2c and write it in rxbuf
    uint8_t rxbuf[2];
    i2c.i2cRead(LIGHT_SENSOR, rxbuf, 2);
    uint16_t raw = (rxbuf[0] << 8) + rxbuf[1];

    // isolate exponent: 12 bit rightshift, then set all the bits apart from the last 4 bits to 0
    uint8_t exponent = (raw >> 12) & 0x0F;

    // isolate mantissa: set all the bits apart from the last 12 bits to 0
    uint16_t mantissa = raw & 0x0FFF;

    // leftshift the number 1 by exponent positions in order to calculate pow(2, exponent)
    double result = (1 << exponent) * 0.01 * mantissa;
    return roundMeasurement(result, 2);
}

double Room::getTemperature() {
    // Read value from i2c and write it in rxbuf
    uint8_t rxbuf[2];
    i2c.i2cRead(TEMPERATURE_SENSOR, rxbuf, 2);
    uint16_t raw = (rxbuf[0] << 8) + rxbuf[1];

    // use reinterpret_cast to convert the two's complement
    int16_t interpretedData = *reinterpret_cast<int16_t *>(&raw);

    // two bit right shift (remove the last two LSBs)
    interpretedData >>= 2;

    // divide through 32 to get the temperature in Celsius
    double result = static_cast<float>(interpretedData) / 32.0;
    return roundMeasurement(result, 2);
}

String Room::toString() {
    String result;

    result += "Temperature: ";
    result += doubleToString(getTemperature(), 2);
    result += " °C\n";
    result += "Brightness: ";
    result += doubleToString(getBrightness(), 2);
    result += " Lux\n";

    return result;
}

void Room::printRoomMeasurements() {
    printf("%s", (toString() + "\n").c_str());
}

String doubleToString(double value, int decimals) {
    String s;
    auto factor = (int) std::pow(10.0, decimals);

    value *= factor;
    int integerPart = (int) value / factor;

    // Add - sign manually in case of 0 as integer part because otherwise negative sign will be lost
    if (value < 0 && integerPart == 0) {
        s += '-';
    }

    s += to_String(integerPart, 10);
    s += '.';

    int decimalPart = (int) value % factor;

    // make sure that the decimal part isn't negative for negative measurements values because of truncated modulo division
    if (decimalPart < 0) {
        decimalPart *= -1;
    }

    // add leading 0 of decimal part to the string
    while (factor > 10 && decimalPart < factor / 10) {
        s += "0";
        factor /= 10;
    }

    s += to_String(decimalPart, 10);

    return s;
}

double roundMeasurement(double value, int decimals) {
    auto factor = (int) std::pow(10.0, decimals);
    double roundedNumber = std::round(value * factor) / factor;
    return roundedNumber;
}