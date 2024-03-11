//
// Created by LME on 03.03.2024.
//

#include "PlaySound.h"
#include <math.h>

PlaySound::PlaySound() : delay_us(0), sum_us(0), buzzer(14), tone_timer(1) {
    buzzer.gpioMode(GPIO::OUTPUT);
}

// Ton spielen
void PlaySound::playTone(TONE t, int ms) {
    delay_us = calculateDelay_us(t);
    sum_us = 0;
    tone_timer.setPeriod(delay_us, TIMER::PERIODIC);
    tone_timer.setCallback([this]() {
        buzzer.gpioToggle();
        sum_us += delay_us;
    });
    buzzer.gpioMode(t == P ? GPIO::INPUT : GPIO::OUTPUT);
    tone_timer.start();
    while (sum_us / 1000 < ms);
    tone_timer.stop();
}

// Melodie spielen
void PlaySound::playMelody(ENTRY t[], int scale) {
    int index = 0;
    while (t[index].ms) {
        playTone(t[index].t, t[index].ms * scale);
        index++;
    }
}

// Frequenz für einen Ton berechnen
int PlaySound::calculateDelay_us(TONE t) {
    float exponent = ((float) t - 40) / 12.0;
    float cnt = pow(2, exponent) * 440.0;
    return (int) (1000000 / cnt / 2);
}
