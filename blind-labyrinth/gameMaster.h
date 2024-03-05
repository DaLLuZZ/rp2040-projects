//
// Created by maurr on 28.02.2024.
//

#ifndef LABYRINTH_GAMEMASTER_H
#define LABYRINTH_GAMEMASTER_H

#include "labyrinth.h"
#include "SDTools/sdreader.h"
#include "gpio_rp2040.h"
#include "Player/player.h"
#include "string"
#include <posix_io.h>
#include "uart_rp2040.h"
#include "adc_rp2040.h"
#include <cmath>
#include "SDTools/sdreader.h"
#include "SDTools/sdWriter.h"

#define SWITCH1 16
#define SWITCH2 17
#define DEBOUNCESLEEP 200
#define GAMESPEED 50 //The delay between moves

class GameMaster{
private:
    uint8_t currentPhase = 0;
    //Joystick
    adc_rp2040_channel joy_X = adc_rp2040_channel(1);
    adc_rp2040_channel joy_Y = adc_rp2040_channel(3);
    uint16_t x_offset;
    uint16_t y_offset;

    //Buttons
    gpio_rp2040_pin top_button = gpio_rp2040_pin(SWITCH1);
    gpio_rp2040_pin bottom_button = gpio_rp2040_pin(SWITCH2);

    //TimerForButtons

    bool bounceGuardBottom = false;
    bool bounceGuardTop = false;

    bool sdCardMalfunctioning = false;

    int highscore = 0;
    Visualizer* visualizer;
    std::string currentMapName;
    ModeName mode;
    std::vector<int8_t> currentLabData;
    Labyrinth* labyrinth;
    Player* p = new Player();

public:
    
    GameMaster(){
        x_offset = joy_X.adcReadRaw();
        y_offset = joy_Y.adcReadRaw();
        bottom_button.gpioMode(GPIO::INPUT | GPIO::PULLUP);
        top_button.gpioMode(GPIO::INPUT | GPIO::PULLUP);
        visualizer = new Visualizer();
        labyrinth = new Labyrinth();

    }

    /// @brief Displays the titlescreen. Exits thes stage on button press.
    void startPhaseTitle(){
            visualizer->displayTitleScreen();

            while(currentPhase == 0){
                if(bottom_button.gpioRead() && bounceGuardBottom){
                    bounceGuardBottom = false;
                    task::sleep(DEBOUNCESLEEP);
                }
                if(!bottom_button.gpioRead() && !bounceGuardBottom){
                    currentPhase = 1;
                    bounceGuardBottom = true;
                }
            }
    }

    /// @brief Reads the map names from the sd if possible (otherwise provides the standard map and shows an error message). Exits the stage on choice by the player.
    void startPhaseMap(){
        visualizer ->stopAnimation();
        auto labNames = SDReader::ReadLabNamesFromSD();
        visualizer->displayChooseLab(labNames);
        int labNameListSize = labNames.size();
        if(labNameListSize == 0){
            sdCardMalfunctioning = true;
            visualizer->displayErrorScreen("SD Card ERROR");
            task::sleep(2000);
            return;
        }
        int previousChooseLabIndex = -1;
        int chooseLabIndex = 0;
        bool labChosen = false;
        visualizer->displayChoosePointer(chooseLabIndex, previousChooseLabIndex);

        while(!labChosen){
            if(bounceGuardBottom && bottom_button.gpioRead()){
                bounceGuardBottom = false;
                task::sleep(DEBOUNCESLEEP);
            }
            if(!bounceGuardBottom && !bottom_button.gpioRead()){
                labChosen = true;
                currentMapName = labNames[chooseLabIndex];
                bounceGuardBottom = true;
            }
            if(!bounceGuardTop && !top_button.gpioRead()){
                previousChooseLabIndex = chooseLabIndex;
                if(chooseLabIndex < labNameListSize-1){
                    chooseLabIndex++;
                }else{
                    chooseLabIndex = 0;
                }
                visualizer->displayChoosePointer(chooseLabIndex, previousChooseLabIndex);
                bounceGuardTop = true;
            }
            if(bounceGuardTop && top_button.gpioRead()){
                bounceGuardTop = false;
                task::sleep(DEBOUNCESLEEP);
            }
        }
    }

