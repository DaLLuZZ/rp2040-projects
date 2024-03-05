#ifndef PLAYER_H
#define PLAYER_H

#include "timer_rp2040.h"
#include <cmath>

/// @brief This class holds valiues which represent the player and its stats
class Player
{
private:
    /* data */
    uint8_t x;
    uint8_t y;
    const Labyrinth* labyrinth;
    int steps;
    int time;
    timer_rp2040 timer = timer_rp2040(1); //ALSO USED FOR ANIMATIONS!!

    /// @brief steps of player to caculate the score
    /// @return int steps
    [[nodiscard]] int getSteps() const{
        return steps;
    }

    /// @brief Stops the timer, whichis used to count the playtime and returns it
    /// @return an int in ms
    int getTime()
    {
        timer.stop();
        time = timer.getCounter() / 100000;
        return time;
    }
    /// @brief the score uses the time and steps to create a score. The score is later scaled with the difficulty by the gameMaster class
    /// @return int score
    [[nodiscard]] int getScore() const{
        return (1000000 / (time/10 + steps*10));
    }

public:

    Player():x(0),y(0),labyrinth(nullptr),steps(0),time(0){

    };

    ~Player() = default;

    /// @brief Initializes the player and sets all stats to 0.
    /// @param _x uint8_t (0 - 15) X - Start Position of the player (the start tile of the labyrinth)
    /// @param _y uint8_t (0 - 15) Y - Start Position of the player (the start tile of the labyrinth)
    /// @param lab the used labyrinth. Needed to check for legal moves
    void InitPlayer(uint8_t _x, uint8_t _y, const Labyrinth* lab){
        x = _x;
        y = _y;
        labyrinth = lab;
    }

    /// @brief sets Playerstats to 0 and start to count the playtime
    void StartPlayerStatCounts(){
        steps = 0;
        timer.resetCounter();
        timer.setCallback([&]() {  });
        timer.start();
    }

    /// @brief returns X position of player in labyrinth
    /// @return int x
    [[nodiscard]] int getX() const{return x;}
    bool setX(int _x)
    {
        if(labyrinth->legalMove(_x, y)){
        this->x = _x;
            return  true;
        }
        return false;
    }

    /// @brief returns Y position of player in labyrinth
    /// @return int y
    [[nodiscard]] int getY() const{return y;}
    bool setY(int _y){
        if(labyrinth->legalMove(x, _y)){
        this->y = _y;
            return true;
        }
        return false;
    }

    /// @brief moves the player to a new position im possible
    /// @param x_diff x value of joysticl
    /// @param y_diff y value of joystick
    /// @return bool stepTaken
    bool movePlayer(int x_diff, int y_diff){
        bool changed = false;
        if (abs(x_diff) > abs(y_diff)) {
            if (x_diff > 1200) {
                changed = setX(x + 1);
            } else if (x_diff < -1200) {
                changed = setX(x - 1);
            }
        } else {
            if (y_diff > 1200) {
                changed = setY(y - 1);
            } else if (y_diff < -1200) {
                changed = setY(y + 1);
            }
        }
        if(changed) steps++;
        return changed;
    }

    /// @brief Stats for score and Highscore
    /// @return a vector of 3 ints representing steps, time, and score
    std::vector<int> getStats(){
        return std::vector<int> {getSteps(), getTime(), getScore()};
    }
};


#endif PLAYER_H