//
// Created by steffen on 19.01.24.
//

#ifndef IMPLEMENTATION_IACTOR_H
#define IMPLEMENTATION_IACTOR_H

#include <utility>

#include "ISensor.h"

class IActor {
protected:
    String name;

    explicit IActor(String name) : name(std::move(name)) {}
    virtual ~IActor() = default;
public:
    IActor() = delete;
    virtual void setState(bool state) = 0;
    virtual bool getState() const = 0;

    String ToString() { return name + ":" + (getState() ? "On" : "Off") + "\n"; }
    String getName() { return name; }
};


#endif //IMPLEMENTATION_IACTOR_H
