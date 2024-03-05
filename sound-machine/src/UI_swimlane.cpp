//
// Created by Florian Bremm on 19.01.24.
//
#include "gpio_rp2040.h"
#include "spi_rp2040.h"
#include "ssd2119_drv.h"
#include "uGUI.h"
#include "uGUI_colors.h"
#include "font_4x6.h"
#include "font_5x8.h"
#include "font_5x12.h"
#include "font_6x8.h"
#include "font_6x10.h"
#include "font_7x12.h"
#include "font_8x12.h"
#include "adc_rp2040.h"
#include "yahal_String.h"
#include "task.h"
#include <cstdlib>

#include <cstdio>
#include "uart_rp2040.h"
#include "posix_io.h"

//using namespace uGUI;

class Touch {
public:
    Touch(lcd_interface & lcd)
            : _lcd(lcd), _XN(2), _YN(3), _XP(27), _YP(26) {
    }

    bool pressed() {
        uint16_t x, y;
        this->getXY_raw(x,y);
        return x>200 && y>200;
    }

    void getXY_raw(uint16_t & x, uint16_t & y) {
        _XN.gpioMode(GPIO::OUTPUT | GPIO::INIT_LOW);
        _XP.gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);
        _YN.gpioMode(GPIO::INPUT);
        _YP.gpioMode(GPIO::INPUT | GPIO::PULLDOWN);
        task::sleep(10);
        x = adc_rp2040::inst.adcReadRaw(0);
        _YN.gpioMode(GPIO::OUTPUT | GPIO::INIT_LOW);
        _YP.gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);
        _XN.gpioMode(GPIO::INPUT);
        _XP.gpioMode(GPIO::INPUT | GPIO::PULLDOWN);
        task::sleep(10);
        y = adc_rp2040::inst.adcReadRaw(2);
    }

    bool transform(uint16_t & x, uint16_t & y) {
        x = ((int)x - (int)_xs) * ((int)_lcd.getSizeX()-10) / ((int)_xe - (int)_xs) + 5;
        y = ((int)y - (int)_ys) * ((int)_lcd.getSizeY()-10) / ((int)_ye - (int)_ys) + 5;
        if (x > 319) return false;
        if (y > 239) return false;
        return true;
    }

    void calibrate() {
        uint16_t x,y;
        // Clear screen
        _lcd.fillArea(0, 0, _lcd.getSizeX()-1, _lcd.getSizeY()-1, C_BLACK);

        //
        _lcd.fillArea(0, 0, 19, 19, C_RED);
        while(!pressed()) ;
        task::sleep(10);
        getXY_raw(x, y);
        while(pressed());
        _xs = x;
        _ys = y;

        //
        _lcd.fillArea(_lcd.getSizeX()-20, 0,
                      _lcd.getSizeX()- 1, 19, C_RED);
        while(!pressed()) ;
        task::sleep(10);
        getXY_raw(x, y);
        while( pressed()) ;
        _xe = x;
        _ys = (_ys + y) / 2;

        //
        _lcd.fillArea( 0, _lcd.getSizeY()-20,
                       19, _lcd.getSizeY()- 1, C_RED);
        while(!pressed()) ;
        task::sleep(10);
        getXY_raw(x, y);
        while( pressed()) ;
        _xs = (_xs + x) / 2;
        _ye = y;

        //
        _lcd.fillArea(_lcd.getSizeX()-20, _lcd.getSizeY()-20,
                      _lcd.getSizeX()- 1, _lcd.getSizeY()- 1, C_RED);
        while(!pressed()) ;
        task::sleep(10);
        getXY_raw(x, y);
        while( pressed()) ;
        _xe = (_xe + x) / 2;
        _ye = (_ye + y) / 2;
    }

private:
    // The LCD we sit on
    lcd_interface   &   _lcd;
    // touch enable gpios
    gpio_rp2040_pin     _XN;
    gpio_rp2040_pin     _YN;
    gpio_rp2040_pin     _XP;
    gpio_rp2040_pin     _YP;

    uint16_t            _xs, _xe, _ys, _ye;
    bool _pressed;
};

class TouchUIBox {
private:
    int16_t pos_x, pos_y, width, height;
    UG_COLOR fg, bg;
public:

