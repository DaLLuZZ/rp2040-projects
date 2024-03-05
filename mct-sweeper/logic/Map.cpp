/*
 * Map.cpp
 *
 *  Contains the Map class implementation.
 *
 *  Created on: 23.02.2024
 *      Author: Patrick N. Michalke
 */

#include "Map.h"
#include <cstdlib>
#include <cstdio>

Map::Map(int height, int width, int bombCount)
    : height(height),
      width(width),
      bombCount(bombCount),
      solution((Field*)malloc(sizeof(Field)*width*height)),
      currentState((Field*)malloc(sizeof(Field)*width*height)) {
    for( int i = 0; i < (height * width); i++) {
        // besetze den currentState und die Loesung mit Standardwerten
        this->currentState[i] = Field(this, i, FieldState::empty);
        this->solution[i] = Field(this, i, FieldState::emptyClicked);
    }

    this->generate();
}

uint32_t Map::generate_rnd(){
    // 32-bit Zufallszahl generieren mittels ring oscillator clock (ROSC)
    int k, random=0;
    int random_bit1, random_bit2;
    volatile uint32_t *rnd_reg=(uint32_t *)(ROSC_BASE + RANDOMBIT_OFFSET);
    
    for(k=0;k<32;k++){
        while(1){
            // hole zwei Zufallsbits, wenn sie ungleich sind, dann nehme das erste
            random_bit1=0x00000001 & (*rnd_reg);
            random_bit2=0x00000001 & (*rnd_reg);
            if(random_bit1!=random_bit2) break;
        }

	random = random << 1;        
    random=random + random_bit1;

    }
    return random;
}

void Map::checkNeighbours(int index) {
    // Alle benachbarten Felder überprüfen
    bool isFirstRow = (index < this->width);
    bool isLastRow = (index < (this->width * this->height) ) && (index > (this->width * (this->height-1)));
    bool isFirstColumn = (index % this->width == 0);
    bool isLastColumn = (index % this->width == this->width - 1);

    if(!isFirstColumn) {
        checkField(index - 1, true);
    }
    if(!isLastColumn) {
        checkField(index + 1, true);
    }

    if(!isFirstRow) {
        checkField(index - this->width, true);
        if(!isFirstColumn) {
            checkField(index - this->width - 1, true);
        }
        if(!isLastColumn) {
            checkField(index - this->width + 1, true);
        }
    }

    if(!isLastRow) {
        checkField(index + this->width, true);
        if(!isFirstColumn) {
            checkField(index + this->width - 1, true);
        }
        if(!isLastColumn) {
            checkField(index + this->width + 1, true);
        }
    }
}

void Map::checkField(int index, bool triggered) {
    Field* field = &this->currentState[index];
    if (field->getFieldState() == FieldState::emptyClicked) {
        // Geöffnete Felder nicht kontrollieren
        return;
    } else if (field->getFieldState() == FieldState::flag || field->getFieldState() == FieldState::questionmark) {
        // Flagge und Fragezeichen nicht kontrollieren
        return;
    } else {
        Field* sol = &this->solution[index];

        switch (sol->getFieldState()) {
            case bomb:
                // game over
                if(!triggered){
                    this->currentState = this->solution;
                }
                break;
            case emptyClicked: // Wenn in Lösung keine Zahl steht schaue Nachbarn an und offenbare Status
                field->setFieldState(sol->getFieldState());
                checkNeighbours(index);
                break;
            default: // Wenn Zahl in Lösung steht
                field->setFieldState(sol->getFieldState());
                break;
        }
        return;
    }
}

bool Map::hasWon() {
    // Win condition:
    // currentState == solution, wobei Bomben mit Flaggen besetzt sein müssen
    for (int i = 0; i < this->width * this->height; i++) {
        if (! (this->currentState[i].getFieldState() == this->solution[i].getFieldState() || (this->currentState[i].getFieldState() == FieldState::flag && this->solution[i].getFieldState() == FieldState::bomb)) ) {
            return false;
        }
    }
    return true;
}

void Map::clickField(int row, int column, Action action) {
    int index = row * this->width + column;

    if (!(this->currentState[index].getFieldState() == FieldState::emptyClicked)) {
        switch (action) {
            case click:
                // aufdecken
                Map::checkField(index, false);
                return;
            case flagBomb:
                if (this->currentState[index].getFieldState() == FieldState::flag) {
                     // Flagge weg nehmen
                    this->currentState[index].setFieldState(FieldState::empty);
                } else {
                     // Flagge weg setzen
                    if (this->currentState[index].getFieldState() == FieldState::empty) {
                        this->currentState[index].setFieldState(FieldState::flag);
                    }
                }
                return;
            case flagQuestionmark:
                // Fragezeichen weg nehmen
                if (this->currentState[index].getFieldState() == FieldState::questionmark) {
                    this->currentState[index].setFieldState(FieldState::empty);
                } else {
                     // Fragezeichen weg setzen
                    if (this->currentState[index].getFieldState() == FieldState::empty) {
                        this->currentState[index].setFieldState(FieldState::questionmark);
                    }
                }
                return;
        }
    }
    return;
}

Field* Map::generate() {
    for( int i = 0; i < this->bombCount; i++) {
        // Bomben an zufällige Orte setzen
        uint32_t bombSpot = generate_rnd() % (this->height*this->width);
        while(this->solution[bombSpot].getFieldState() == FieldState::bomb){
            if (bombSpot < (height+width)){
                bombSpot++;
            } else {
                bombSpot = 0;
            }
        }
        this->solution[bombSpot].setFieldState(FieldState::bomb);
    }

    for( int i = 0; i < (height * width); i++) {
        // Alle anderen Felder bekommen die Anzahl der benachbarten Bomben als Status
        Field* field = &this->solution[i];
        if (field->getFieldState() == bomb)
            continue;
        field->setFieldState(field->intToFieldstate(field->countNeighbourghBombs()));
    }
}

FieldState Map::getCurrentFieldState(int row, int column) {
    int index = row * this->width + column;
    return this->currentState[index].getFieldState();
}

int Map::getWidth() {
    return this->width;
}

int Map::getHeight() {
    return this->height;
}

Field* Map::getCurrentState() {
    return this->currentState;
}

Field* Map::getSolution() {
    return this->solution;
}