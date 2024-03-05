//
// Created by steffen on 25.02.24.
//

#include "Helper.h"

String Helper::doubleToString(double value) {
    int nachkomma =(int)(value * 10) % 10;
    int vorkomma = (int)(value);
    std::stringstream ss;
    ss << std::to_string(vorkomma) << "." << nachkomma;
    return { ss.str().c_str() };
}
