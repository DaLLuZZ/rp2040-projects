//
// Created by steffen on 19.01.24.
//


#include "ISensor.h"
#include "../Helper.h"

String ISensor::ToString() {
    double value = getValue();
    std::stringstream ss;
    ss << getName().c_str() << ": " << Helper::doubleToString(value).c_str() << std::endl;
    return { ss.str().c_str() };
}
