//
// Created by janni on 28.02.2024.
//

#ifndef MCT_PROJECT_GS_MENU_H
#define MCT_PROJECT_GS_MENU_H

#ifndef BUTTON_DELAY
#define BUTTON_DELAY 200
#endif

#include <uGUI.h>
#include <adc_rp2040.h>
#include <gpio_rp2040.h>
#include <string>
#include "yahal_String.h"

#include "font_4x6.h"
#include "font_5x8.h"
#include "font_5x12.h"
#include "font_6x8.h"
#include "font_6x10.h"
#include "font_7x12.h"
#include "font_8x12.h"

#include "Color.h"

class Menu {
private:
    uGUI gui;
    adc_rp2040_channel joy;
    uint16_t offsetY;
    gpio_rp2040_pin s1;
    gpio_rp2040_pin s2;
    UG_COLOR menuFocusColor = Color(20, 200, 20);
    UG_COLOR menuBackgroundColor = Color(255, 255, 255);
    UG_COLOR menuFontColor = Color(0, 0, 0);
    uint8_t loadMenuState = 0;
    const std::vector<std::string> loadMenuStates = {".", "..", "...", "..", ".", ""};

    void drawChoices(std::vector<std::string> &choices, uint8_t focusIndex, uint8_t focusIndexOld);
public:
    Menu(uGUI gui, adc_rp2040_channel joy, uint16_t offsetY, gpio_rp2040_pin s1, gpio_rp2040_pin s2);
    int8_t GetMenuChoice(String &menuName, std::vector<std::string> &choices, bool enableS1=true);
    void RefreshLoadMenu(String &menuName, bool initializing=false);
};


#endif //MCT_PROJECT_GS_MENU_H
