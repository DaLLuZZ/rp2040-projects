#ifndef LABYRINTH_LABYRINTH_H
#define LABYRINTH_LABYRINTH_H

#include "gpio_rp2040.h"
#include "sd_spi_drv.h"
#include "spi_rp2040.h"
#include "uart_rp2040.h"
#include "yahal_String.h"
#include "Tiles/tile.h"
#include "visualizer.h"
#include "mode.h"

#include <cstdio>
#include <cassert>
#include <cstring>
#include <boostxl_eduMKII.h>
#include <st7735s_drv.h>
#include "task.h"


class Labyrinth {
private:
    Tile* labyrinthBoard[16][16];
    Tile* InitTile(int8_t tileTypes, int x, int y);
    Mode difficulty;
    Visualizer* visualizer;
    std::vector<int8_t> startCoords = {-1, -1};
    std::vector<int8_t> finishCoords = {-1, -1};


public:
    Labyrinth();

    [[nodiscard]] Tile* GetTileAt(int row, int col) const;
    int8_t InitLabyrinth(const std::vector<int8_t> &tileTypes, Visualizer* _visualizer, int diff);
    void visualizeLab(uint8_t player_x, uint8_t player_y, bool vis_all = false);
    static std::vector<int8_t> getDefaultLab();
    [[nodiscard]] const std::vector<int8_t >& getStartCoords() const;
    [[nodiscard]] const std::vector<int8_t >& getFinishCoords() const;
    const bool legalMove(int x, int y) const;
    static std::tuple<std::vector<int8_t>,int> convertStringLab(std::vector<std::string > s_lab);
};






#endif //LABYRINTH_LABYRINTH_H
