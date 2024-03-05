//
// Created by tobia on 24.02.2024.
//

#include "Menu.h"
#include <cstdio>

/**
 * Constructor for the menu.  Will initialize some of the necessary attributes but not draw anything.
 *
 * @param gui The gui to draw on.
 */
Menu::Menu(uGUI &gui) :
    gui(gui){

}

/**
 * Method to draw the Menu.
 */
void Menu::draw() {
    screensizeX = gui.GetXDim() - 1;
    screensizeY = gui.GetYDim() - 1;

    gui.FillScreen(C_BLACK); // Reset screen
    // Draw Title
    drawHeadline();

    // Draw Settings
    drawSettings();
    printf("DEBUG       DEFAULT SETTINGS:\n");
    printf("                DimX  = %d\n", dimX);
    printf("                DimY  = %d\n", dimY);
    printf("                Bombs = %d\n", countBombs);

    // Draw Start Button
    drawStartBtn();
}

/**
 * Method to process a touch input.
 *
 * Method checks if one of the interactables had been clicked.
 *
 * @param x x-Coordinate of the touch input.
 * @param y y-Coordinate of the touch input.
 * @return Returns a boolean to indicate if the start button was clicked and a state transition is necessary.
 */
bool Menu::processTouch(int x, int y) {
    // Loop trough all interactables - for better readabilty enums were used so every method could be called by a name instead of integer.
    for(Interactable* ia : interactables){
        if (ia->gotTouched(x, y)){
            switch (ia->getFunction()) {
                case btnDecreaseDimX:
                    decreaseDimX();
                    return true;
                case btnIncreaseDimX:
                    increaseDimX();
                    return true;
                case btnDecreaseDimY:
                    decreaseDimY();
                    return true;
                case btnIncreaseDimY:
                    increaseDimY();
                    return true;
                case btnDecreaseCountButton:
                    decreaseCountBombs();
                    return true;
                case btnIncreaseCountButton:
                    increaseCountBombs();
                    return true;
                case btnStartGame:
                    printf("Starting Game\n");
                    return false; // false to indicate state transaction
            }
        }
    }
    return true; // True if no button was pressed
}

/**
 * Method to get the settings of the game (width, height as well as bombs).
 * @param width Width of the game's grid.
 * @param height Height of the game's grid.
 * @param countBomb Number of the Bombs.
 */
void Menu::getFieldSize(int &width, int &height, int &countBomb) {
    width = dimX;
    height = dimY;
    countBomb = countBombs;
}

/**
 * Method to draw the Headline with rounded corners
 */
void Menu::drawHeadline() {
    // Define Title, Font, etc.
    String titleString = "MCTsweeper";
    gui.FontSelect(&FONT_22X36);
    gui.SetBackcolor(C_LIGHT_BLUE);
    gui.SetForecolor(C_BLACK);
    // Calculate coordinates
    titleMinX = (screensizeX / 2) - (FONT_22X36.char_width * titleString.size()) / 2;
    titleMinY = 10;
    titleMaxX = titleMinX + FONT_22X36.char_width * titleString.size();
    titleMaxY = titleMinY + FONT_22X36.char_height;
    gui.FillCircle(titleMinX - 5,
                   titleMinY + (titleMaxY - titleMinY) / 2,
                   FONT_22X36.char_height/2 + 5,
                   C_LIGHT_BLUE);
    gui.FillCircle(titleMaxX + 5,
                   titleMinY + (titleMaxY - titleMinY) / 2,
                   FONT_22X36.char_height/2 + 5,
                   C_LIGHT_BLUE);
    gui.FillFrame(titleMinX - 5,
                  titleMinY - 5,
                  titleMaxX + 10,
                  titleMaxY + 5,
                  C_LIGHT_BLUE);
    gui.PutString(titleMinX, titleMinY, titleString);
}

/**
 * Method to draw Settings and the corresponding buttons. Each button will be added to the list interactables so that a interaction via touch is possible.
 */
