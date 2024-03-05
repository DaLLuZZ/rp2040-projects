//
// Created by steffen on 19.01.24.
//

#include "DataLogger.h"

void DataLogger::registerSensor(ISensor &p) {
    sensorList.push_back(&p);
}

void DataLogger::registerActor(IActor &p) {
    actorList.push_back(&p);
}

bool DataLogger::log(const String &title) {
    if(!sd.mount())
        return false;

    String content = title + "\n";
    for(auto &sensor : sensorList) {
        content += sensor->ToString();
    }
    for(auto &actor : actorList) {
        content += actor->ToString();
    }
    content += "\n";
    sd.writeFile(LOGGING_FILE, content, true);

    sd.unmount();
    return true;
}