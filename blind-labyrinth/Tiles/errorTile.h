#ifndef LABYRINTH_ERROR_TILE_H 
#define LABYRINTH_ERROR_TILE_H

#define STD_COLOR_ERROR 0xFF00FF //Red

#include "tile.h"
class ErrorTile: public Tile
{
private:
    /* data */
public:
    ErrorTile(int _x, int _y, int _vision_range, Visualizer* _visualizer):Tile(_x,_y,_vision_range,_visualizer){};
    ~ErrorTile() = default;

    /// @brief Visualize the tile color depending on the player position, visited time and if all tiles shall be visualized
    /// @param player_x x-coords of the player
    /// @param player_y y-coords of the player
    /// @param vis_all  true when all tiles shall be visualized
    void visualize(const int player_x, const int player_y, bool vis_all) override {

        visualizer->visualizeMe(0, 0, 7, 7, STD_COLOR_ERROR);
    }
};


#endif // LABYRINTH_ERROR_TILE_H