//
// Created by tobia on 24.02.2024.
//

#ifndef GAME_INTERACTABLE_H
#define GAME_INTERACTABLE_H

#include "yahal_String.h"

/**
 * Class representing touchable zones of the display.
 */
class Interactable{
private:
    // Bounds
    int             minX;
    int             maxX;
    int             minY;
    int             maxY;
    int             function_name; // Can be used via enums so functions can be mapped by name.

public:
    /**
     * Constructor of the Class Interactable to create touchable.
     * @param min1 The x-Coordinate of the top left corner.
     * @param min1 The y-Coordinate of the top left corner.
     * @param max2 The x-Coordinate of the bottom right corner.
     * @param max2 The y-Coordinate of the bottm right corner.
     * @param functionName The functionName or index of the interactable. Can be used with a enum.
     */
    Interactable(int minX, int minY, int maxX, int maxY, int functionName)
        : minX(minX), maxX(maxX), minY(minY), maxY(maxY), function_name(functionName){
    }

    /**
     * Method to check if interactable had been clicked.
     *
     * @param x x-Coordinate of the input.
     * @param y y-Coordinate of the input
     * @return Bool if the interactable had been clicked.
     */
    bool gotTouched(int x, int y){
        return minX < x && maxX > x && minY < y && maxY > y;
    }

    /**
     * Method to return the function name/index.
     * @return function name
     */
    int getFunction(){
        return function_name;
    }
};

#endif //GAME_INTERACTABLE_H
