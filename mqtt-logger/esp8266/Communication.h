#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Adafruit_MQTT.h>

/**
 * Create an ESP8266 WiFiClient class to connect to the MQTT server.
 */
WiFiClient client;

/**
 * Function to connect and reconnect as necessary to the MQTT server.
 * Should be called in the loop function, and it will take care if connecting.
 */
void MQTT_connect();

/**
 * Publishes the new measurement value.
 * @param value The measurement value.
 * @param publisher The mqtt publisher for the measurement value.
 */
void PublishValue(double value, Adafruit_MQTT_Publish &publisher);

#endif //COMMUNICATION_H
