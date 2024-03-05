#ifndef LABYRINTH_PATH_TILE_H
#define LABYRINTH_PATH_TILE_H

#define STD_COLOR_PATH 0x8AE4 //Brownish
#define STD_COLOR_PLAYER 0x680E //Purple
#define STD_COLOR_VISITED 0x001F //Blue

#include "tile.h"

class PathTile : public Tile{
private:
    
    int visited_countdown = 0;
    const uint16_t expiration_time;

public:
    PathTile(int _x, int _y, int _vision_range, Visualizer* _visualizer, uint16_t _expiration_time):Tile(_x,_y,_vision_range,_visualizer), expiration_time(_expiration_time) {};
    
    /// @brief Visualize the tile color depending on the player position, visited time and if all tiles shall be visualized
    /// @param player_x x-coords of the player
    /// @param player_y y-coords of the player
    /// @param vis_all  true when all tiles shall be visualized
    void visualize(const int player_x, const int player_y, bool vis_all) override {
        if(this->getX() == player_x && this->getY() == player_y){
            currently_visualized = true;
            visited_countdown = expiration_time;
            visualizer->visualizeMe(this->pixel_values[0][0], this->pixel_values[1][0], this->pixel_values[0][1],
                                    this->pixel_values[1][1], STD_COLOR_PLAYER);
        }else if(visited_countdown > 0) {
            currently_visualized = true;
            visited_countdown--;
            visualizer->visualizeMe(this->pixel_values[0][0], this->pixel_values[1][0], this->pixel_values[0][1],
                                    this->pixel_values[1][1], STD_COLOR_VISITED);
        }else if(inVision(player_x, player_y) || vis_all){
            currently_visualized = true;
            visualizer->visualizeMe(this->pixel_values[0][0], this->pixel_values[1][0], this->pixel_values[0][1],
                                    this->pixel_values[1][1], STD_COLOR_PATH);
        }else if(currently_visualized){
            currently_visualized = false;
            visualizer->visualizeMe(this->pixel_values[0][0], this->pixel_values[1][0], this->pixel_values[0][1],
                                    this->pixel_values[1][1], STD_COLOR_OUTOFVISION);
        }
        
    }

};

#endif //LABYRINTH_PATH_TILE_H
