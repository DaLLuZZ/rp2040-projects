

#ifndef LABYRINTH_CHARPATTERN_H
#define LABYRINTH_CHARPATTERN_H

#include <vector>

#define STD_CHARMESHSIZE 6

/// @brief charpatterns for a-z, 0-9, and " ", ".", "!", ",".
static std::vector<std::vector<uint8_t>> charPattern = {
        // A
        {
                0b1111,
                0b1001,
                0b1001,
                0b1111,
                0b1001,
                0b1001
        },
        // B
        {
                0b1110,
                0b1001,
                0b1110,
                0b1001,
                0b1001,
                0b1110
        },
        // C
        {
                0b0111,
                0b1000,
                0b1000,
                0b1000,
                0b1000,
                0b0111
        },
        // D
        {
                0b1110,
                0b1001,
                0b1001,
                0b1001,
                0b1001,
                0b1110
        },
        // E
        {
                0b1111,
                0b1000,
                0b1110,
                0b1000,
                0b1000,
                0b1111
        },
        // F
        {
                0b1111,
                0b1000,
                0b1110,
                0b1000,
                0b1000,
                0b1000
        },
        // G
        {
                0b0111,
                0b1000,
                0b1000,
                0b1011,
                0b1001,
                0b0111
        },
        // H
        {
                0b1001,
                0b1001,
                0b1111,
                0b1001,
                0b1001,
                0b1001
        },
        // I
        {
                0b111,
                0b010,
                0b010,
                0b010,
                0b010,
                0b111
        },
        // J
        {
                0b1111,
                0b0010,
                0b0010,
                0b0010,
                0b1010,
                0b0100
        },
        // K
        {
                0b1001,
                0b1010,
                0b1100,
                0b1100,
                0b1010,
                0b1001
        },
        // L
        {
                0b1000,
                0b1000,
                0b1000,
                0b1000,
                0b1000,
                0b1111
        },
        // M
        {
                0b10001,
                0b11011,
                0b10101,
                0b10101,
                0b10001,
                0b10001
        },
        // N
        {
                0b1001,
                0b1101,
                0b1011,
                0b1001,
                0b1001,
                0b1001
        },
        // O
        {
                0b0110,
                0b1001,
                0b1001,
                0b1001,
                0b1001,
                0b0110
        },
        // P
        {
                0b1110,
                0b1001,
                0b1110,
                0b1000,
                0b1000,
                0b1000
        },
        // Q
        {
                0b0110,
                0b1001,
                0b1001,
                0b1011,
                0b1001,
                0b0111
        },
        // R
        {
                0b1110,
                0b1001,
                0b1110,
                0b1100,
                0b1010,
                0b1001
        },
        // S
        {
                0b0111,
                0b1000,
                0b0110,
                0b0001,
                0b1001,
                0b0110
        },
        // T
        {
                0b11111,
                0b00100,
                0b00100,
                0b00100,
                0b00100,
                0b00100
        },
        // U
        {
                0b1001,
                0b1001,
                0b1001,
                0b1001,
                0b1001,
                0b0110
        },
        // V
        {
                0b1001,
                0b1001,
                0b1001,
                0b1001,
                0b0101,
                0b0010
        },
        // W
        {
                0b10001,
                0b10001,
                0b10101,
                0b10101,
                0b11011,
                0b10001
        },
        // X
        {
                0b1001,
                0b1001,
                0b0110,
                0b0110,
                0b1001,
                0b1001
        },
        // Y
        {
                0b10001,
                0b10001,
                0b01010,
                0b00100,
                0b00100,
                0b00100
        },
        // Z
        {
                0b1111,
                0b0010,
                0b0100,
                0b1000,
                0b1000,
                0b1111
        },
        // Digits from 0 bis 9
        {
                0b011100,
                0b100010,
                0b100010,
                0b100010,
                0b100010,
                0b011100
        },
        {
                0b00100,
                0b01100,
                0b10100,
                0b00100,
                0b00100,
                0b11110
        },
        {
                0b011100,
                0b100010,
                0b000100,
                0b001000,
                0b010000,
                0b111110
        },
        {
                0b11110,
                0b00010,
                0b01110,
                0b00010,
                0b00010,
                0b11110
        },
        {
                0b000010,
                0b000110,
                0b001010,
                0b010010,
                0b111111,
                0b000010
        },
        {
                0b111110,
                0b100000,
                0b111100,
                0b000010,
                0b000010,
                0b111100
        },
        {
                0b011110,
                0b100000,
                0b111100,
                0b100010,
                0b100010,
                0b011100
        },
        {
                0b111110,
                0b000010,
                0b000100,
                0b001000,
                0b010000,
                0b100000
        },
        {
                0b011100,
                0b100010,
                0b011100,
                0b100010,
                0b100010,
                0b011100
        },
        {
                0b011100,
                0b100010,
                0b111110,
                0b000010,
                0b000010,
                0b011100
        },
        // Punkt
        {
                0b0000,
                0b0000,
                0b0000,
                0b0000,
                0b1100,
                0b1100
        },
        // Ausrufezeichen
        {
                0b1100,
                0b1100,
                0b1100,
                0b0000,
                0b1100,
                0b1100
        },
        // Komma
        {
                    0b0000,
                    0b0000,
                    0b0000,
                    0b0000,
                    0b1100,
                    0b1100
        }
};

/// @brief creates colorstream from text to be used with the drawArea Method
/// @param text the Text to be written on screen
/// @param colorForeground RGB656 value
/// @param colorBackground RGB656 value
/// @param scaling base scaling is 6x6
/// @return 
static std::vector<uint16_t> createColorStream(const char* text, uint16_t colorForeground, uint16_t colorBackground, int scaling = 1) {
    std::vector<uint16_t> colorStream;

    // Durchlaufe jedes Zeilenmuster für jedes Zeichen im Text
    for (int row = 0; row < STD_CHARMESHSIZE; ++row) {
        for(int i = 0; i < scaling; i++) {
            for (const char *ptr = text; *ptr != '\0'; ++ptr) {
                char c = *ptr;
                //When the char is empty one can just add 6 background pixels in every row.
                if (c == ' ') {
                    for (int col = 0; col < STD_CHARMESHSIZE; ++col) {
                        colorStream.push_back(colorBackground);
                    }
                } else {
                    //Now evaluate the other necessary ASCII symbols
                    int index;
                    if (isalpha(c)) {
                        index = toupper(c) - 'A';
                    } else if (isdigit(c)) {
                        index = c - '0' + 26;
                    } else if (c == '.') {
                        index = 36;
                    } else if (c == '!') {
                        index = 37;
                    } else if (c == ',') {
                        index = 38;
                    } else {
                        continue;
                    }
                    //
                    for (int col = 0; col < STD_CHARMESHSIZE; ++col) {
                        // bool evaluates if charpattern is 1 or 0 with a byte mask (wie bei den Registern :,) )
                        bool isForeground = charPattern[index][row] & (1 << (STD_CHARMESHSIZE - 1 - col));
                        for (int j = 0; j < scaling; j++) {
                            colorStream.push_back(isForeground ? colorForeground : colorBackground);
                        }

                    }
                }
            }
        }
    }

    return colorStream;
}




#endif //LABYRINTH_CHARPATTERN_H
