//
// Created by Florian Bremm on 25.02.24.
//

#ifndef MCT_UI_H
#define MCT_UI_H

#include "gpio_rp2040.h"
#include "spi_rp2040.h"
#include "ssd2119_drv.h"
#include "uGUI.h"
#include "uGUI_colors.h"
#include "adc_rp2040.h"
#include "yahal_String.h"
#include "task.h"
#include <cstdlib>
#include "font_6x10.h"
#include "font_4x6.h"
#include "font_5x8.h"
#include <cstdio>
#include <font_10x16.h>
#include "uart_rp2040.h"
#include "posix_io.h"


class Touch {
public:
    Touch(lcd_interface &lcd)
            : _lcd(lcd), _XN(2), _YN(3), _XP(27), _YP(26) {
    }

    bool pressed() {
        uint16_t x, y;
        this->getXY_raw(x, y);
        return x > 200 && y > 200;
    }

    void getXY_raw(uint16_t &x, uint16_t &y) {
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

    bool transform(uint16_t &x, uint16_t &y) {
        x = ((int) x - (int) _xs) * ((int) _lcd.getSizeX() - 10) / ((int) _xe - (int) _xs) + 5;
        y = ((int) y - (int) _ys) * ((int) _lcd.getSizeY() - 10) / ((int) _ye - (int) _ys) + 5;
        if (x > 319) return false;
        if (y > 239) return false;
        return true;
    }

    void calibrate() {
        uint16_t x, y;
        // Clear screen
        _lcd.fillArea(0, 0, _lcd.getSizeX() - 1, _lcd.getSizeY() - 1, C_BLACK);

        //
        _lcd.fillArea(0, 0, 19, 19, C_RED);
        while (!pressed());
        task::sleep(10);
        getXY_raw(x, y);
        while (pressed());
        _xs = x;
        _ys = y;

        //
        _lcd.fillArea(_lcd.getSizeX() - 20, 0,
                      _lcd.getSizeX() - 1, 19, C_RED);
        while (!pressed());
        task::sleep(10);
        getXY_raw(x, y);
        while (pressed());
        _xe = x;
        _ys = (_ys + y) / 2;

        //
        _lcd.fillArea(0, _lcd.getSizeY() - 20,
                      19, _lcd.getSizeY() - 1, C_RED);
        while (!pressed());
        task::sleep(10);
        getXY_raw(x, y);
        while (pressed());
        _xs = (_xs + x) / 2;
        _ye = y;

        //
        _lcd.fillArea(_lcd.getSizeX() - 20, _lcd.getSizeY() - 20,
                      _lcd.getSizeX() - 1, _lcd.getSizeY() - 1, C_RED);
        while (!pressed());
        task::sleep(10);
        getXY_raw(x, y);
        while (pressed());
        _xe = (_xe + x) / 2;
        _ye = (_ye + y) / 2;
    }

private:
    // The LCD we sit on
    lcd_interface &_lcd;
    // touch enable gpios
    gpio_rp2040_pin _XN;
    gpio_rp2040_pin _YN;
    gpio_rp2040_pin _XP;
    gpio_rp2040_pin _YP;

    uint16_t _xs, _xe, _ys, _ye;
    bool _pressed;
};


/**
 * Helper class containing basic functionality for touchable UI elements
 */
class TouchUIBox {
private:
    int16_t pos_x, pos_y, width, height;
    UG_COLOR fg, bg;
public:

    TouchUIBox(int16_t posX, int16_t posY, int16_t width, int16_t height, UG_COLOR fg, UG_COLOR bg) : pos_x(posX),
                                                                                                      pos_y(posY),
                                                                                                      width(width),
                                                                                                      height(height),
                                                                                                      fg(fg), bg(bg) {}

    /**
     *
     * @param x
     * @param y
     * @return true if Box is touch target
     */
    bool touchInside(int16_t x, int16_t y) const {
        return x > pos_x && x < pos_x + width && y > pos_y && y < pos_y + height;
    }

