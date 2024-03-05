#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#include "labyrinth.h"
#include "Tiles/errorTile.h"
#include "Tiles/finishTile.h"
#include "Tiles/pathTile.h"
#include "Tiles/startTile.h"
#include "Tiles/wallTile.h"
#include "sstream"
//Construktor
Labyrinth::Labyrinth() {

}
/// @brief Returns the value or tile type at a specific location of the labyrinth 
/// @param row Row of the labyrinth (0 - 15)
/// @param col Column of the labyrinth (0 - 15)
/// @return Value at location or -1 if invalid
Tile* Labyrinth::GetTileAt(int row, int col) const {
    if(row >= 0 && col >= 0 && row < 16 && col < 16){
        return labyrinthBoard[row][col];
    }else{
        return nullptr;
    }
}
/// @brief Initializes the matrix of the labyrinth using a vector of uint8_t which shall have 256 entries
/// @param tileTypes vector of uint8_t with length of 256
/// @return -1 if vector has invalid amount of entries, -2 if vector contains an invalid value, 0 if everything went according to plan
int8_t Labyrinth::InitLabyrinth(const std::vector<int8_t> &tileTypes, Visualizer* _visualizer, int diff){
    this->difficulty = Mode(diff);
    this->visualizer = _visualizer;

    if( tileTypes.size() != 256){
        return -1;
    }
    for (uint16_t i = 0; i < (uint16_t)tileTypes.size(); i++) // CARE FOR DATATYPE
    {
        if(tileTypes[i] < 0){
            return -2;
        }
        this->labyrinthBoard[i%16][i/16] = InitTile(tileTypes[i],i%16,i/16);
    }
    return 0;
    
}


/// @brief Initializes a certain tile dependent on the type and position
/// @param tileType Type of the tile (0 = Path, 1 = Wall, 2 = Start, 3 = Finish, Else nullptr)
/// @param x X-position of the tile 
/// @param y Y-position of the tile
/// @return a new Tile
Tile* Labyrinth::InitTile(int8_t tileType, int x, int y)
{
    if(tileType < 0){
        return nullptr;
    }
    switch (tileType)
    {
    case 0:
        return new PathTile(x,y,difficulty.vision_range,visualizer, difficulty.tail_length);
    case 1:
        return new WallTile(x,y,difficulty.vision_range,visualizer);
    case 2:
        startCoords[0] = x;
        startCoords[1] = y;
        return new StartTile(x,y,visualizer);
    case 3:
        finishCoords[0] = x;
        finishCoords[1] = y;
        return new FinishTile(x,y,visualizer);
    default:
        return nullptr;
    }
}
/// @brief Iterates over the Tiles and executes their visualize method with the current position of the player
/// @param player_x x-coordinate of the player
/// @param player_y y-coordinate of the player
void Labyrinth::visualizeLab(uint8_t player_x, uint8_t player_y, bool vis_all) {
    for (int row = 0; row < 16; ++row) {
        for (int col = 0; col < 16; ++col) {
            Tile* currentTile = labyrinthBoard[row][col];
            currentTile->visualize(player_x,player_y, vis_all);
        }
    }
}
#pragma clang diagnostic pop

/// @brief Default Labyrinth for Testing
/// @return  Vector of int8_t with the tileTypes of the default labyrinth
std::vector<int8_t> Labyrinth::getDefaultLab(){
    std::vector<int8_t > labyrinth = {
            // Zeile 1
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            // Zeile 2
            1, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
            // Zeile 3
            1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
            // Zeile 4
            1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1,
            // Zeile 5
            1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1,
            // Zeile 6
            1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1,
            // Zeile 7
            1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1,
            // Zeile 8
            1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1,
            // Zeile 9
            1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1,
            // Zeile 10
            1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1,
            // Zeile 11
            1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1,
            // Zeile 12
            1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1,
            // Zeile 13
            1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
            // Zeile 14
            1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
            // Zeile 15
            1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 3, 1,
            // Zeile 16
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    return labyrinth;
}
/// @brief Getter for the coordinates of the finish tile
/// @return a vector with 2 entries. first is x-coord, second y-coord. Is {-1,-1} if invalid finish.
const std::vector<int8_t>& Labyrinth::getFinishCoords() const {
    return finishCoords;
}
/// @brief Getter for the coordinates of the start tile
/// @return a vector with 2 entries. first is x-coord, second y-coord. Is {-1,-1} if invalid start.
const std::vector<int8_t>& Labyrinth::getStartCoords() const {
    return startCoords;
}
/// @brief Checks for impassable (wall) tiles at the given coordinates.
/// @param x x-coord
/// @param y y-coord
/// @return true if passable tile, else false.
const bool Labyrinth::legalMove(int x, int y) const {
    return GetTileAt(x, y)->passable ;
}

/// @brief Converts read in LabyrinthData into a usable data format. Extracts the highscore.
/// @param stringVector tile types of the labyrinth and highscore at the end.
/// @return Tuple of tile types and highscore (vector<int8_t>, int).
std::tuple<std::vector<int8_t >,int>Labyrinth::convertStringLab(std::vector<std::string> stringVector) {
    std::vector<int8_t > integerVector;
    int highscore = std::stoi(stringVector.back());
    stringVector.pop_back();

    for (const std::string& line : stringVector) {
        std::stringstream ss(line);
        std::string token;

        while (std::getline(ss, token, ',')) {
            integerVector.push_back(std::stoi(token));
        }
    }
    std::tuple<std::vector<int8_t >,uint16_t> ret = {integerVector,highscore};
    return ret;
}
