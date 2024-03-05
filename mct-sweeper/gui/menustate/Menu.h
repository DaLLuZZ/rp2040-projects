//
// Created by tobia on 24.02.2024.
//

#ifndef GAME_MENU_H
#define GAME_MENU_H

#include "uGUI.h"
#include "font_16x20.h"
#include "font_10x16.h"
#include "font_12x20.h"
#include "font_22x36.h"
#include "yahal_String.h"
#include "gui/technical/Interactable.h"

class Menu {
public:
    Menu() = delete;
    explicit Menu(uGUI &gui);

    void draw();
    bool processTouch(int x, int y);
    void getFieldSize(int &width, int &height, int &countBomb);

private:
    void drawHeadline();
    void drawSettings();
    void drawStartBtn();

    void settingsWriteDimX();
    void settingsWriteDimY();
    void settingsWriteBombs();

    void increaseDimX();
    void increaseDimY();
    void increaseCountBombs();

    void decreaseDimX();
    void decreaseDimY();
    void decreaseCountBombs();

    // Coordinates and sizes for the objects
    uint16_t            titleMinX;
    uint16_t            titleMinY;
    uint16_t            titleMaxX;
    uint16_t            titleMaxY;
    int16_t             screensizeX;
    int16_t             screensizeY;

    // values of the settings for the game
    int16_t            dimX = 5;
    int16_t            dimY = 6;
    int16_t            countBombs = 10;

    // Objects for drawing objects on the display and checking if those objects were touched
    uGUI                gui;
    Interactable*       interactables[8];
    int16_t             interactables_next = 0;

    // private enum to map button functions to integer values
    enum functions{
        btnIncreaseDimX, btnIncreaseDimY, btnIncreaseCountButton,
        btnDecreaseDimX, btnDecreaseDimY, btnDecreaseCountButton,
        btnStartGame
    };
};


#endif //GAME_MENU_H
