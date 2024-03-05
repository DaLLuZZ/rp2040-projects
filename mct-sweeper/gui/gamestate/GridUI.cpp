//
// Created by tobia on 19.01.2024.
//

#include "GridUI.h"

/**
 * Constructor of the gridUI. Will initialize some of the necessary attributes but not draw anything.
 *
 * @param gui The gui to draw on.
 * @param dimX Number of columns of the grid.
 * @param dimY Number of rows of the grid.
 * @param barHeight Size of the menubar. Default value is set to 20px.
 */
GridUI::GridUI(uGUI &gui, int dimX, int dimY, Map gameMap, int barHeight)
        : gui(gui) , gameMap(gameMap), seconds(0), mbBottomHeight(barHeight){
    screenWidth = gui.GetXDim() - 1;
    screenHeight = gui.GetYDim() - 1;
    this->gridWidth = dimX;
    this->gridHeight = dimY;

    // Adapt font size to size of the menubar
    if (mbBottomHeight > 20)
        font = FONT_10X16;
    else if (mbBottomHeight > 18)
        font = FONT_8X12;
    else
        font = FONT_6X10;
}

/**
 * Method to draw the menuBar of the game.
 */
void GridUI::drawMenuBar() {
    gui.FillScreen(C_BLACK); // reset screen
    printf("DEBUG   PRINTING MENU BAR\n");
    gui.DrawLine(0, mbBottomHeight, screenWidth, mbBottomHeight, C_BLUE);
    drawClock();
    startTime();

    // Draw the three action-buttons
    drawQMButton();
    drawFlagButton();
    drawClickButton();
}

/**
 * Method to draw the game's grid.
 * First the size of the frames will be calculated. Afterwards the fields will be printed with the associated state.
 *
 * @returns none
 */
void GridUI::drawGrid() {
    // calculate size of the frames
    int currX = 1;
    int currY = mbBottomHeight+2;
    fieldWidth = (screenWidth - gridWidth - 3) / gridWidth;
    fieldHeight = (screenHeight - mbBottomHeight - gridHeight - 2 - 6) / gridHeight;

    // Loop through map and draw all fields
    for (int i = 0; i < gridHeight; ++i) {
        for (int j = 0; j < gridWidth; ++j) {
            drawField(currX, currY, currX + fieldWidth, currY + fieldHeight, gameMap.getCurrentFieldState(i,j));
            currX += fieldWidth + 2;
        }
        currX = 1;
        currY += fieldHeight + 2;
    }
}

/**
 * Method to process a touch input while the grid is displayed.
 *
 * Method checks if the touch was located in the menu bar or the game field.
 *
 * @param x x-Coordinate of the touch input.
 * @param y y-Coordinate of the touch input.
 * @return Returns a boolean to indicate if the game is over (won or lost). true if the game is still going, false if the game is over.
 */
bool GridUI::processTouch(int x, int y) {
    // If game is over the next click will lead you to the end-screen.
    if (gameOver)
        return false; // return false to indicate state-change
    // Menubar clicked?
    if (y < mbBottomHeight){
            // Loop through all interactables
            for (Interactable* ia : interactables) {
                // got this interactable touched?
                if (ia->gotTouched(x, y)) {
                    // wich interactable got touched? Because of the few interactables they are indicated by numbers and not a more readable method like enums.
                    switch (ia->getFunction()) {
                        case 0: // QM-Button
                            printf("DEBUG         -> CHANGING BUTTON STATE TO QM\n");
                            buttonAction = flagQuestionmark;
                            break;
                        case 1: // Flag Button
                            printf("DEBUG         -> CHANGING BUTTON STATE TO FLAG\n");
                            buttonAction = flagBomb;
                            break;
                        case 2: // Click-Button
                            printf("DEBUG         -> CHANGING BUTTON STATE TO CLICK\n");
                            buttonAction = click;
                            break;
                    }
                    // Draw Buttons again to indicate which mode is selected
                    drawFlagButton();
                    drawQMButton();
                    drawClickButton();
                    break;
                }
            }
        }
    // Click in Grid
    else {
        // Find out which field got touched
        int xField = x / fieldWidth;
        int yField = (y-mbBottomHeight) / fieldHeight;
        printf("DEBUG         -> CLICK IN FIELD (%d,%d)\n", xField, yField);
        switch (buttonAction) {
            case Action::click:
                printf("                WITH CLICK-ACTION\n");
                break;
            case Action::flagBomb:
                printf("                WITH FLAG-ACTION\n");
                break;
            case Action::flagQuestionmark:
                printf("                WITH QM-ACTION\n");
                break;
        }
        // update field
        gameMap.clickField(yField, xField, buttonAction);
        // Re-draw whole grid with updated state
        drawGrid(); // Draw grid will set flag clickedBomb to true, if a bomb has to be drawn
        if (gameMap.hasWon()) {
            printf("DEBUG           HAS WON");
            gameOver = true;
            timer->stop();
        }
        // If bomb got clicked enter Game-over-Mode where the grid is still visible but no new game-inputs are possible. Another click will lead you into the end-screen.
        if (clickedBomb){
            String text = "GAME OVER";
            gui.FillFrame(screenWidth * 0.2,
                          0,
                          screenWidth * 0.8,
                          24,
                          C_RED);
            gui.FontSelect(&FONT_10X16);
            gui.SetBackcolor(C_RED);
            gui.PutString(screenWidth * 0.5 - text.size() * FONT_10X16.char_width / 2,
                          (24 - FONT_10X16.char_height)/2,
                          text,
                          C_WHITE);
            // stop timer to register score
            timer->stop();
        }
    }
    return true;
}

