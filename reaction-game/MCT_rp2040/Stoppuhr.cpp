//
// Created by LME on 17.12.2023.
//

#include "Stoppuhr.h"

Stoppuhr::Stoppuhr(uint32_t accuracy1, uint32_t limit1, uint32_t index) : ms(0), accuracy(accuracy1), limit(limit1),
                                                                          is_running(false), timer(index) {
    timer.setPeriod(accuracy, TIMER::PERIODIC);
    timer.setCallback([&]() {
        ms++;
        if (ms == limit) {
            stop();
        }
    });
}

void Stoppuhr::start() {
    if (!is_running) { //sonst wurde sie bereits gestartet
        timer.start();
        is_running = true;
    }
}

void Stoppuhr::stop() {
    if (is_running) { //sonst wurde sie bereits gestoppt
        timer.stop();
        is_running = false;
    }
}

int Stoppuhr::getTime() const {
    return ms;
}

void Stoppuhr::reset() {
    ms = 0;
}

bool Stoppuhr::isRunning() const {
    return is_running;
}