#ifndef LABYRINTH_FINISH_TILE_H 
#define LABYRINTH_FINISH_TILE_H

#define STD_COLOR_FINISH 0xF800 //Red

#include "tile.h"
class FinishTile: public Tile
{
private:
    /* data */
public:
    FinishTile(int _x, int _y, Visualizer* _visualizer):Tile(_x,_y,64,_visualizer){};
    ~FinishTile() = default;

    /// @brief Visualize the tile 
    /// @param player_x x-coords of the player (not needed)
    /// @param player_y y-coords of the player (not needed)
    /// @param vis_all  true when all tiles shall be visualized (not needed)
    void visualize(const int player_x, const int player_y, bool vis_all) override {
        if(!currently_visualized){
            currently_visualized = true;
            visualizer->visualizeMe(this->pixel_values[0][0], this->pixel_values[1][0], this->pixel_values[0][1],
                                    this->pixel_values[1][1], STD_COLOR_FINISH);
        }
    }
};


#endif // LABYRINTH_FINISH_TILE_H