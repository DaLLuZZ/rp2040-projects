//
// Created by tobia on 19.01.2024.
//

#ifndef GAME_GAMEGUI_H
#define GAME_GAMEGUI_H

#include "gpio_rp2040.h"
#include "spi_rp2040.h"
#include "ssd2119_drv.h"

#include "uGUI.h"
#include <cstdio>
#include "posix_io.h"

// Custom Imports
#include "../Enums.h"
#include "gui/menustate/Menu.h"
#include "gui/gamestate/GridUI.h"
#include "gui/technical/Touch.h"
#include "logic/Map.h"
class GameGUI {

public:
    GameGUI();
    void draw(GameState state);
    bool processTouch(int x, int y);

    void waitForTouch(int &x, int &y);

    void drawEndScreen(bool lost, int score);
private:

    // Attributes for spi for lcd
    gpio_rp2040_pin     lcdLED;
    gpio_rp2040_pin     lcdCS;
    gpio_rp2040_pin     lcdReset;
    gpio_rp2040_pin     lcdDC;
    spi_rp2040          spi;
    ssd2119_drv*        lcd;
    // uGUI - Framework for displaying simple objects
    uGUI*               gui;
    // Touch interface to process user input
    Touch*              touch = nullptr;
    // State of the application
    GameState           state;


    // Class for displaying the Menu-State
    Menu*               menu = nullptr;

    // Class for displaying the GameState
    GridUI*             gameUI = nullptr;
    Map*                gameMap = nullptr;
};


#endif //GAME_GAMEGUI_H
