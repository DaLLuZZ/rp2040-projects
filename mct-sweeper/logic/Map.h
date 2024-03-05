/*
 * Map.h
 *
 *  Contains the map class definition.
 *
 *  Created on: 23.02.2024
 *      Author: Patrick N. Michalke
 */
 
#ifndef MAP_H
#define MAP_H

#include "../Enums.h"
#include "Field.h"
#include <cstdint>

class Field;

#define ROSC_BASE 0x40060000
#define RANDOMBIT_OFFSET 0x1c

class Map {
    private:
        int height;
        int width;
        Field* solution;
        Field* currentState;
        int bombCount;
        
        Field* generate();
        void checkField(int index, bool triggered);
        void checkNeighbours(int index);
        uint32_t generate_rnd();
        
    public:
        Map(int height, int width, int bombCount);
        void clickField(int row, int column, Action action);
        FieldState getCurrentFieldState(int row, int column);
        int getWidth();
        int getHeight();
        Field* getSolution();
        Field* getCurrentState();
        bool hasWon();
};

#endif // MAP_H