//
// Created by tobia on 01.02.2024.
//

#ifndef GAME_TOUCH_H
#define GAME_TOUCH_H

#include <gpio_rp2040.h>
#include "ssd2119_drv.h"
#include "task.h"
#include <adc_rp2040.h>
#include "uGUI.h"
#include "uGUI_colors.h"
#include "yahal_String.h"
#include "font_10x16.h"
#include "font_7x12.h"

#include <cstdio>

class Touch {
public:
    Touch(lcd_interface &lcd, uGUI &uGUI);
    bool pressed();
    void calibrate();
    void waitForPress(uint16_t &x, uint16_t &y);

private:
    void getRawXY(uint16_t &x, uint16_t &y);
    void transformLinear(uint16_t &x, uint16_t &y) const;
    void testCalibration(uint16_t allowedClicks=1);

    // Objects for communcating with display and drawing objects
    lcd_interface           &lcd;
    uGUI                    &gui;

    // Pins to read touch-input
    gpio_rp2040_pin         XN;
    gpio_rp2040_pin         YN;
    gpio_rp2040_pin         XP;
    gpio_rp2040_pin         YP;

    // factors for the transformation - calculated after calibration
    uint16_t                ax, ay, bx, by, cx, cy, dx, dy, ex, ey;
    float                   sx, sy, tx, ty;
    bool                    isCalibrated;
};


#endif //GAME_TOUCH_H
