/*
 * Map.cpp
 *
 *  Contains the Map class implementation.
 *
 *  Created on: 23.02.2024
 *      Author: Patrick N. Michalke
 */
#include "Field.h"

Field::Field(Map* map, int index) : 
    map(map),
    index(index), 
    clicked(false), 
    state(FieldState::empty),
    neighbouringBombs(0) {
        this->neighbouringBombs = countNeighbourghBombs();
        this->state = intToFieldstate(neighbouringBombs);
    };

Field::Field(Map* map, int index, FieldState state) : 
    map(map),
    index(index), 
    clicked(false), 
    state(state),
    neighbouringBombs(0) {
        this->neighbouringBombs = countNeighbourghBombs();
        switch(state) {
            case FieldState::bomb:
                this->setFieldState(state);
                break;
            case FieldState::empty:
                this->setFieldState(state);
                break;
            case FieldState::emptyClicked:
                this->setFieldState(state);
                break;
            case FieldState::flag:
                this->setFieldState(state);
                break;
            case FieldState::questionmark:
                this->setFieldState(state);
                break;
            default:
                this->setFieldState(intToFieldstate(neighbouringBombs));
        }
    };

int Field::countNeighbourghBombs() {
    // Prüfe alle benachbarten Felder auf Bomben und zähle diese
    int counter = 0;
    int width = this->map->getWidth();
    int height = this->map->getHeight();

    bool isFirstRow = (this->index < width);
    bool isLastRow = (this->index < (width * height) ) && (this->index > (width * (height-1)));
    bool isFirstColumn = (this->index % width == 0);
    bool isLastColumn = (this->index % width == width - 1);

    if(!isFirstColumn) {
        if(this->map->getSolution()[this->index - 1].getFieldState() == FieldState::bomb) {
            counter++;
        }
    }
    if(!isLastColumn) {
        if(this->map->getSolution()[this->index + 1].getFieldState() == FieldState::bomb) {
            counter++;
        }
    }

    if(!isFirstRow) {
        if(this->map->getSolution()[this->index - width].getFieldState() == FieldState::bomb) {
            counter++;
        }
        if(!isFirstColumn) {
            if(this->map->getSolution()[this->index - width - 1].getFieldState() == FieldState::bomb) {
                counter++;
            }
        }
        if(!isLastColumn) {
            if(this->map->getSolution()[this->index - width + 1].getFieldState() == FieldState::bomb) {
                counter++;
            }
        }
    }

    if(!isLastRow) {
        if(this->map->getSolution()[this->index + width].getFieldState() == FieldState::bomb) {
            counter++;
        }
        if(!isFirstColumn) {
            if(this->map->getSolution()[this->index + width - 1].getFieldState() == FieldState::bomb) {
                counter++;
            }
        }
        if(!isLastColumn) {
            if(this->map->getSolution()[this->index + width + 1].getFieldState() == FieldState::bomb){
                counter++;
            }
        }
    }
    
    return counter;
}

FieldState Field::intToFieldstate(int n) {
    switch(n){
        case 1:
            return FieldState::one;
        case 2:
            return FieldState::two;
        case 3:
            return FieldState::three;
        case 4:
            return FieldState::four;
        case 5:
            return FieldState::five;
        case 6:
            return FieldState::six;
        case 7:
            return FieldState::seven;
        case 8:
            return FieldState::eight;
        default:
            return FieldState::emptyClicked;
    }
}

bool Field::getClicked() {
    return this->clicked;
}

void Field::setClicked(bool clicked) {
    this->clicked = clicked;
}

FieldState Field::getFieldState() {
    return this->state;
}

void Field::setFieldState(FieldState state) {
    this->state = state;
}