/*
 * Field.h
 *
 *  Contains the Field class definition.
 *
 *  Created on: 23.02.2024
 *      Author: Patrick N. Michalke
 */
 
#ifndef FIELD_H
#define FIELD_H

#include "../Enums.h"
#include "Map.h"

class Map;

class Field {
    private:
        Map* map;
        int index;
        bool clicked;
        FieldState state;
        int neighbouringBombs;
    
    public:
        Field(Map* map, int index);
        Field(Map* map, int index, FieldState state);

        bool getClicked();
        void setClicked(bool clicked);

        FieldState getFieldState();
        void setFieldState(FieldState state);
        FieldState intToFieldstate(int n);
        int countNeighbourghBombs();
};

#endif //FIELD_H