/**
 * Method to get the seconds (score) and lost-flag.
 *
 * Params are passed-by-reference so multiple return-values are possible
 *
 * @param seconds The score in seconds
 * @param lost Flag indicating if the player clicked a bomb.
 */
void GridUI::getScore(int &seconds, bool &lost) {
    seconds = this->seconds;
    lost = clickedBomb;
}

/**
 * Method to drawMenuBar Clock.
 *
 * Clock consists out of multiple circles an two lines. Size is adjusted to size of MenuBar.
 *
 * @returns void
 */
void GridUI::drawClock() {
    printTime(seconds);
    gui.FillCircle((int16_t)(mbBottomHeight / 2), (int16_t)(mbBottomHeight / 2), (int16_t)(mbBottomHeight / 2 - 2), C_WHITE);
    gui.DrawCircle((int16_t)(mbBottomHeight / 2), (int16_t)(mbBottomHeight / 2), (int16_t)(mbBottomHeight / 2 - 3), C_LIGHT_GRAY);
    gui.DrawCircle((int16_t)(mbBottomHeight / 2), (int16_t)(mbBottomHeight / 2), (int16_t)(mbBottomHeight / 2 - 2), C_GOLD);
    gui.DrawLine((int16_t)(mbBottomHeight / 2), (int16_t)(mbBottomHeight / 2), (int16_t)(mbBottomHeight / 2), (int16_t )((mbBottomHeight / 2) * 0.6), C_BLACK);
    gui.DrawLine((int16_t)(mbBottomHeight / 2), (int16_t)(mbBottomHeight / 2), (int16_t)((mbBottomHeight / 2) * 1.3), (int16_t)(mbBottomHeight / 2), C_BLACK);
}

/**
 * Method to drawMenuBar elapsed time.
 *
 * Method creates String in mm:ss format out of the elapsed time in seconds. Usually triggered asynchrone by timer.
 *
 * @param sec Elapsed time since start
 * @return void
 */
void GridUI::printTime(int32_t  sec) {
    // Generate string displaying the correct time
    int min = sec/60;
    int s = sec%60;
    String t = ((min) < 10 ? "0" + to_String(min) : to_String(min)) +
            ":" + ((s) < 10 ? "0" + to_String(s) : to_String(s));
    int y_text = (mbBottomHeight - font.char_height) / 2;
    // Print string
    gui.FontSelect(&font);
    gui.SetBackcolor(C_BLACK);
    gui.SetForecolor(C_WHITE);
    gui.PutString((int16_t)(mbBottomHeight + 4), (int16_t)(y_text), t);
}

/**
 * Method to create and start timer.
 *
 * Timer runs periodically and increases the elapsed time every second as well as triggers the printTime-Method.
 *
 * @return void
 */
void GridUI::startTime() {
    timer = new timer_rp2040(2);
    timer->setPeriod(10e5, TIMER::PERIODIC);
    timer->setCallback([&]() {
        seconds++;
        printTime(seconds);
    });
    timer->start();
}