    void draw(uGUI &gui) {
        gui.DrawFrame(pos_x, pos_y, pos_x + width, pos_y + height, fg);
        drawIcon(gui, fg);
    }

    /**
     * glow up / down to visually react to user input
     * @param gui
     */
    void glowUp(uGUI &gui) {
        gui.FillFrame(pos_x, pos_y, pos_x + width, pos_y + height, fg);
        drawIcon(gui, bg);
    }

    void glowDown(uGUI &gui) {
        gui.FillFrame(pos_x, pos_y, pos_x + width, pos_y + height, bg);
        gui.DrawFrame(pos_x, pos_y, pos_x + width, pos_y + height, fg);
        drawIcon(gui, fg);
    }

    /**
     * Override this method to add a icon to the box
     * @param gui
     * @param c
     */
    virtual void drawIcon(uGUI &gui, UG_COLOR c) {};

    /**
     * Methode to react to touch input
     * @param gui
     * @param x
     * @param y
     * @return true if box is touch target
     */
    virtual bool reactToTouch(uGUI &gui, int16_t x, int16_t y) {
        if (touchInside(x, y)) {
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

/**
 * Class for Boxes in Sound-Matrix
 */
class MatrixBox : public TouchUIBox {
private:
    bool active = false;

public:
    MatrixBox(int16_t posX, int16_t posY, int16_t width, int16_t height, UG_COLOR fg, UG_COLOR bg) : TouchUIBox(posX,
                                                                                                                posY,
                                                                                                                width,
                                                                                                                height,
                                                                                                                fg,
                                                                                                                bg) {}

    bool reactToTouch(uGUI &gui, int16_t x, int16_t y) override {
        if (touchInside(x, y)) {
            active = !active;
            glowUp(gui);
            return true;
        }
        return false;
    }

    /**
     * draws a X if box is active
     * @param gui
     * @param c
     */
    void drawIcon(uGUI &gui, UG_COLOR c) override {
        if (active) {
            gui.DrawLine(getPosX(), getPosY(), getPosX() + getWidth(), getPosY() + getHeight(), c);
            gui.DrawLine(getPosX(), getPosY() + getHeight(), getPosX() + getWidth(), getPosY(), c);
        }
    }
};

class PlayPauseButton : public TouchUIBox {
private:
    bool playing = false;
    float relativ_padding = 0.25;
    uint8_t padding_h, padding_v;
public:
    PlayPauseButton(int16_t posX, int16_t posY, int16_t width, int16_t height, UG_COLOR fg, UG_COLOR bg) : TouchUIBox(
            posX, posY, width, height, fg, bg) {
        padding_h = relativ_padding * width;
        padding_v = relativ_padding * height;
    }

    bool reactToTouch(uGUI &gui, int16_t x, int16_t y) override {
        if (touchInside(x, y)) {
            playing = !playing;
            glowUp(gui);
            return true;
        }
        return false;
    }

    /**
     * Draws a Triangle or a Square depending on whether the current state is editing or playing
     * @param gui
     * @param c
     */
    void drawIcon(uGUI &gui, UG_COLOR c) override {
        if (playing) {
            gui.DrawFrame(getPosX() + padding_h, getPosY() + padding_v, getPosX() + getWidth() - padding_h,
                          getPosY() + getHeight() - padding_v, c);
        } else {
            gui.DrawLine(getPosX() + padding_h, getPosY() + padding_v, getPosX() + getWidth() - padding_h,
                         getPosY() + 0.5 * getHeight(), c);
            gui.DrawLine(getPosX() + padding_h, getPosY() + getHeight() - padding_v, getPosX() + getWidth() - padding_h,
                         getPosY() + 0.5 * getHeight(), c);
            gui.DrawLine(getPosX() + padding_h, getPosY() + padding_v, getPosX() + padding_h,
                         getPosY() + getHeight() - padding_v, c);
        }
    }
};

/**
 * Boxes for Instrument selection
 */
class SelectBox : public TouchUIBox {
private:
    const char *_title;
    uGUI::FONT _font;
public:
    SelectBox(int16_t posX, int16_t posY, int16_t width, int16_t height, UG_COLOR fg, UG_COLOR bg, uGUI::FONT font,
              const char *title = "") :
            TouchUIBox(posX, posY, width, height, fg, bg),
            _title(title),
            _font(font) {}

    void setTitle(const char *title) {
        this->_title = title;
    }

    /**
     * Icon is the title of the sample
     * @param gui
     * @param c
     */
    void drawIcon(uGUI &gui, UG_COLOR c) override {
        gui.FontSelect(&this->_font);
        int16_t x = getPosX() + getWidth() / 7, y = getPosY() + (getHeight() - this->_font.char_height) / 2;
        gui.PutString(x, y, this->_title);
    }

    ~SelectBox() = default;
};


class ConfirmButton : public TouchUIBox {
private:
    float relativ_padding = 0.25;
    uint8_t padding_h, padding_v;
public:
    ConfirmButton(int16_t posX, int16_t posY, int16_t width, int16_t height, UG_COLOR fg, UG_COLOR bg) : TouchUIBox(
            posX, posY, width, height, fg, bg) {
        padding_h = relativ_padding * width;
        padding_v = relativ_padding * height;
    }

    bool reactToTouch(uGUI &gui, int16_t x, int16_t y) override {
        if (touchInside(x, y)) {
            glowUp(gui);
            return true;
        }
        return false;
    }

    /**
     * Icon is a checkmark
     * @param gui
     * @param c
     */
    void drawIcon(uGUI &gui, UG_COLOR c) override {
        gui.DrawLine(getPosX() + padding_h, getPosY() + padding_v + (getHeight() - 2 * padding_v) * 2 / 3,
                     getPosX() + getWidth() / 3, getPosY() + getHeight() - padding_v, c);
        gui.DrawLine(getPosX() + getWidth() / 3, getPosY() + getHeight() - padding_v,
                     getPosX() + getWidth() - padding_h, getPosY() + padding_v, c);
    }

    ~ ConfirmButton() = default;
};


class TextButton : public TouchUIBox {
private:
    float relativ_padding = 0.25;
    uint8_t padding_h, padding_v;
    uGUI::FONT _font;
    char *_text;
public:
    TextButton(int16_t posX, int16_t posY, int16_t width, int16_t height, UG_COLOR fg, UG_COLOR bg, uGUI::FONT font,
               char *text) :
            TouchUIBox(posX, posY, width, height, fg, bg), _font(font), _text(text) {
        padding_h = relativ_padding * width;
        padding_v = relativ_padding * height;
    }

    bool reactToTouch(uGUI &gui, int16_t x, int16_t y) override {
        if (touchInside(x, y)) {
            glowUp(gui);
            return true;
        }
        return false;
    }

    void drawIcon(uGUI &gui, UG_COLOR c) override {
        int16_t x = getPosX() + getWidth() / 7, y = getPosY() + (getHeight() - this->_font.char_height) / 2;
        gui.FontSelect(&_font);
        gui.PutString(x, y, _text);
    }

    ~TextButton() = default;
};

/*
 * Helper class to have less code for LCD setup in Main
 */
class LCD_Setup {
public:
    gpio_rp2040_pin _lcd_led, _lcd_cs, _lcd_rst, _lcd_dc;
    spi_rp2040 _spi;
    uint16_t spi_mode = SPI::CPOL_1 | SPI::CPHA_1 | SPI::_8_BIT;

    LCD_Setup() :
            _lcd_led(14),
            _lcd_cs(5),
            _lcd_rst(17),
            _lcd_dc(23),
            _spi(0, 4, 7, 22, _lcd_cs, SPI::MASTER, spi_mode) {
        _lcd_led.gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);

        // Setup SPI interface in SPI mode 3.

        _spi.setSpeed(24000000);

        // Setup LCD driver

    }
};

/**
 * UI main controller
 */
class UI {
private:
    const uint8_t N_HORIZONTAL = 16, N_VERTICAL = 4;
    const int16_t paddingLeft = 8, paddingTop = 6;
    const int16_t box_width = 19, box_height = 30;

    bool playing = false;
    bool started = false;

    ssd2119_drv _lcd;
    uGUI _gui;
    Touch _touch;
    MatrixBox **_boxes;
    PlayPauseButton *_playPauseButton;
    TextButton *_editButton;
    Model *_model;


public:
    UI(Model *model, const ssd2119_drv &lcd, Touch &touch) : _lcd{lcd}, _gui{_lcd}, _touch{touch}, _model{model} {
    }

    /**
     * Sets up touchscreen and calibrates it
     * @param lcd
     * @return
     */
    static Touch setUpTouch(ssd2119_drv& lcd) {
        Touch touch(lcd);
        gpio_rp2040::inst.gpioMode(18, GPIO::OUTPUT | GPIO::INIT_LOW);
        lcd.clearScreen(C_BLACK);
        touch.calibrate();
        lcd.clearScreen(C_BLACK);
        return touch;
    }

    /**
     * Draws the UI
     */
    void buildUI() {
        _lcd.clearScreen(C_BLACK);
        _boxes = new MatrixBox *[N_HORIZONTAL * N_VERTICAL];

        for (uint8_t y = 0; y < N_VERTICAL; y++) {
            for (uint8_t x = 0; x < N_HORIZONTAL; x++) {
                _boxes[y * N_HORIZONTAL + x] = new MatrixBox(paddingLeft + x * box_width, paddingTop + y * box_height,
                                                             box_width, box_height, C_WHITE, C_BLACK);
                if (_model->at(x, y)) {
                    _boxes[y * N_HORIZONTAL + x]->reactToTouch(_gui, paddingLeft + x * box_width + 1,
                                                               paddingTop + y * box_height + 1);
                    _boxes[y * N_HORIZONTAL + x]->glowDown(_gui);
                }
            }
        }

        for (uint8_t i = 0; i < N_HORIZONTAL * N_VERTICAL; i++) {
            _boxes[i]->draw(_gui);
        }

        _playPauseButton = new PlayPauseButton(paddingLeft, 190, 60, 40, C_WHITE, C_BLACK);
        _playPauseButton->draw(_gui);
        _editButton = new TextButton(240, 190, 70, 40, C_WHITE, C_BLACK, FONT_10X16, "EDIT");
        _editButton->draw(_gui);
    }

    /**
     * rendering UI containing Matrix and Playbutton
     * @param Player Task which contains the Model.play() method
     * @return if the Edit button was clicked
     */
    bool runUI(task *Player) {
        uint16_t x, y;
        int16_t targetIndex;
        uint8_t i;
        if (_touch.pressed()) {
            _touch.getXY_raw(x, y);
            if (_touch.transform(x, y)) {
                targetIndex = -1;
                for (i = 0; i < N_HORIZONTAL * N_VERTICAL; i++) {
                    //handle matrix click
                    if (!playing && _boxes[i]->reactToTouch(_gui, x, y)) {
                        targetIndex = i;
                        int row = i / N_HORIZONTAL;
                        int column = i % N_HORIZONTAL;
                        _model->at(column, row) = !_model->at(column, row);
                        break;
                    }
                }
                if (_playPauseButton->reactToTouch(_gui, x, y)) {
                    //check if Music is already playing. if not start or resume playing task
                    if (!playing) {
                        if (started) {
                            Player->resume();
                        } else {
                            Player->start();
                            started = true;
                        }

                    } else {
                        //triggered when pause button is pressed
                        Player->suspend();
                    }
                    playing = !playing;
                    targetIndex = -100;
                }

                if (!playing && _editButton->reactToTouch(_gui, x, y)) {
                    selectInstrumentUI(_model);
                    return true;
                }

                while (_touch.pressed());
                //box shouöd glowDown once not pressed anymore
                if (targetIndex >= 0) {
                    _boxes[targetIndex]->glowDown(_gui);
                } else if (targetIndex == -100) {
                    _playPauseButton->glowDown(_gui);
                }
            }
        }
        //draw current position of model during play
        if (playing) {
            const uint8_t currentColumn = _model->getCurrentColumn();
            const uint8_t lastColumnIndex = N_HORIZONTAL - 1;
            for (i = 0; i < N_HORIZONTAL * N_VERTICAL; i++) {
                uint8_t currentElementColumn = i % N_HORIZONTAL;
                //if currentElem is displayed let it glow up
                if (currentElementColumn == currentColumn) {
                    _boxes[i]->glowUp(_gui);
                } else if (currentElementColumn == currentColumn - 1 ||
                           (currentColumn == 0 && currentElementColumn == lastColumnIndex)) {
                    _boxes[i]->glowDown(_gui);
                }

            }
        }
        return false;
    }

    /**
     * Methode for instrument selection / change. Includes the touch handling in the selection area.
     * @param model
     */
    void selectInstrumentUI(Model *model) {
        const uint8_t paddingCenter = 50, NumSamples = 16, SBHeight = 40, SBWidth = 100, ABHeight = 25, ABWidth = 70;

        _lcd.clearScreen(C_BLACK);

        int8_t selectedIndex = -1;

        auto **selectedSounds = new SelectBox *[N_VERTICAL];
        auto **availableSounds = new SelectBox *[NumSamples];
        ConfirmButton confirmButton(paddingLeft, 190, 60, 40, C_WHITE, C_BLACK);
        confirmButton.draw(_gui);

        Sample *selected = model->getSamples();
        for (uint8_t i = 0; i < N_VERTICAL; i++) {
            char const *text = selected[i].getName();
            selectedSounds[i] = new SelectBox(paddingLeft, paddingTop + i * SBHeight, SBWidth, SBHeight, C_WHITE,
                                              C_BLACK, FONT_5X8, text);
            selectedSounds[i]->draw(_gui);
        }

        for (uint8_t i = 0; i < NumSamples; i++) {
            const char *text = LABELS[i];
            if (i % 2 == 0) {
                availableSounds[i] = new SelectBox(paddingLeft + SBWidth + paddingCenter,
                                                   paddingTop + (i / 2) * ABHeight, ABWidth, ABHeight, C_WHITE, C_BLACK,
                                                   FONT_4X6, text);
                availableSounds[i]->draw(_gui);
            } else {
                availableSounds[i] = new SelectBox(paddingLeft + SBWidth + paddingCenter + ABWidth,
                                                   paddingTop + (i / 2) * ABHeight, ABWidth, ABHeight, C_WHITE, C_BLACK,
                                                   FONT_4X6, text);
                availableSounds[i]->draw(_gui);
            }

        }

        while (true) {
            uint16_t x, y;
            int16_t targetIndex = -1;
            uint8_t i;
            if (_touch.pressed()) {
                _touch.getXY_raw(x, y);
                if (_touch.transform(x, y)) {
                    if (confirmButton.reactToTouch(_gui, x, y)) {
                        delete[] availableSounds;
                        delete[] selectedSounds;
                        return;
                    }
                    for (uint8_t i = 0; i < N_VERTICAL; i++) {
                        if (selectedSounds[i]->reactToTouch(_gui, x, y)) {
                            if (selectedIndex != -1) {
                                selectedSounds[selectedIndex]->glowDown(_gui);
                            }
                            selectedIndex = i;
                        }
                    }
                    if (selectedIndex != -1) {
                        for (uint8_t i = 0; i < NumSamples; i++) {
                            if (availableSounds[i]->reactToTouch(_gui, x, y)) {
                                targetIndex = i;
                                model->getSamples()[selectedIndex] = Sample(static_cast<Song>(i));
                                selectedSounds[selectedIndex]->setTitle(LABELS[i]);
                                selectedSounds[selectedIndex]->glowDown(_gui);
                                selectedIndex = -1;
                            }
                        }
                    }
                    while (_touch.pressed());
                    if (targetIndex != -1) {
                        availableSounds[targetIndex]->glowDown(_gui);
                    }
                }
            }
        }
    }
};


#endif //MCT_UI_H
