//
// Created by janni on 28.02.2024.
//

#include <task.h>
#include <string>
#include "Menu.h"

Menu::Menu(uGUI gui, adc_rp2040_channel joy, uint16_t offsetY, gpio_rp2040_pin s1, gpio_rp2040_pin s2):
    gui(gui), joy(joy), offsetY(offsetY), s1(s1), s2(s2) { }

void Menu::drawChoices(std::vector<std::string>& choices, uint8_t focusIndex, uint8_t focusIndexOld) {
    gui.FontSelect(&FONT_6X10);
    gui.SetForecolor(menuFontColor);
    gui.SetBackcolor(menuBackgroundColor);

    uint8_t menuHeight = 108;
    uint8_t choiceCountClamped = choices.size()>10 ? 10 : choices.size();
    uint8_t menuOffsetY = 20 + (menuHeight - choiceCountClamped * 10) / 2;
    for (uint8_t i = 0; i < choiceCountClamped; i++) {
        uint8_t positionY = menuOffsetY + 10 * i;
        if (focusIndex == i) {
            gui.SetBackcolor(menuFocusColor);
            for (uint8_t x=0;x<128;x++){
                for (uint8_t y=positionY;y<positionY+10;y++){
                    gui.DrawPixel(x, y, menuFocusColor);
                }
            }
        } else {
            gui.SetBackcolor(menuBackgroundColor);
            if (focusIndexOld == i) {
                for (uint8_t x=0;x<128;x++){
                    for (uint8_t y=positionY;y<positionY+10;y++){
                        gui.DrawPixel(x, y, menuBackgroundColor);
                    }
                }
            }
        }
        std::string displayString = choices[i];
        for (uint8_t j = 0; j < 16-displayString.length(); j++) {
            displayString += " ";
        }
        gui.PutString(10, positionY, displayString.c_str());
    }
}


void Menu::RefreshLoadMenu(String& menuName, bool initializing) {
    std::vector<std::string> displayState = {loadMenuStates[loadMenuState]};
    if (initializing) {
        loadMenuState = 0;
        displayState[0] = {loadMenuStates[loadMenuState]};
        for (uint8_t x=0;x<128;x++){
            for (uint8_t y=0;y<20;y++){
                gui.DrawPixel(x, y, menuFontColor);
            }
        }
        for (uint8_t x=0;x<128;x++){
            for (uint8_t y=20;y<128;y++){
                gui.DrawPixel(x, y, menuBackgroundColor);
            }
        }
        gui.FontSelect(&FONT_8X12);
        gui.SetForecolor(menuBackgroundColor);
        gui.SetBackcolor(menuFontColor);
        gui.PutString(10, 3, menuName);
    }
    drawChoices(displayState, -1, -1);
    loadMenuState = (loadMenuState + 1) % loadMenuStates.size();
}


int8_t Menu::GetMenuChoice(String& menuName, std::vector<std::string>& choices, bool enableS1) {
    for (uint8_t x=0;x<128;x++){
        for (uint8_t y=0;y<20;y++){
            gui.DrawPixel(x, y, menuFontColor);
        }
    }
    for (uint8_t x=0;x<128;x++){
        for (uint8_t y=20;y<128;y++){
            gui.DrawPixel(x, y, menuBackgroundColor);
        }
    }
    gui.FontSelect(&FONT_8X12);
    gui.SetForecolor(menuBackgroundColor);
    gui.SetBackcolor(menuFontColor);
    gui.PutString(10, 3, menuName);

    int8_t cursorY = 0;
    int8_t cursorYOld = 0;
    uint8_t choiceCountClamped = choices.size()>10 ? 10 : choices.size();
    drawChoices(choices, cursorY, 0);

    while (true) {

        if (cursorY != cursorYOld) {
            drawChoices(choices, cursorY, cursorYOld);
            cursorYOld = cursorY;
        }

        // read in a new ADC value
        uint16_t valY = joy.adcReadRaw();


        if ((valY - offsetY) < -1000 && cursorY < choiceCountClamped - 1) {
            cursorY++;
            task::sleep(20);
            //focusSquare(gui, cursorX, cursorY);
            uint8_t buttonPassedTime = 0;
            while ((joy.adcReadRaw() - offsetY) < -1000 && buttonPassedTime < BUTTON_DELAY) {
                task::sleep(1);
                buttonPassedTime++;
            }
            task::sleep(10);
        }
        if ((valY - offsetY) > 1000 && cursorY > 0) {
            cursorY--;
            task::sleep(20);
            //focusSquare(gui, cursorX, cursorY);
            uint8_t buttonPassedTime = 0;
            while ((joy.adcReadRaw() - offsetY) > 1000 && buttonPassedTime < BUTTON_DELAY) {
                task::sleep(1);
                buttonPassedTime++;
            }
            task::sleep(10);
        }

        if (enableS1 && !s1.gpioRead()) {
            task::sleep(20);
            while (!s1.gpioRead());
            task::sleep(10);
            return -1;
        }
        if (!s2.gpioRead()) {
            task::sleep(20);
            while (!s2.gpioRead());
            task::sleep(10);
            return cursorY;
        }
    }
}
