#ifndef LICHTORGEL_PROJEKT_MC_VIEW_CONTROLLER_H
#define LICHTORGEL_PROJEKT_MC_VIEW_CONTROLLER_H

#include <gpio_rp2040.h>
#include <spi_rp2040.h>
#include <st7735s_drv.h>
#include <arm_math_types.h>
#include "uGUI.h"

#include "ui_constants.h"
#include "UI_components/fft_ui_components/fft_ui_component.h"
#include "UI_components/leds_ui_components/leds_ui_component.h"
#include "UI_components/settings_ui_component/led_settings_bar.h"
#include "UI_components/settings_ui_component/freq_color_bar.h"

#define PIN_BACKLIGHT 13
#define PIN_LCD_CS 5
#define PIN_RESET 9
#define PIN_DC 2
#define LCD_SIZE st7735s_drv::Crystalfontz_128x128

class view_controller {
public:
    view_controller();
    void update_view();
    void set_fft_data(q15_t fft_data[512]);

    void increase();
    void decrease();
    void go_to_next();
private:
    //// GPIO Pins
    gpio_rp2040_pin _backlight;
    gpio_rp2040_pin _lcd_cs;
    gpio_rp2040_pin _lcd_rst;
    gpio_rp2040_pin _lcd_dc;

    //// Properties controlling the ui
    spi_rp2040 _spi;
    st7735s_drv _lcd;
    uGUI _gui;

    //// Setup methods
    void setup_lcd();
    void setup_view();

    //// UI components
    fft_ui_component fft_view{_gui, 1, 1, 126, 48};
    leds_ui_component leds_view{_gui, 1, 51, 126, 10};

    int tuning_width = 68;
    led_settings_bar region_size{"Region size", LED_COUNT / 3.0, _gui, 1, 65, tuning_width, 13};
    led_settings_bar color_randomness{"Color rand", 1.0, _gui, 1, 80, tuning_width, 13};
    freq_color_bar color_low{"Low freq", 768, _gui, 1, 94, tuning_width, 13};
    freq_color_bar color_middle{"Middle freq", 768, _gui, 1, 104, tuning_width, 13};
    freq_color_bar color_high{"High freq", 100, _gui, 1, 114, tuning_width, 13};

    int focusable_ui_components_count = 5;
    ui_bar_component** ui_components = new ui_bar_component*[focusable_ui_components_count];
    int current_active = 0;

    void update_led_settings_by_ui();
};

#endif //LICHTORGEL_PROJEKT_MC_VIEW_CONTROLLER_H
