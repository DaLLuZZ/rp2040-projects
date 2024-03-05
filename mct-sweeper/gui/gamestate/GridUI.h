//
// Created by tobia on 19.01.2024.
//

#ifndef GAME_GRIDUI_H
#define GAME_GRIDUI_H

#include <cmath>
#include <cstdio>

#include "uGUI.h"
#include "font_6x10.h"
#include "font_7x12.h"
#include "font_8x12.h"
#include "font_10x16.h"
#include "timer_rp2040.h"
#include "yahal_String.h"

#include "Enums.h"
#include "./gui/technical/Interactable.h"
#include "./logic/Map.h"


class GridUI {
public:
    GridUI() = delete;
    explicit GridUI(uGUI &gui,
                    int width,
                    int height,
                    Map gameMap,
                    int barHeight = 20);

    void drawMenuBar();
    void drawGrid();
    bool processTouch(int x, int y);
    void getScore(int &seconds, bool &lost);

private:
    // MenuBar Draw-Methods
    void drawClock();
    void printTime(int32_t sec);
    void startTime();
    void drawQMButton();
    void drawFlagButton();
    void drawClickButton();

    // Grid Draw Method
    void drawField(int x1, int y1, int x2, int y2, FieldState state);

private:
    // objects to communicate with the display and draw objects
    uGUI            gui;
    uGUI::FONT      font = FONT_6X10;
    // timer for game duration and score
    timer_rp2040 *  timer = nullptr;
    // settings of the game
    int             screenWidth;
    int             screenHeight;
    bool            clickedBomb = false;
    bool            gameOver = false;
    Map             gameMap;

    // MenuBar-Elements
    int32_t         seconds;
    int             mbBottomHeight;
    Action          buttonAction = click;
    Interactable*   interactables[3] = {nullptr, nullptr, nullptr};

    // Grid Elements
    int             gridWidth;
    int             gridHeight;
    int             fieldWidth;
    int             fieldHeight;
};


#endif //GAME_GRIDUI_H
