//
// Created by steffen on 19.01.24.
//
//
// Created by steffen on 06.10.23.
//
#include "Greenhouse.h"
#include "UartController.h"

#include "i2c_rp2040.h"
#include "Sensors/dht.h"

int main() {

    Greenhouse house;
    UartController controller;

    controller.registerCommand("enablelogging", std::function<bool()>([&]() -> bool { return house.setLogging(true); }));
    controller.registerCommand("disablelogging", std::function<bool()>([&]() -> bool { return house.setLogging(false); }));
    controller.registerCommand("logging", std::function<bool()>([&]() -> bool { return house.getLogging(); }));
    controller.registerCommand("setloggingperiod", std::function<void(double)>([&](double ms) { return house.setLoggingPeriod((int)ms); }));

    controller.registerCommand("enableautomode", std::function<bool()>([&]() -> bool { return house.setAutomaticMode(true);}));
    controller.registerCommand("disableautomode", std::function<bool()>([&]() -> bool { return house.setAutomaticMode(false);}));
    controller.registerCommand("automode", std::function<bool()>([&]() -> bool { return house.getAutomaticMode();}));

    controller.registerCommand("gethumidity", std::function<double()>([&]() -> double { return house.getCurrentHumidity();}));
    controller.registerCommand("getmoisture", std::function<double()>([&]() -> double { return house.getCurrentMoisture();}));
    controller.registerCommand("gettemperature", std::function<double()>([&]() -> double { return house.getCurrentTemperature();}));

    controller.registerCommand("setminhumidity", std::function<void(double)>([&](double val) { house.setMinHumidity(val);}));
    controller.registerCommand("setmaxhumidity", std::function<void(double)>([&](double val) { house.setMaxHumidity(val);}));
    controller.registerCommand("setminmoisture", std::function<void(double)>([&](double val) { house.setMinMoisture(val);}));
    controller.registerCommand("setmaxmoisture", std::function<void(double)>([&](double val) { house.setMaxMoisture(val);}));

    controller.registerCommand("getminhumidity", std::function<double()>([&]() -> double { return house.getMinHumidity(); }));
    controller.registerCommand("getmaxhumidity", std::function<double()>([&]() -> double { return house.getMaxHumidity(); }));
    controller.registerCommand("getminmoisture", std::function<double()>([&]() -> double { return house.getMinMoisture(); }));
    controller.registerCommand("getmaxmoisture", std::function<double()>([&]() -> double { return house.getMaxMoisture(); }));

    controller.registerCommand("enablefan"  , std::function<bool()>([&]() { return house.setFan(true);}));
    controller.registerCommand("disablefan" , std::function<bool()>([&]() { return house.setFan(false);}));
    controller.registerCommand("fan"  , std::function<bool()>([&]() { return house.getFan();}));

    controller.registerCommand("enablepump" , std::function<bool()>([&]() { return house.setPump(true);}));
    controller.registerCommand("disablepump", std::function<bool()>([&]() { return house.setPump(false);}));
    controller.registerCommand("pump", std::function<bool()>([&]() { return house.getPump();}));

    controller.registerCommand("readconfig", std::function<void()>([&]() { house.readConfig(); }));
    controller.registerCommand("help", std::function<void()>([&]()  { controller.helpCommand(); }));

    controller.start();

    while(true) {
        house.Update();
    }
}