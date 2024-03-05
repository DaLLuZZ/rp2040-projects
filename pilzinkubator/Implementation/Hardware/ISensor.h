//
// Created by steffen on 19.01.24.
//

#ifndef IMPLEMENTATION_ISENSOR_H
#define IMPLEMENTATION_ISENSOR_H

#include <utility>
#include <string>
#include <sstream>
#include "yahal_String.h"

class ISensor {
    String name;

protected:
    explicit ISensor(String name) : name(std::move(name)) {}
    virtual ~ISensor() = default;
public:
    ISensor() = delete;
    virtual double getValue() = 0;
    String ToString();
    String getName() { return name; }
};


#endif //IMPLEMENTATION_ISENSOR_H
