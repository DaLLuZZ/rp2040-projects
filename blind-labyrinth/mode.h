#ifndef LABYRINTH_MODE_H
#define LABYRINTH_MODE_H

enum ModeName{
    EASY,
    MEDIUM,
    HARD,
    LIGHTS_OUT,
    EXTREME,
    FLASH,
    IMPOSSIBLE,
    Count
};


/// @brief Structure for the mode of the game. Determines the vision range and tail length of the player. Also can trigger a flash in gameMaster.
struct Mode
{
    int vision_range;
    int tail_length;
    Mode(){
        vision_range = 3;
        tail_length = 5;
    }
    static std::vector<ModeName> getModes(){
        std::vector<ModeName> result(ModeName::Count);
        for(int i = 0; i < static_cast<int>(ModeName::Count); ++i){
            result[i] = static_cast<ModeName>(i);
        }
        return result;
    }
    explicit Mode(int value)
    {
        switch (value)
        {
            case 1:
                vision_range = 2;
                tail_length = 3;
                break;
            case 2:
                vision_range = 1;
                tail_length = 2;
                break;
            case 3:
                vision_range = 0;
                tail_length = 9999;
                break;
            case 4:
                vision_range = 1;
                tail_length = 0;
                break;
            case 5:
                vision_range = 0;
                tail_length = 0;
                break;
            case 6:
                vision_range = 0;
                tail_length = 0;
                break;
            case 0: //EASY is the default
            default:
                vision_range = 2;
                tail_length = 4;
                break;
        }
    }

    /// @brief Used for displaying all difficulties to choose from on the DIfficulty Stage.
    /// @param value Name of the Mode
    /// @return the name of the Mode
    static std::string ModeToString(ModeName value) {
        switch (value) {
            case ModeName::EASY: return "Easy";
            case ModeName::MEDIUM: return "Medium";
            case ModeName::HARD: return "Hard";
            case ModeName::LIGHTS_OUT: return "Lights Out";
            case ModeName::EXTREME: return "Extreme";
            case ModeName::FLASH: return "Flash";
            case ModeName::IMPOSSIBLE: return "IMPOSSIBLE!";
            default: return "Unknown";
        }
    }

    /// @brief Returns all Modes
    /// @return vector<string> names
    static std::vector<std::string> ModesToString(){
        std::vector<std::string> result(ModeName::Count);
        for(int i = 0; i < static_cast<int>(ModeName::Count); ++i){
            result[i] = ModeToString(static_cast<ModeName>(i));
        }
        return result;
    }
};



#endif //LABYRINTH_MODE_H
