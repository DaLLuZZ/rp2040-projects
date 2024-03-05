//
// Created by tobia on 19.01.2024.
//

#include "GameGUI.h"
#include "logic/Map.h"

/**
 * Constructor for Class GameGUI. GameGUI will handle the connection to the display, the touch-interface as well as some of the game's logic.
 * The constructor creates spi to the display and starts the touch-calibration.
 */
GameGUI::GameGUI()
    : lcdLED(14),
    lcdCS(5),
    lcdReset(17),
    lcdDC(23),
    spi(0,4,7,22,lcdCS, SPI::MASTER, SPI::CPOL_1 | SPI::CPHA_1 | SPI::_8_BIT){
    printf("DEBUG   Created GameGUI-Object\n");
    lcdLED.gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);
    spi.setSpeed(24000000);
    lcd = new ssd2119_drv(spi, lcdReset, lcdDC, ssd2119_drv::Kentec_K350QVG);

    lcd->clearScreen(C_BLACK);
    gui = new uGUI(*lcd);

    touch = new Touch(*lcd, *gui);
    touch->calibrate();
}

/**
 * Method to draw the current state of the game. After a state transition the neccessary variables will be collected and passed to the next game state.
 *
 * @param state Current state of the game. Is responsible for the active screen.
 */
void GameGUI::draw(GameState state) {
    this->state = state;
    switch (this->state) {
        case GameState::menu: // Menu to start the game and change the settings
            printf("DEBUG   DRAWING MENU STATE\n");
            if (menu == nullptr)
                menu = new Menu(*gui);
            menu->draw();
            break;
        case GameState::game: // Game
            printf("DEBUG   DRAWING GAME STATE\n");
            int width, height, countBomb;
            menu->getFieldSize(width, height, countBomb);
            if (gameMap == nullptr)
                gameMap = new Map(height, width, countBomb);
            printf("DEBUG   GAME MAP GENERATED\n");
            if (gameUI == nullptr)
                gameUI = new GridUI(*gui, width, height, *gameMap);
            gameUI->drawMenuBar();
            gameUI->drawGrid();
            break;
        case GameState::end: // Endscreen displayed after winning or loosing the game
            printf("DEBUG   DRAWING END STATE\n");
            int score;
            bool lost;
            gameUI->getScore(score, lost);
            drawEndScreen(lost, score);
            break;
    }
}

/**
 * Method to connect the game-loop with the screens. The inputs are passed to the active screen depending of the current state.
 * @param x x-Coordinate of the touch.
 * @param y y-Coordinate of the touch.
 * @return Returns a boolean to indicate a state transition. false if the current state is outdated and a transition is necessary.
 */
bool GameGUI::processTouch(int x, int y) {
    switch (state) {
        case GameState::menu:
            return menu->processTouch(x,y);
        case GameState::game:
            return gameUI->processTouch(x, y);
        case GameState::end:
            return false;
    }
}

/**
 * Method to trigger the waitForTouch-Method of the Touch-Class.
 * @param x x-Coordinate of the touch.
 * @param y y-Coordinate of the touch.
 */
void GameGUI::waitForTouch(int &x, int &y) {
    uint16_t xTouch, yTouch;
    touch->waitForPress(xTouch,yTouch);
    x = xTouch;
    y = yTouch;
}

/**
 * Method to draw end-screen (won or lost). Because there are no coordinate specific buttons this screen is not a whole class.
 *
 * @param lost Flag if the game was lost or won. The text of the screen is dependant on that.
 * @param score Score in seconds. Aka. the timespan the game was played.
 */
void GameGUI::drawEndScreen(bool lost, int score) {
    gui->FillScreen(C_BLACK); // Reset screen
    int screenWidth = gui->GetXDim()-1;
    int screenHeight = gui->GetYDim()-1;

    // Write if the game was lost or won
    String endText = lost ? "YOU'VE LOST" : "YOU'VE WON";
    gui->FillFrame(screenWidth * 0.05,
                   screenHeight * 0.3,
                   screenWidth * 0.95,
                   screenHeight * 0.5,
                   lost ? C_RED : C_GREEN);
    gui->FontSelect(&FONT_22X36);
    gui->SetForecolor(C_WHITE);
    gui->SetBackcolor(lost ? C_RED : C_GREEN);
    gui->PutString(screenWidth*0.2 + (screenWidth * 0.6 - FONT_22X36.char_width * endText.size())/2,
                   screenHeight*0.3 + (screenHeight * 0.2 - FONT_22X36.char_height)/2,
                   endText);

    // Write score
    int min = score/60;
    int s = score%60;
    String scoreText = "Score:" + ((min) < 10 ? "0" + to_String(min) : to_String(min)) +
               ":" + ((s) < 10 ? "0" + to_String(s) : to_String(s));
    gui->FontSelect(&FONT_10X16);
    gui->SetBackcolor(C_BLACK);
    gui->PutString((screenWidth - FONT_10X16.char_width * scoreText.size())/2,
                   screenHeight * 0.6,
                   scoreText);
    String continueText = "Reset to restart";
    gui->PutString((screenWidth - FONT_10X16.char_width * continueText.size())/2,
                   screenHeight * 0.8,
                   continueText);
}
