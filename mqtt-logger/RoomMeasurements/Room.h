#ifndef ROOM_MEASUREMENTS_H
#define ROOM_MEASUREMENTS_H

#include <i2c_rp2040.h>
#include "yahal_String.h"


class Room {
private:
    // Initialize I2C port
    i2c_rp2040 i2c = i2c_rp2040(0, 24, 25, 0);

public:
    Room();

    /**
     * Method to get the brightness of the sensor from the Educational Boosterpack MKII.
     * @return The current brightness measurement value as a double rounded on two decimal places.
     */
    [[nodiscard]] double getBrightness();

    /**
     * Method to get the brightness of the sensor from the Educational Boosterpack MKII.
     * @return The current temperature measurement value as a double rounded on two decimal places.
     */
    [[nodiscard]] double getTemperature();

    /**
     * Method to get the room object with the current measurement values as a string.
     * @return All room measurement values as a string.
     */
    String toString();

    /**
     * Method to directly print the room measurement values using printf.
     */
    void printRoomMeasurements();
};

/**
 * Method to convert the measurement value into a string.
 * @param value The measurement value.
 * @param decimals The number of decimal places of the measurement.
 * @return The measurement value as a string.
 */
String doubleToString(double value, int decimals);

/**
 * Method to round the measurement value to a double with a certain number of decimal places.
 * @param value The measurement value.
 * @param decimals The number of decimal places of the rounded double.
 * @return The rounded value with the given number of decimal places.
 */
double roundMeasurement(double value, int decimals);

#endif  // ROOM_MEASUREMENTS_H