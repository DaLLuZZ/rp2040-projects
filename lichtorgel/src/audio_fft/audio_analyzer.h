#ifndef LICHTORGEL_PROJEKT_MC_AUDIO_ANALYZER_H
#define LICHTORGEL_PROJEKT_MC_AUDIO_ANALYZER_H

#include "../view/view_controller.h"
#include <gpio_rp2040.h>
#include <timer_rp2040.h>
#include <arm_math_types.h>
#include <adc_rp2040.h>
#include "arm_math.h"
#include "arm_const_structs.h"

#define SAMPLE_FREQUENCY 8000
#define AUX_PIN 19

class audio_analyzer {
public:
    audio_analyzer();
    void analyze(view_controller &view);

    timer_rp2040 timer_fft;
    gpio_rp2040_pin aux_input_pin;

    q15_t data_adc0  [FFT_SIZE]{};     // ADC data 0
    q15_t data_adc1  [FFT_SIZE]{};     // ADC data 1
    bool fill_data_adc0 = true;        // Flag controlling which buffer is written to

private:
    q15_t data_hann  [FFT_SIZE]{};     // Hanning window function
    q15_t data_fft   [FFT_SIZE * 2]{}; // Complex FFT result
    q15_t data_result[FFT_SIZE]{};     // Magnitudes of FFT result
    arm_rfft_instance_q15 instance{};    // Instance for running fft
};

#endif //LICHTORGEL_PROJEKT_MC_AUDIO_ANALYZER_H