/**
 * Method to draw the QuestionMark Button.
 *
 * QM-Button will be placed in MenuBar. If button is selected in buttonAction the button will be highlighted.
 *
 * @returns void
 */
void GridUI::drawQMButton() {
    // Is button selected or not?
    UG_COLOR color = buttonAction == Action::flagQuestionmark ? C_DARK_ORANGE : C_GRAY;

    // Calculate coordinates
    int16_t x1 = (int16_t)(screenWidth / 2);
    int16_t y1 = 1;
    int16_t x2 = (int16_t)(screenWidth / 2 + mbBottomHeight * 1.5);
    int16_t y2 = mbBottomHeight - 2;

    // Drawing
    gui.SetBackcolor(C_BLACK);
    gui.DrawRoundFrame(x1, y1, x2, y2, 5, color);
    gui.FontSelect(&FONT_8X12);
    gui.PutString(x1 + (x2-x1-8)/2+1, y1 + (y2-y1-12)/2 + 1, "?");

    // Adding Interactable to listen to touches
    if (interactables[0] == nullptr){
        printf("DEBUG       ADDING QM-BTN TO INTERACTABLES\n");
        interactables[0] = new Interactable(x1, y1, x2, y2, 0); // 0 for QM-Button
    }
}

/**
 * Method to draw the FLag Button.
 *
 * Flag-Button will be placed in MenuBar. If button is selected in buttonAction the button will be highlighted.
 *
 * @returns void
 */
void GridUI::drawFlagButton() {
    // Is button selected or not?
    UG_COLOR color = buttonAction == Action::flagBomb ? C_DARK_ORANGE : C_GRAY;

    // Calculate coordinates
    int16_t x1 = (int16_t)(screenWidth / 2 + mbBottomHeight * 3 + 6);
    int16_t y1 = 1;
    int16_t x2 = (int16_t)(screenWidth / 2 + mbBottomHeight * 4.5 + 6);
    int16_t y2 = mbBottomHeight - 2;

    // Drawing Button
    gui.DrawRoundFrame(x1, y1, x2, y2, 5, color);
    gui.DrawLine((int16_t)(x1 + (x2-x1) / 2), (int16_t)(y1 + 2), (int16_t)(x1 + (x2-x1) / 2), y2 - 2, C_RED);
    gui.DrawLine((int16_t)(x1 + (x2-x1) / 2), (int16_t)(y1 + 2), (int16_t)(x1 + (x2-x1) * 0.7), y1 + (y2-y1) / 3, C_RED);
    gui.DrawLine((int16_t)(x1 + (x2-x1) *0.7),  (int16_t)(y1 + (y2-y1) / 3),(int16_t)(x1 + (x2-x1) / 2), (int16_t)(y1 + (y2-y1)/2), C_RED);

    // Adding Interactable to listen to touches
    if (interactables[1] == nullptr){
        printf("DEBUG       ADDING CLICK-BTN TO INTERACTABLES\n");
        interactables[1] = new Interactable(x1, y1, x2, y2, 1); // 1 for Flag-Button
    }
}

/**
 * Method to draw the Click Button.
 *
 * Click-Button will be placed in MenuBar. If button is selected in buttonAction the button will be highlighted.
 *
 * @returns void
 */
void GridUI::drawClickButton() {
    // Is button selected or not?
    UG_COLOR color = buttonAction == Action::click ? C_DARK_ORANGE : C_GRAY;

    // Calculate coordinates
    int16_t x1 = (int16_t) (screenWidth / 2 + mbBottomHeight * 1.5 + 3);
    int16_t y1 = 1;
    int16_t x2 = (int16_t) (screenWidth / 2 + mbBottomHeight * 3 + 3);
    int16_t y2 = mbBottomHeight - 2;

    // Drawing Button
    gui.DrawRoundFrame(x1, y1, x2, y2, 5, color);
    gui.FillCircle(x1 + (x2 - x1) / 2, y1 + (y2 - y1) / 2, (y2 - y1) / 3, C_WHITE);

    // Adding Interactable to listen to touches
    if (interactables[2] == nullptr) {
        printf("DEBUG       ADDING CLICK-BTN TO INTERACTABLES\n");
        interactables[2] = new Interactable(x1, y1, x2, y2, 2); // 1 for Flag-Button
    }
}