    TouchUIBox(int16_t posX, int16_t posY, int16_t width, int16_t height, UG_COLOR fg, UG_COLOR bg) : pos_x(posX), pos_y(posY), width(width), height(height), fg(fg), bg(bg) {}

    bool touchInside(int16_t x, int16_t y) const {
        return x > pos_x && x < pos_x + width && y > pos_y && y < pos_y + height;
    }

    void draw(uGUI& gui) {
        gui.DrawFrame(pos_x, pos_y, pos_x + width, pos_y + height, fg);
        drawIcon(gui, fg);
    }

    void glowUp(uGUI& gui) {
        gui.FillFrame(pos_x, pos_y, pos_x + width, pos_y + height, fg);
        drawIcon(gui, bg);
    }

    void glowDown(uGUI& gui) {
        gui.FillFrame(pos_x, pos_y, pos_x + width, pos_y + height, bg);
        gui.DrawFrame(pos_x, pos_y, pos_x + width, pos_y + height, fg);
        drawIcon(gui, fg);
    }

    virtual void drawIcon(uGUI& gui, UG_COLOR c) {};

     virtual bool reactToTouch(uGUI& gui, int16_t x, int16_t y) {
        if(touchInside(x, y)) {
            glowUp(gui);
            return true;
        }
         return false;
    }

    [[nodiscard]] int16_t getPosX() const {
        return pos_x;
    }

    [[nodiscard]] int16_t getPosY() const {
        return pos_y;
    }

    [[nodiscard]] int16_t getWidth() const {
        return width;
    }

    [[nodiscard]] int16_t getHeight() const {
        return height;
    }

    [[nodiscard]] UG_COLOR getFg() const {
        return fg;
    }

    [[nodiscard]] UG_COLOR getBg() const {
        return bg;
    }
};

class MatrixBox: public TouchUIBox {
private:
    bool active = false;

public:
    MatrixBox(int16_t posX, int16_t posY, int16_t width, int16_t height, UG_COLOR fg, UG_COLOR bg) : TouchUIBox(posX, posY, width, height, fg, bg) {}

    bool reactToTouch(uGUI& gui, int16_t x, int16_t y) override {
        if(touchInside(x, y)) {
            active = !active;
            glowUp(gui);
            return true;
        }
        return false;
    }

    void drawIcon(uGUI& gui, UG_COLOR c) override {
        if(active) {
            gui.DrawLine(getPosX(), getPosY(), getPosX() + getWidth(), getPosY() + getHeight(), c);
            gui.DrawLine(getPosX(), getPosY() + getHeight(), getPosX() + getWidth(), getPosY(), c);
        }
    }
};

class PlayPauseButton: public TouchUIBox {
private:
    bool playing = false;
    float relativ_padding = 0.25;
    uint8_t padding_h, padding_v;
public:
    PlayPauseButton(int16_t posX, int16_t posY, int16_t width, int16_t height, UG_COLOR fg, UG_COLOR bg) : TouchUIBox(posX, posY, width, height, fg, bg) {
        padding_h = relativ_padding * width;
        padding_v = relativ_padding * height;
    }

    bool reactToTouch(uGUI& gui, int16_t x, int16_t y) override {
        if(touchInside(x, y)) {
            playing = !playing;
            glowUp(gui);
            return true;
        }
        return false;
    }

    void drawIcon(uGUI &gui, UG_COLOR c) override {
        if(playing) {
            gui.DrawFrame(getPosX() + padding_h, getPosY() + padding_v, getPosX() + getWidth() - padding_h, getPosY() + getHeight() -padding_v, c);
        } else {
            gui.DrawLine(getPosX() + padding_h, getPosY() + padding_v, getPosX() + getWidth() - padding_h, getPosY() + 0.5 * getHeight(), c);
            gui.DrawLine(getPosX() + padding_h, getPosY() + getHeight() -padding_v, getPosX() + getWidth() - padding_h, getPosY() + 0.5 * getHeight(), c);
            gui.DrawLine(getPosX() + padding_h, getPosY() + padding_v, getPosX() + padding_h, getPosY() + getHeight() -padding_v, c);
        }
    }
};

