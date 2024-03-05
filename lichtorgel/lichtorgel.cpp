#include <timer_rp2040.h>
#include <adc_rp2040.h>
#include <task.h>
#include "src/global_constants.h"
#include "src/view/view_controller.h"
#include "src/audio_fft/audio_analyzer.h"
#include "src/user_controller/user_control.h"

int main(void)
{
    view_controller v;
    audio_analyzer a;
    user_control u;

    timer_rp2040 * t = &a.timer_fft;
    q15_t (* data0) [FFT_SIZE] = &a.data_adc0;
    q15_t (* data1) [FFT_SIZE] = &a.data_adc1;

    //// I placed the callback function here, since I am bad at programming in C++ and could not make it work in a
    //// global scope. It is inspired by the example from the yahal example "mic_fft".
    t->setCallback([&]() {
        static int index = 0;
        // Read in ADC value
        uint16_t val = adc_rp2040::inst.adcReadRaw(6); // Aux
        // Copy value to correct buffer
        q15_t (* data_adc)[FFT_SIZE] = a.fill_data_adc0 ? data0 : data1;
        (*data_adc)[index++] = (val - 2048) << 4;
        // Check if all samples have been recorded
        if (index == FFT_SIZE) {
            t->stop();
            index = 0;
        }
    });

    u.button_decrease.gpioAttachIrq(GPIO::RISING, [&]() {
       v.decrease();
       a.timer_fft.start();
    });

    u.button_increase.gpioAttachIrq(GPIO::RISING, [&]() {
        v.increase();
        a.timer_fft.start();
    });

    u.button_next.gpioAttachIrq(GPIO::FALLING, [&]() {
        v.go_to_next();
        task::sleep(50);
        a.timer_fft.start();
    });

    //// Main loop
    while(1) {
        a.analyze(v);
        v.update_view();
    }

}