/**
 *  Method to draw the individual fields of the grid.
 *
 *  The content of the field is defined by the FieldState param. Possible States are empty, emptyClicked, one-eight, flag, questionmarked or flagged.
 *
 * @param x1 The x-Coordinate of the top left corner.
 * @param y1 The y-Coordinate of the top left corner.
 * @param x2 The x-Coordinate of the bottom right corner.
 * @param y2 The y-Coordinate of the bottm right corner.
 * @param state The state indicating the content of the field.
 */
void GridUI::drawField(int x1, int y1, int x2, int y2, FieldState state) {
    //printf("DRAWING FIELD WITH STATE: %s\n", to_String(state).c_str());
    switch (state) {
        case empty: // Not clicked -> filled frame
            gui.FillFrame(x1, y1, x2, y2, C_BLUE);
            break;
        case emptyClicked: // clicked -> frame
            gui.FillFrame(x1,y1,x2,y2, C_BLACK);
            gui.DrawFrame(x1, y1, x2, y2, C_BLUE);
            break;
        case one: // clicked with one bomb in adjacent field
            gui.FillFrame(x1,y1,x2,y2, C_BLACK);
            gui.DrawFrame(x1, y1, x2, y2, C_BLUE);
            gui.SetForecolor(C_GREEN);
            gui.SetBackcolor(C_BLACK);
            gui.FontSelect(&FONT_8X12);
            gui.PutString(x1 + (fieldWidth - FONT_8X12.char_width)/2,
                          y1 + (fieldHeight - FONT_8X12.char_height)/2,
                          "1");
            break;
        case two: // clicked with two bomb in adjacent field
            gui.FillFrame(x1,y1,x2,y2, C_BLACK);
            gui.DrawFrame(x1, y1, x2, y2, C_BLUE);
            gui.SetForecolor(C_YELLOW);
            gui.SetBackcolor(C_BLACK);
            gui.FontSelect(&FONT_8X12);
            gui.PutString(x1 + (fieldWidth - FONT_8X12.char_width)/2,
                          y1 + (fieldHeight - FONT_8X12.char_height)/2,
                          "2");
            break;
        case three: // clicked with three bomb in adjacent field
        case four:
        case five:
        case six:
        case seven:
        case eight:
            gui.FillFrame(x1,y1,x2,y2, C_BLACK);
            gui.DrawFrame(x1, y1, x2, y2, C_BLUE);
            gui.SetForecolor(C_RED);
            gui.SetBackcolor(C_BLACK);
            gui.FontSelect(&FONT_8X12);
            gui.PutString(x1 + (fieldWidth - FONT_8X12.char_width)/2,
                          y1 + (fieldHeight - FONT_8X12.char_height)/2,
                          to_String(state-4));
            break;
        case flag: // flagged field
            gui.FillFrame(x1, y1, x2, y2, C_BLUE);
            gui.DrawLine(x1 + fieldWidth / 3, y1 + fieldHeight * 0.1, x1 + fieldWidth / 3, y1 + fieldHeight * 0.9, C_RED);
            gui.DrawLine(x1 + fieldWidth / 3, y1 + fieldHeight * 0.1, x1 + fieldWidth / 3 + fieldHeight * 0.4, y1 + fieldHeight * 0.35, C_RED);
            gui.DrawLine(x1 + fieldWidth / 3 + fieldHeight * 0.4, y1 + fieldHeight * 0.35, x1 + fieldWidth / 3, y1 + fieldHeight * 0.6, C_RED);
            break;
        case questionmark:
            gui.FillFrame(x1, y1, x2, y2, C_BLUE);
            gui.SetForecolor(C_WHITE);
            gui.SetBackcolor(C_BLUE);
            gui.FontSelect(&FONT_8X12);
            gui.PutString(x1 + (fieldWidth - FONT_8X12.char_width)/2,
                          y1 + (fieldHeight - FONT_8X12.char_height)/2,
                          "?");
            break;
        case bomb: // field with bomb
            gui.DrawFrame(x1, y1, x2, y2, C_RED);
            gui.FillCircle(x1 + fieldWidth/2, y1 + fieldHeight/2, fieldHeight * 0.4, C_WHITE);
            // if bomb is drawn the game is over
            clickedBomb = true;
            gameOver = true;
            break;
    }
}

