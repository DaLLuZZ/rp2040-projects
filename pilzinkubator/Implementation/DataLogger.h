//
// Created by steffen on 19.01.24.
//

#ifndef IMPLEMENTATION_DATALOGGER_H
#define IMPLEMENTATION_DATALOGGER_H

#include "Hardware/SDCard.h"
#include "Hardware/ISensor.h"
#include "Hardware/IActor.h"
#include <vector>
#include "yahal_String.h"

#include "task.h"

#define LOGGING_FILE "log.txt"

class DataLogger {

    std::vector<ISensor*> sensorList;
    std::vector<IActor*> actorList;
    SDCard sd;

public:
    explicit DataLogger(SDCard& sd) : sd(sd) {}

    void registerSensor(ISensor &p);
    void registerActor(IActor &p);
    bool hasSDCard() {return sd.hasSDCard();}
    bool log(const String &title);
};


#endif //IMPLEMENTATION_DATALOGGER_H