    /// @brief Displays the differnt playable modes. Exits the stage on choice taken by the player.
    void startPhaseMode(){
        auto modes = Mode::getModes();
        visualizer->displayChooseModes(Mode::ModesToString());
        int modesListSize = modes.size();
        int previousChooseModeIndex = -1;
        int chooseModeIndex = 0;
        bool modeChosen = false;
        visualizer->displayChoosePointer(chooseModeIndex, previousChooseModeIndex);

        while(!modeChosen){
            if(bounceGuardBottom && bottom_button.gpioRead()){
                bounceGuardBottom = false;
                task::sleep(DEBOUNCESLEEP);
            }
            if(!bounceGuardBottom && !bottom_button.gpioRead()){
                modeChosen = true;
                mode = modes[chooseModeIndex];
                bounceGuardBottom = true;
            }
            if(!bounceGuardTop && !top_button.gpioRead()){
                previousChooseModeIndex = chooseModeIndex;
                if(chooseModeIndex < modesListSize - 1){
                    chooseModeIndex++;
                }else{
                    chooseModeIndex = 0;
                }
                visualizer->displayChoosePointer(chooseModeIndex, previousChooseModeIndex);
                bounceGuardTop = true;
            }
            if(bounceGuardTop && top_button.gpioRead()){
                bounceGuardTop = false;
                task::sleep(DEBOUNCESLEEP);
            }
        }
        visualizer->displayLoadingScreen();

    }

    /// @brief Game phase where the player is trying to beat the labyrinth. Exits the stage on the goal being reached.
    void startPhaseLabyrinth(){
        if(!sdCardMalfunctioning){
            auto l = SDReader::ReadMapsFromSD(currentMapName);
            auto tuple = Labyrinth::convertStringLab(l);
            currentLabData = std::get<0>(tuple);
            highscore =  std::get<1>(tuple);
        }else{
            currentLabData = Labyrinth::getDefaultLab();
            highscore = 404;
        }

        labyrinth->InitLabyrinth(currentLabData, visualizer, mode);
        p->InitPlayer(labyrinth->getStartCoords()[0],labyrinth->getStartCoords()[1], labyrinth);
        visualizer->displayBlankScreen();
        if(mode != FLASH){
            labyrinth->visualizeLab(p->getX(),p->getY());
        }else{
            labyrinth->visualizeLab(p->getX(),p->getY(), true);
            task::sleep(2000);
            labyrinth->visualizeLab(p->getX(),p->getY());
        }

        p->StartPlayerStatCounts();
        while(currentPhase == 1){
            if(!bottom_button.gpioRead()) {
                uint16_t x_value = joy_X.adcReadRaw();
                uint16_t y_value = joy_Y.adcReadRaw();
                int x_diff = x_value - x_offset;
                int y_diff = y_value - y_offset;

                if(p->movePlayer(x_diff,y_diff)){
                    labyrinth->visualizeLab(p->getX(), p->getY());
                    task::sleep(GAMESPEED);
                }
            }

            if(p->getX() == labyrinth->getFinishCoords()[0] && p->getY() == labyrinth->getFinishCoords()[1])
            {
                currentPhase = 2;
            }
        }
    };

    /// @brief Displys the end screen. Calculates the score based on difficulty. Writes back to SD if new highscore was reached. Exits stage on button press.
    void startPhaseEnd(){
        bool highscoreBeaten = false;
        auto stats = p->getStats();
        stats[2] *= ((double)1+((double)mode / 10));
        if(stats[2] > highscore){
            highscore = stats[2];
            highscoreBeaten = true;
        }
        visualizer->displayEndScreen(stats, highscore, currentMapName);

        if(!sdCardMalfunctioning && highscoreBeaten){
            SDWriter::WriteLab(currentMapName, currentLabData, highscore);
        }

        bounceGuardBottom = true;
        while (currentPhase == 2){
            if(bottom_button.gpioRead()){
                bounceGuardBottom = false;
                task::sleep(DEBOUNCESLEEP);
            };
            if(!bottom_button.gpioRead() && !bounceGuardBottom){
                currentPhase = 0;
                bounceGuardBottom = true;
            }
        }
        visualizer->displayBlankScreen();
    };
};

#endif //LABYRINTH_GAMEMASTER_H
