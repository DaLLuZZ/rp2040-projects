#include "view_controller.h"

view_controller::view_controller() : _backlight(PIN_BACKLIGHT), _lcd_cs(PIN_LCD_CS), _lcd_rst(PIN_RESET),
    _lcd_dc(PIN_DC), _spi(0, 4, 7, 22, _lcd_cs), _lcd(_spi, _lcd_rst,
 _lcd_dc, LCD_SIZE), _gui(_lcd) {

    //// Setup the focusable ui element array
    ui_components[0] = &region_size;
    ui_components[1] = &color_randomness;
    ui_components[2] = &color_low;
    ui_components[3] = &color_middle;
    ui_components[4] = &color_high;

    setup_lcd();
    setup_view();
}

//// Initialize spi protocol to control the lcd
void view_controller::setup_lcd() {
    _backlight.gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);
    _lcd.clearScreen(DEFAULT_BACKGROUND_COLOR);

    _spi.setSpeed(24000000);
    _spi.generateCS(false);
    _spi.setCS(LOW);

    _gui.SetForecolor(DEFAULT_TEXT_COLOR);
    _gui.SetBackcolor(DEFAULT_BACKGROUND_COLOR);
}

//// Draw the ui components
void view_controller::setup_view() {
    fft_view.setup();
    leds_view.setup();

    for(int i = 0; i < 5; i++) {
        ui_components[i]->setup();
    }
    ui_components[current_active]->activate();
    update_led_settings_by_ui();
}

//// Update the view for all ui_component children
void view_controller::update_view() {
    fft_view.update();
    leds_view.update();

    for(int i = 0; i < 5; i++) {
        ui_components[i]->update();
    }
}

//// Forward the fft data to all components needing it
void view_controller::set_fft_data(q15_t fft_data[512]) {
    fft_view.set_data(fft_data);
    leds_view.set_data(fft_data);
}

//// Decrease the value of the currently active ui element
void view_controller::decrease() {
    ui_components[current_active]->decrease();
    update_led_settings_by_ui();
}

//// Increase the value of the currently active ui element
void view_controller::increase() {
    ui_components[current_active]->increase();
    update_led_settings_by_ui();
}

//// Change the active state to the next ui element
void view_controller::go_to_next() {
    ui_components[current_active]->deactivate();
    current_active++;
    if (current_active >= focusable_ui_components_count) {
        current_active = 0;
    }
    ui_components[current_active]->activate();
}

//// Send the new settings toward the led controller
void view_controller::update_led_settings_by_ui() {
    int region_s = std::max(1, (int) region_size.get_value());
    float randomness = color_randomness.get_value();
    unsigned int c_lf = color_low.get_value();
    unsigned int c_mf = color_middle.get_value();
    unsigned int c_hf = color_high.get_value();
    leds_view.update_led_controller_data(region_s, randomness, c_lf, c_mf, c_hf);
}
