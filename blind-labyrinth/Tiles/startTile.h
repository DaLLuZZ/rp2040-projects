#ifndef LABYRINTH_START_TILE_H 
#define LABYRINTH_START_TILE_H

#define STD_COLOR_START 0x07E0 //Green

#include "tile.h"
class StartTile: public Tile
{
private:
    /* data */
public:
    StartTile(int _x, int _y, Visualizer* _visualizer):Tile(_x,_y,64,_visualizer){};
    ~StartTile() = default;

    /// @brief Visualize the tile 
    /// @param player_x x-coords of the player (not needed)
    /// @param player_y y-coords of the player (not needed)
    /// @param vis_all  true when all tiles shall be visualized (not needed)
    void visualize(const int player_x, const int player_y, bool vis_all) override {
        if(vis_all){
            currently_visualized = true;
            visualizer->visualizeMe(this->pixel_values[0][0], this->pixel_values[1][0], this->pixel_values[0][1],
                                    this->pixel_values[1][1], STD_COLOR_START);
            return;
        }
        if(!currently_visualized){
            currently_visualized = true;
            visualizer->visualizeMe(this->pixel_values[0][0], this->pixel_values[1][0], this->pixel_values[0][1],
                                    this->pixel_values[1][1], STD_COLOR_START);
        }
        
    }
};


#endif // LABYRINTH_START_TILE_H