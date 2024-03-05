//Silas Schaprian
//Matrikelnummer: 3520242

#ifndef MCT_PCM_PLAYER_H
#define MCT_PCM_PLAYER_H

#include "pcm_pwm_rp2040_drv.h"
#include <cstdint>

/**
 *  Class to help interacting with pcm_driver
 */
class PCM_Player {
private:
    pcm_pwm_rp2040_drv pcm_if;
public:

    explicit PCM_Player(gpio_pin_t left = 9, gpio_pin_t right = 11, uint32_t sampleRate = 44100) : pcm_if(left, right) {
        pcm_if.setPcmRate(sampleRate);
    }

    bool canPut() {
        return pcm_if.pcmFifoAvailablePut();
    }

    void put(pcm_value_t &pcmValue) {
        pcm_if.pcmFifoPut(pcmValue);
    }

    void timer_reset(){
        pcm_if.timer_reset();
    }

};

#endif //MCT_PCM_PLAYER_H
