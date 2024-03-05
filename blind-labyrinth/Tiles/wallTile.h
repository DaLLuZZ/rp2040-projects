#ifndef LABYRINTH_WALL_TILE_H 
#define LABYRINTH_WALL_TILE_H

#define STD_COLOR_WALL 0x2820 // (Brown)

#include "tile.h"
class WallTile: public Tile
{
private:
    /* data */
public:
    WallTile(int _x, int _y, int _vision_range, Visualizer* _visualizer):Tile(_x,_y,_vision_range,_visualizer, false){};
    ~WallTile() = default;

        /// @brief Visualize the tile color depending on the player position and if all tiles shall be visualized
        /// @param player_x x-coords of the player
        /// @param player_y y-coords of the player
        /// @param vis_all  true when all tiles shall be visualized
        void visualize(const int player_x, const int player_y, bool vis_all) override {
        if(inVision(player_x, player_y) || vis_all){
            currently_visualized = true;
            visualizer->visualizeMe(this->pixel_values[0][0], this->pixel_values[1][0], this->pixel_values[0][1],
                                    this->pixel_values[1][1], STD_COLOR_WALL);
        }else if(currently_visualized){
            currently_visualized = false;
            visualizer->visualizeMe(this->pixel_values[0][0], this->pixel_values[1][0], this->pixel_values[0][1],
                                    this->pixel_values[1][1], STD_COLOR_OUTOFVISION);
        }
    }
};


#endif // LABYRINTH_WALL_TILE_H