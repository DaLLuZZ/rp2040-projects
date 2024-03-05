#include "leds.h"

using namespace std;

//// Saves the data given by the fft
void leds::set_data(const q15_t *d) {
    for(int i = 0; i < 512; i++) {
        data[i] = d[i];
    }
}

//// Returns the transformed colors for the visualization
//// Since I cant
unsigned int * leds::get_colors() {
    timer_counter++;
    if (timer_counter == 4) {
        transform_fft_to_colors();
        timer_counter = 0;
    }
    return colors;
}

//// Transform the fft data to rgb colors
void leds::transform_fft_to_colors() {
    //// Calculate the mean values for the different frequency regions
    //// Low freq
    double mean_low = 0.0;
    for(int i = 0; i < LOW_FREQ_TOP_INDEX; i++) {
        mean_low += data[i];
    }
    mean_low = mean_low / low_freq_region_size;

    //// Middle freq
    double mean_middle = 0.0;
    for(int i = LOW_FREQ_TOP_INDEX; i < MIDDLE_FREQ_TOP_INDEX; i++) {
        mean_middle += data[i];
    }
    mean_middle = mean_middle / middle_freq_region_size;

    //// High freq
    double mean_top = 0.0;
    for(int i = MIDDLE_FREQ_TOP_INDEX; i < FFT_SIZE; i++) {
        mean_top += data[i];
    }
    mean_top = mean_top / top_freq_region_size;


    int current_freq_region = -1;
    unsigned int color_to_use;
    //// Set the different colors for each led region
    for(int i = 0; i < LED_COUNT; i++) {
        int freq_region_to_use = (i / region_size) % 3;
        if(current_freq_region != freq_region_to_use) {
            current_freq_region = freq_region_to_use;
            //// Change the color
            //// Low frequency
            if (freq_region_to_use == 0) {
                color_to_use= get_color_by_mean(mean_low, LOW_FREQ_MEAN_AMPLITUDE, low_freq_c);
            }
                //// Middle frequency
            else if (freq_region_to_use == 1) {
                color_to_use= get_color_by_mean(mean_middle, MIDDLE_FREQ_MEAN_AMPLITUDE, middle_freq_c); ;
            }
                //// High frequency
            else {
                color_to_use= get_color_by_mean(mean_top, TOP_FREQ_MEAN_AMPLITUDE, high_freq_c);
            }
        }
        colors[i] = color_to_use;
    }
}

//// Calculates the color by the mean amplitude in a given frequency region
//// Shifts the color by a random factor, which size is defined by the ui
unsigned int leds::get_color_by_mean(double mean, float max, unsigned int c) {
    double percental_value = mean / max;

    if (percental_value < BOTTOM_THRESHOLD) {
        percental_value = 0;
    }
    //// Get the values from each channel (r, g, b)
    unsigned char red = (c >> 16) & 0xFF;
    unsigned char green = (c >> 8) & 0xFF;
    unsigned char blue = c & 0xFF;

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    //// Calculate the random shifts
    float rand_r = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2.0)) - 1.0) * random_color_factor;
    float rand_g = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2.0)) - 1.0) * random_color_factor;
    float rand_b = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2.0)) - 1.0) * random_color_factor;


    unsigned char red_shift = 0;
    unsigned char green_shift = 0;
    unsigned char blue_shift = 0;
    if (percental_value >= BOTTOM_THRESHOLD) {
        red_shift = rand_r * RANDOMNESS_AMPLITUDE;
        green_shift = rand_g * RANDOMNESS_AMPLITUDE;
        blue_shift = rand_b * RANDOMNESS_AMPLITUDE;
    }

    //// Calculate the new values by the percental value
    red = static_cast<unsigned char>(red * percental_value) + red_shift;
    green = static_cast<unsigned char>(green * percental_value) + green_shift;
    blue = static_cast<unsigned char>(blue * percental_value) + blue_shift;

    auto minC = static_cast<unsigned char>(0);
    auto maxC = static_cast<unsigned char>(255);

    //// Reassemble the unsigned int color code
    return (std::clamp(red, minC, maxC) << 16) | (std::clamp(green, minC, maxC) << 8) | std::clamp(blue, minC, maxC);
}

void leds::set_settings(int region_s, float color_randomness, unsigned int low_freq_color, unsigned int mid_freq_color,
                   unsigned int high_freq_color) {
    this->region_size = region_s;
    this->random_color_factor = color_randomness;
    this->low_freq_c = low_freq_color;
    this->middle_freq_c = mid_freq_color;
    this->high_freq_c = high_freq_color;
}
