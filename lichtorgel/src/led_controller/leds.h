#ifndef LICHTORGEL_PROJEKT_MC_LEDS_H
#define LICHTORGEL_PROJEKT_MC_LEDS_H

#include "../global_constants.h"
#include <ctime>
#include <arm_math_types.h>
#include <experimental/random>
#include <algorithm>

#define LOW_FREQ_TOP_INDEX 171
#define MIDDLE_FREQ_TOP_INDEX 342

#define LOW_FREQ_MEAN_AMPLITUDE 1000.0
#define MIDDLE_FREQ_MEAN_AMPLITUDE 1000.0
#define TOP_FREQ_MEAN_AMPLITUDE 1000.0

#define BOTTOM_THRESHOLD 0.3
#define RANDOMNESS_AMPLITUDE 8.0;

class leds {
public:
    void set_data(const q15_t d [512]);
    void set_settings(int region_size, float color_randomness, unsigned int low_freq_color, unsigned int mid_freq_color, unsigned int high_freq_color);
    unsigned int * get_colors();
private:
    q15_t data [FFT_SIZE]{};
    unsigned int colors [LED_COUNT] {};

    void transform_fft_to_colors();
    unsigned int get_color_by_mean(double mean, float max, unsigned int c);

    //// Set the regions which are represented by the leds
    int low_freq_region_size = LOW_FREQ_TOP_INDEX;
    int middle_freq_region_size = MIDDLE_FREQ_TOP_INDEX - LOW_FREQ_TOP_INDEX;
    int top_freq_region_size = FFT_SIZE - MIDDLE_FREQ_TOP_INDEX;

    //// Default values, if no ui is used
    int region_size = 1;
    float random_color_factor = 0;
    unsigned int low_freq_c = 0xFF0000;
    unsigned int middle_freq_c = 0x00FF00;
    unsigned int high_freq_c = 0x0000FF;

    uint8_t timer_counter = 0;
};

#endif //LICHTORGEL_PROJEKT_MC_LEDS_H
