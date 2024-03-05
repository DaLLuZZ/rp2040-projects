//
// Created by steffen on 01.02.24.
//

#ifndef IMPLEMENTATION_GREENHOUSE_H
#define IMPLEMENTATION_GREENHOUSE_H

#include "gpio_rp2040.h"
#include "timer_rp2040.h"

#include "ws2812_rp2040.h"
#include "uart_rp2040.h"
#include "posix_io.h"
#include <cassert>
#include <simple_mutex.h>

#include "mutex.h"

#include "Hardware/Button.h"
#include "Hardware/OnOffSystem.h"
#include "Hardware/HumiditySensor.h"
#include "Hardware/MoistureSensor.h"
#include "DataLogger.h"
#include "MinMaxController.h"
#include "Hardware/TemperatureSensor.h"

#define PUMP_PIN 16
#define FAN_PIN 15

#define DHT_SDA 24
#define DHT_SCL 25
// I2C 0 depending on SDA and SCL!!! (See dht.h)

#define WS2812_PIN 29   // The GPIO pin controlling all the LEDs
#define LED_COUNT  8    // Number of LEDs

class Greenhouse {

    timer_rp2040 loggingTimer, configTimer;

    gpio_rp2040_pin statusLED;
    ws2812_rp2040 ledStripe;

    dht dhtsensor;

    OnOffSystem pump, fan;
    MoistureSensor moistureSensor;
    HumiditySensor humiditySensor;
    TemperatureSensor temperatureSensor;

    SDCard sd;
    DataLogger logger;

    MinMaxController moistureController, humidityController;

    bool hasSD = false;

    bool automaticMode = false;
    bool loggingOnSDCard = false;

    simple_mutex configCheckedMutex;
    bool configChecked = false;

    uint32_t loggingPeriod = 1000;

    void UpdateConfig();
    void UpdateSensors();
    void UpdateController();
    void UpdateVisuals();

    void LoggingCallback();
    void CheckConfig();

    void parseConfig(std::string& cfg);

public:
    Greenhouse();

    bool setAutomaticMode(bool state) {automaticMode = state; return automaticMode; }
    bool getAutomaticMode() const {return automaticMode;}

    bool setLogging(bool state) {loggingOnSDCard = state; return loggingOnSDCard;}
    bool getLogging() const {return loggingOnSDCard;}
    bool setLoggingPeriod(uint32_t ms) {
        loggingPeriod = ms;
        loggingTimer.setPeriod(loggingPeriod*1000, TIMER::PERIODIC);
        return loggingPeriod;
    }

    bool setPump(bool state) { pump.setState(state); return pump.getState(); }
    bool setFan(bool state) { fan.setState(state); return fan.getState(); }
    bool getPump() const {return pump.getState();}
    bool getFan() const {return fan.getState();}

    void setMinHumidity(double val) {humidityController.setMinValue(val);}
    void setMaxHumidity(double val) {humidityController.setMaxValue(val);}
    void setMinMoisture(double val) {moistureController.setMinValue(val);}
    void setMaxMoisture(double val) {moistureController.setMaxValue(val);}

    double getMinHumidity() const {return humidityController.getMinValue();}
    double getMaxHumidity() const {return humidityController.getMaxValue();}
    double getMinMoisture() const {return moistureController.getMinValue();}
    double getMaxMoisture() const {return moistureController.getMaxValue();}

    double getCurrentHumidity() {return humiditySensor.getValue();}
    double getCurrentMoisture() {return moistureSensor.getValue();}
    double getCurrentTemperature() {return temperatureSensor.getValue();}

    void readConfig();

    void Update();
};

#endif //IMPLEMENTATION_GREENHOUSE_H
