//
// Created by LME on 17.12.2023.
//

#ifndef MCT_RP2040_STOPPUHR_H
#define MCT_RP2040_STOPPUHR_H

#include "timer_rp2040.h"

class Stoppuhr {
private:
    uint32_t ms;
    uint32_t accuracy;
    uint32_t limit;
    bool is_running;
    timer_rp2040 timer;
public:
    Stoppuhr(uint32_t accuracy1, uint32_t limit1, uint32_t index);

    void start();

    void stop();

    int getTime() const;

    void reset();

    bool isRunning() const;
};


#endif //MCT_RP2040_STOPPUHR_H
