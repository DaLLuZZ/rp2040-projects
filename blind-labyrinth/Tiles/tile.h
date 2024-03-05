#ifndef LABYRINTH_TILE_H
#define LABYRINTH_TILE_H

#include <cmath>
#include "../visualizer.h"

#define STD_COLOR_OUTOFVISION 0x0000 //Black

class Tile {
private:
    int x;
    int y;
    int vision_range;

    /// @brief calculates the coordinates for displaying the tile on the display
    void calculatePixels(){
        pixel_values[0][0] = x * 8;                     //pixel_x_min
        pixel_values[0][1] = pixel_values[0][0] + 7;    //pixel_x_max
        pixel_values[1][0] = y * 8;                     //pixel_y_min
        pixel_values[1][1] = pixel_values[1][0] + 7;    //pixel_y_max
    }
    

public:
    /// @brief The min and max values of the tile area [x][min, max] and [y][min, max]
    int8_t pixel_values[2][2]{};
    bool passable;
    Visualizer* visualizer;
    bool currently_visualized;

    /// @brief Constructor
    /// @param _x x coordinate of the tile
    /// @param _y y coordinate of the tile
    /// @param _vision_range vision range of the player ( depends on mode )
    Tile(int _x, int _y, int _vision_range, Visualizer *_visualizer, bool _passable = true) : x(_x), y(_y), vision_range(_vision_range), passable(_passable), visualizer(_visualizer), currently_visualized(false){
        calculatePixels();
    };
    /// @brief visualizing the tile on the display
    /// @param player_x player x coordinate
    /// @param player_y player y coordinate
    /// @param vis_all  true when all tiles shall be visualized
    virtual void visualize(int player_x, int player_y, bool vis_all) = 0;
    
    /// @brief checks if this tile is in vision of the player
    /// @param p_x x coordinate of the player
    /// @param p_y y coordinate of the player
    /// @return true if in vision, false if not
    [[nodiscard]] bool inVision(const int p_x, const int p_y) const{
        if(abs(p_x - x) <= vision_range && abs(p_y - y) <= vision_range){
            return true;
        }
        return false;
    }

    /// @brief Getter for x-coord
    /// @return int x-coord
    int getX() const{
        return this->x;
    }
    /// @brief  Getter for y-coord
    /// @return int y-coord
    int getY() const{
        return this->y;
    }

    /// @brief Destructor
    virtual ~Tile() = default;
};

#endif // LABYRINTH_TILE_H