void Menu::drawSettings() {
    gui.FontSelect(&FONT_10X16);
    gui.SetBackcolor(C_BLACK);
    gui.SetForecolor(C_LIGHT_GRAY);
    // Settings-Text
    gui.PutString(titleMinX + 10, titleMaxY + 30, "Feldbreite:");
    gui.PutString(titleMinX + 10, titleMaxY + FONT_10X16.char_height * 1.5 + 30, "Feldhöhe:");
    gui.PutString(titleMinX + 10, titleMaxY + FONT_10X16.char_height * 3 + 30, "Anz. Bomben:");
    //Settings Buttons
    // -> Dim X
    uint16_t settingsButtonsMinX = titleMinX + 20 + FONT_10X16.char_width * 13;
    gui.FillFrame(settingsButtonsMinX,
                  titleMaxY + 30,
                  settingsButtonsMinX + 16,
                  titleMaxY + 46,
                  C_LIGHT_BLUE);
    gui.SetBackcolor(C_LIGHT_BLUE);
    gui.SetForecolor(C_RED);
    gui.PutString(settingsButtonsMinX + 4,
                  titleMaxY + 30,
                  "-");
    // add minus dimx to interactables
    interactables[interactables_next] = new Interactable(settingsButtonsMinX,
                                        titleMaxY + 30,
                                        settingsButtonsMinX + 16,
                                        titleMaxY + 46,
                                        functions::btnDecreaseDimX);
    interactables_next += 1;
    settingsWriteDimX(); // Write the current value of the setting
    gui.FillFrame(settingsButtonsMinX + 50,
                  titleMaxY + 30,
                  settingsButtonsMinX + 66,
                  titleMaxY + 46,
                  C_LIGHT_BLUE);
    gui.SetBackcolor(C_LIGHT_BLUE);
    gui.SetForecolor(C_GREEN);
    gui.PutString(settingsButtonsMinX + 54,
                  titleMaxY + 30,
                  "+");
    // add plus dimx to interactables
    interactables[interactables_next] = new Interactable(settingsButtonsMinX + 50,
                                        titleMaxY + 30,
                                        settingsButtonsMinX + 66,
                                        titleMaxY + 46,
                                        functions::btnIncreaseDimX);
    interactables_next += 1;


    // -> Dim Y
    gui.FillFrame(settingsButtonsMinX,
                  titleMaxY + FONT_10X16.char_height * 1.5 + 30,
                  settingsButtonsMinX + 16,
                  titleMaxY + FONT_10X16.char_height * 1.5 + 46,
                  C_LIGHT_BLUE);
    gui.SetBackcolor(C_LIGHT_BLUE);
    gui.SetForecolor(C_RED);
    gui.PutString(settingsButtonsMinX + 4,
                  titleMaxY + FONT_10X16.char_height * 1.5 + 30,
                  "-");
    // add minus dimy to interactables
    interactables[interactables_next] = new Interactable(settingsButtonsMinX,
                                        titleMaxY + FONT_10X16.char_height * 1.5 + 30,
                                        settingsButtonsMinX + 16,
                                        titleMaxY + FONT_10X16.char_height * 1.5 + 46,
                                        functions::btnDecreaseDimY);
    interactables_next += 1;
    settingsWriteDimY(); // Write the current value of the setting
    gui.FillFrame(settingsButtonsMinX + 50,
                  titleMaxY + FONT_10X16.char_height * 1.5 + 30,
                  settingsButtonsMinX + 66,
                  titleMaxY + FONT_10X16.char_height * 1.5 + 46,
                  C_LIGHT_BLUE);
    gui.SetBackcolor(C_LIGHT_BLUE);
    gui.SetForecolor(C_GREEN);
    gui.PutString(settingsButtonsMinX + 54,
                  titleMaxY + FONT_10X16.char_height * 1.5 + 30,
                  "+");
    // add plus dimy to interactables
    interactables[interactables_next] = new Interactable(settingsButtonsMinX + 50,
                                        titleMaxY + FONT_10X16.char_height * 1.5 + 30,
                                        settingsButtonsMinX + 66,
                                        titleMaxY + FONT_10X16.char_height * 1.5 + 46,
                                        functions::btnIncreaseDimY);
    interactables_next += 1;

    // -> Count Bombs
    gui.FillFrame(settingsButtonsMinX,
                  titleMaxY + FONT_10X16.char_height * 3 + 30,
                  settingsButtonsMinX + 16,
                  titleMaxY + FONT_10X16.char_height * 3 + 46,
                  C_LIGHT_BLUE);
    gui.SetBackcolor(C_LIGHT_BLUE);
    gui.SetForecolor(C_RED);
    gui.PutString(settingsButtonsMinX + 4,
                  titleMaxY + FONT_10X16.char_height * 3 + 30,
                  "-");
    // add minus bombs to interactables
    interactables[interactables_next] = new Interactable(settingsButtonsMinX,
                                        titleMaxY + FONT_10X16.char_height * 3 + 30,
                                        settingsButtonsMinX + 16,
                                        titleMaxY + FONT_10X16.char_height * 3 + 46,
                                        functions::btnDecreaseCountButton);
    interactables_next += 1;
    settingsWriteBombs(); // write the current value of the setting
    gui.FillFrame(settingsButtonsMinX + 50,
                  titleMaxY + FONT_10X16.char_height * 3 + 30,
                  settingsButtonsMinX + 66,
                  titleMaxY + FONT_10X16.char_height * 3 + 46,
                  C_LIGHT_BLUE);
    gui.SetBackcolor(C_LIGHT_BLUE);
    gui.SetForecolor(C_GREEN);
    gui.PutString(settingsButtonsMinX + 54,
                  titleMaxY + FONT_10X16.char_height * 3 + 30,
                  "+");
    // add plus bombs to interactables
    interactables[interactables_next] = new Interactable(settingsButtonsMinX + 50,
                                        titleMaxY + FONT_10X16.char_height * 3 + 30,
                                        settingsButtonsMinX + 66,
                                        titleMaxY + FONT_10X16.char_height * 3 + 46,
                                        functions::btnIncreaseCountButton);
    interactables_next += 1;
}

