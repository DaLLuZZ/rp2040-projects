//
// Created by steffen on 01.02.24.
//

#include "Greenhouse.h"
#include "Helper.h"

Greenhouse::Greenhouse() : loggingTimer(0),
                           configTimer(1),
                           statusLED(13),
                           ledStripe(WS2812_PIN, LED_COUNT),
                           dhtsensor(DHT_SDA, DHT_SCL),
                           pump("Wasserpumpe", PUMP_PIN),
                           fan("Lüfter", FAN_PIN),
                           moistureSensor("Feuchte Boden [%]", 0),
                           humiditySensor("Feuchte Luft [%]", dhtsensor),
                           temperatureSensor("Temperatur [°C]", dhtsensor),
                           logger(sd),
                           moistureController(25.0,75.0, true),
                           humidityController(60.0,75.0, true)
{
    statusLED.gpioMode(GPIO::OUTPUT);
    loggingTimer.setCallback([this]() {this->LoggingCallback(); statusLED = !statusLED; });
    loggingTimer.setPeriod(1000000, TIMER::PERIODIC);

    configTimer.setCallback([this]() {this->CheckConfig();});
    configTimer.setPeriod(1000000, TIMER::ONE_SHOT);

    logger.registerActor(pump);
    logger.registerActor(fan);
    logger.registerSensor(moistureSensor);
    logger.registerSensor(humiditySensor);
    logger.registerSensor(temperatureSensor);
}

void Greenhouse::UpdateController() {
    if(automaticMode) {
        this->setPump(moistureController.control(getCurrentMoisture()));
        this->setFan(humidityController.control(getCurrentHumidity()));
    }
}

void Greenhouse::UpdateVisuals() {
    ledStripe[0] = getPump() ? 0x001000 : 0x100000;
    ledStripe[1] = getFan() ? 0x001000 : 0x100000;
    ledStripe[7] = hasSD ? loggingOnSDCard ? 0x000010 : 0x001000 : 0x100000;
    ledStripe[6] = configChecked ? 0x001000 : 0x100000;
}

void Greenhouse::UpdateConfig() {
    if(configCheckedMutex.try_lock())
    {
        configChecked = hasSD && configChecked;
        configCheckedMutex.unlock();
    }
    if(hasSD && !configChecked && !configTimer.isRunning())
    {
        configTimer.start();
    }
    else if(!hasSD && configTimer.isRunning())
    {
        configTimer.stop();
    }
}

void Greenhouse::CheckConfig() {
    readConfig();
    configCheckedMutex.lock();
    configChecked = true;
    configCheckedMutex.unlock();
}

void Greenhouse::Update() {
    hasSD = sd.hasSDCard();

    if(loggingOnSDCard && !loggingTimer.isRunning())
    {
        loggingTimer.setPeriod(loggingPeriod*1000, TIMER::PERIODIC);
        loggingTimer.start();
    }
    if(!loggingOnSDCard && loggingTimer.isRunning())
    {
        loggingTimer.setPeriod(100, TIMER::ONE_SHOT);
    }

    UpdateConfig();
    UpdateSensors();
    UpdateController();
    //UpdateVisuals();
}

void Greenhouse::LoggingCallback() {
    YAHAL::String msg = "Measurement\n";
    this->logger.log(msg);
}

void Greenhouse::readConfig() {
    if(hasSD) {
        sd.mount();
        String cfg = sd.readFile("config.txt");
        sd.unmount();
        if(!cfg.empty())
        {
            printf("Config-File:\n");
            std::string s(cfg.c_str());
            parseConfig(s);
        }
    }
    else
    {
        printf("Keine SD-Karte gefunden!\n");
    }
}

void Greenhouse::parseConfig(std::string& cfg) {
    std::istringstream is(cfg);
    while(!is.eof())
    {
        std::string line;
        std::getline(is, line);
        std::istringstream ls(line);
        while(!ls.eof())
        {
            std::string cmd;
            std::getline(ls, cmd, ':');
            if(cmd.empty())
                continue;
            std::string val = ls.str().substr(cmd.size() + 1);

            if(cmd == "automode")
            {
                setAutomaticMode(val == "1");
                printf("Set %s to %s\n", cmd.c_str(), (val == "1" ? "On" : "Off"));
            }
            else if(cmd == "logging")
            {
                setLogging(val == "1");
                printf("Set %s to %s\n", cmd.c_str(), (val == "1" ? "On" : "Off"));
            }
            else if(cmd == "loggingperiod")
            {
                setLoggingPeriod(std::stoi(val));
                printf("Set %s to %d\n", cmd.c_str(), std::stoi(val));
            }
            else if(cmd == "minhumidity")
            {
                setMinHumidity(std::stod(val));
                printf("Set %s to %s\n", cmd.c_str(), Helper::doubleToString(std::stod(val)).c_str());
            }
            else if(cmd == "maxhumidity")
            {
                setMaxHumidity(std::stod(val));
                printf("Set %s to %s\n", cmd.c_str(), Helper::doubleToString(std::stod(val)).c_str());
            }
            else if(cmd == "minmoisture")
            {
                setMinMoisture(std::stod(val));
                printf("Set %s to %s\n", cmd.c_str(), Helper::doubleToString(std::stod(val)).c_str());
            }
            else if(cmd == "maxmoisture")
            {
                setMaxMoisture(std::stod(val));
                printf("Set %s to %s\n", cmd.c_str(), Helper::doubleToString(std::stod(val)).c_str());
            }
            else if(cmd == "fan")
            {
                setFan(val == "1");
                printf("Set %s to %s\n", cmd.c_str(), (val == "1" ? "On" : "Off"));
            }
            else if(cmd == "pump")
            {
                setPump(val == "1");
                printf("Set %s to %s\n", cmd.c_str(), (val == "1" ? "On" : "Off"));
            }
        }
    }
}

void Greenhouse::UpdateSensors() {
    dhtsensor.updateSensor();
}
