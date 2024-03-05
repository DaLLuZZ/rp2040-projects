#include "audio_analyzer.h"

audio_analyzer::audio_analyzer() : timer_fft(0), aux_input_pin(AUX_PIN){
    //// Setup gpio pin for aux input
    aux_input_pin.gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);
    //// Setup timer
    timer_fft.setPeriod(1000000/SAMPLE_FREQUENCY, TIMER::PERIODIC);
    //// Initialize Hann Window
    for(int i = 0; i < FFT_SIZE; i++) {
        data_hann[i] = 16384.0 * (1.0 - cosf((2 * PI * i) / (FFT_SIZE - 1)));
    }
    //// Setup the q15 instance for running the FFT
    arm_rfft_init_q15(&instance, FFT_SIZE, 0, 1);
}

//// This method was inspired by the the yahal example "mic_fft".
void audio_analyzer::analyze(view_controller &view) {
    //// Wait for the ADC-sampling to finish
    while(timer_fft.isRunning());

    //// Switch the buffer to fill
    fill_data_adc0 ^= true;
    timer_fft.start();

    //// Get the buffer which is NOT filled by the ADC
    q15_t * data_adc = fill_data_adc0 ? data_adc1 : data_adc0;

    //// Compute the real FFT result, utilizing shifts
    arm_rfft_q15 (&instance, data_adc, data_fft);
    arm_shift_q15(data_fft, 8, data_fft, FFT_SIZE * 2);

    //// Calculate magnitude of FFT output
    arm_cmplx_mag_q15(data_fft, data_result, FFT_SIZE);

    //// Send the result to the visualization
    view.set_fft_data(data_result);
}