int main(void)
{
    uart_rp2040 uart; // default is backchannel UART!

    posix_io::inst.register_stdin ( uart );
    posix_io::inst.register_stdout( uart );
    posix_io::inst.register_stderr( uart );

    // Switch on background LCD
    gpio_rp2040_pin lcd_led(14);
    lcd_led.gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);

    // Setup SPI interface in SPI mode 3.
    gpio_rp2040_pin lcd_cs (5);
    uint16_t spi_mode = SPI::CPOL_1 | SPI::CPHA_1 | SPI::_8_BIT;
    spi_rp2040  spi(0, 4, 7, 22, lcd_cs, SPI::MASTER, spi_mode );
    spi.setSpeed(24000000);

    // Setup LCD driver
    gpio_rp2040_pin lcd_rst(17);
    gpio_rp2040_pin lcd_dc (23);
    ssd2119_drv lcd(spi, lcd_rst, lcd_dc, ssd2119_drv::Kentec_K350QVG);

    // Clear screen
    lcd.clearScreen(C_BLACK);

    // Setup uGUI
    uGUI gui(lcd);

    // Touch interface
    Touch touch(lcd);
    gpio_rp2040::inst.gpioMode(18, GPIO::OUTPUT | GPIO::INIT_LOW);

    touch.calibrate();
    lcd.clearScreen(C_BLACK);
//    uint16_t x, y;
//    while (1) {
//        touch.getXY_raw(x,y);
//        printf("%d %d %d\n", x, y, touch.pressed());
//        //task::sleep(100);
//    }

    uint16_t x=0, y=0;
    MatrixBox box = {0, 0, 20, 35, C_WHITE, C_BLACK};

    uint8_t N_HORIZONTAL = 16, N_VERTICAL = 4;
    int16_t paddingLeft = 8, paddingTop = 6;
    int16_t box_width = 19, box_height = 30;

    MatrixBox** boxes = new MatrixBox*[N_HORIZONTAL * N_VERTICAL];

    for(uint8_t y = 0; y < N_VERTICAL; y++) {
        for(uint8_t x = 0; x < N_HORIZONTAL; x++) {
            boxes[y * N_HORIZONTAL + x] = new MatrixBox(paddingLeft + x * box_width, paddingTop + y * box_height, box_width, box_height, C_WHITE, C_BLACK);
        }
    }

    for(uint8_t i = 0; i < N_HORIZONTAL * N_VERTICAL; i ++) {
        boxes[i]->draw(gui);
    }

    auto* playPause = new PlayPauseButton(paddingLeft, 160, 60, 40, C_WHITE, C_BLACK);
    playPause->draw(gui);
    while(1) {
        bool p = touch.pressed();
        if (p) {
            touch.getXY_raw(x, y);
            if (touch.transform(x, y)) {
                int16_t targetIndex = -1;
                for(uint8_t i = 0; i < N_HORIZONTAL * N_VERTICAL; i ++) {
                    if(boxes[i]->reactToTouch(gui, x, y)){
                        targetIndex = i;
                        // Todo Code to calculate index and manipulate Model
                        break;
                    }
                }
                if(playPause->reactToTouch(gui, x, y)) {
                    // Todo Code to stop Model
                    targetIndex = -100;
                }

                while(touch.pressed());
                if(targetIndex >= 0) {
                    boxes[targetIndex]->glowDown(gui);
                } else if(targetIndex == -100) {
                    playPause->glowDown(gui);
                }
            }

//            gui.DrawLine(xl, 0,   xl, 239, C_BLACK);
//            gui.DrawLine(0,  yl, 319,  yl, C_BLACK);
//            xl = x;
//            yl = y;
//            gui.DrawLine(xl, 0,   xl, 239, C_WHITE);
//            gui.DrawLine(0,  yl, 319,  yl, C_WHITE);


//            for (int i=0; i < 100000; ++i) ;
        }

//        String xs = to_String(x) + "    ";
//        String ys = to_String(y) + "    ";
//        gui.PutString(200,  9, xs.c_str());
//        gui.PutString(200, 29, ys.c_str());
//
//        for (int i=0; i < 10000; ++i) ;
    }


//    for(int i=0; i < 100000; ++i) {
//        uint32_t c = rand() % 0x1000000;
//        uint16_t x1 = rand() % lcd.getSizeX();
//        uint16_t y1 = rand() % lcd.getSizeY();
//        uint16_t x2 = rand() % lcd.getSizeX();
//        uint16_t y2 = rand() % lcd.getSizeY();
//        gui.FillFrame(x1, y1, x2, y2, c);
//    }

}