/**
 * Method to draw the start button with rounded corners and adding it to the interactables list.
 */
void Menu::drawStartBtn() {
    // Text-Settings
    String btnString = "Starten";
    gui.FontSelect(&FONT_12X20);
    gui.SetBackcolor(C_BLUE);
    gui.SetForecolor(C_BLACK);
    // Calculate Coordinates
    int startBtnMinX = (screensizeX / 2) - (FONT_12X20.char_width * btnString.size()) / 2;
    int startBtnMinY = screensizeY - 10 - FONT_12X20.char_height;
    int startBtnMaxX = startBtnMinX + FONT_12X20.char_width * btnString.size();
    int startBtnMaxY = screensizeY - 10;
    // Draw Button with rounded corners
    gui.FillCircle(startBtnMinX - 5,
                   startBtnMinY + (startBtnMaxY - startBtnMinY) / 2,
                   FONT_16X20.char_height/2 + 5,
                   C_BLUE);
    gui.FillCircle(startBtnMaxX + 5,
                   startBtnMinY + (startBtnMaxY - startBtnMinY) / 2,
                   FONT_16X20.char_height/2 + 5,
                   C_BLUE);
    gui.FillFrame(startBtnMinX - 5,
                  startBtnMinY - 5,
                  startBtnMaxX + 10,
                  startBtnMaxY + 5,
                  C_BLUE);
    gui.PutString(startBtnMinX, startBtnMinY, btnString);
    // Add button to interactables list
    interactables[interactables_next] = new Interactable(startBtnMinX,
                                                         startBtnMinY,
                                                         startBtnMaxX,
                                                         startBtnMaxY,
                                                         functions::btnStartGame);
    interactables_next += 1;
}

/*
 * Draw DimX Value
 */
void Menu::settingsWriteDimX() {
    gui.FontSelect(&FONT_10X16);
    gui.SetBackcolor(C_BLACK);
    gui.SetForecolor(C_WHITE);
    String anz = dimX < 10 ? "0" + to_String(dimX) : to_String(dimX);
    gui.PutString(titleMinX + 45 + FONT_10X16.char_width * 13,
                  titleMaxY + 33,
                  anz);
}

/*
 * Draw DimY Value
 */
void Menu::settingsWriteDimY() {
    gui.FontSelect(&FONT_10X16);
    gui.SetBackcolor(C_BLACK);
    gui.SetForecolor(C_WHITE);
    String anz = dimY < 10 ? "0" + to_String(dimY) : to_String(dimY);
    gui.PutString(titleMinX + 45 + FONT_10X16.char_width * 13,
                  titleMaxY + FONT_10X16.char_height * 1.5 + 33,
                  anz);
}

/**
 * Draw bomb value
 */
void Menu::settingsWriteBombs() {
    gui.FontSelect(&FONT_10X16);
    gui.SetBackcolor(C_BLACK);
    gui.SetForecolor(C_WHITE);
    String anz = countBombs < 10 ? "0" + to_String(countBombs) : to_String(countBombs);
    gui.PutString(titleMinX + 45 + FONT_10X16.char_width * 13,
                  titleMaxY + FONT_10X16.char_height * 3 + 33,
                  anz);
}

/*
 * increase DimX
 */
void Menu::increaseDimX() {
    if (dimX == 99)
        dimX = 0;
    dimX += 1;
    settingsWriteDimX();
}

/**
 * increase DimY
 */
void Menu::increaseDimY() {
    if (dimY == 99)
        dimY = 0;
    dimY += 1;
    settingsWriteDimY();
}

/**
 * increase number of bombs
 */
void Menu::increaseCountBombs() {
    if (countBombs == 99)
        countBombs = 0;
    countBombs += 1;
    settingsWriteBombs();
}

/**
 * decrease dimX
 */
void Menu::decreaseDimX() {
    if (dimX <= 1)
        dimX = 100;
    dimX -= 1;
    settingsWriteDimX();
}

/**
 * decrease dimY
 */
void Menu::decreaseDimY() {
    if (dimY <= 1)
        dimY = 100;
    dimY -= 1;
    settingsWriteDimY();
}

/**
 * decrease number of bombs
 */
void Menu::decreaseCountBombs() {
    if (countBombs <= 1)
        countBombs = 100;
    countBombs -= 1;
    settingsWriteBombs();
}
