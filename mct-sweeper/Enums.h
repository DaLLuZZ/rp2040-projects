//
// Created by tobia on 19.01.2024.
//

#ifndef GAME_ENUMS_H
#define GAME_ENUMS_H
enum GameState{
    menu, game, end, stop
};

enum FieldState{
    empty, emptyClicked, flag, bomb, questionmark,
    one, two, three, four, five, six, seven, eight
};

enum Action{
    click, flagBomb, flagQuestionmark
};

#endif //GAME_ENUMS_H
