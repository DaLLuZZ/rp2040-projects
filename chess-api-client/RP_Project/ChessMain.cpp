// ---------------------------------------------
//           This file is part of
//      _  _   __    _   _    __    __
//     ( \/ ) /__\  ( )_( )  /__\  (  )
//      \  / /(__)\  ) _ (  /(__)\  )(__
//      (__)(__)(__)(_) (_)(__)(__)(____)
//
//     Yet Another HW Abstraction Library
//      Copyright (C) Andreas Terstegge
//      BSD Licensed (see file LICENSE)
//
// ---------------------------------------------
//
// A simple example for displaying text, lines and
// circles on the LCD using the uGUI library.
// See the doc folder in YAHAL/src/uGUI for more
// information on the uGUI API.

#include "gpio_rp2040.h"
#include "spi_rp2040.h"
#include "st7735s_drv.h"
#include "uGUI.h"
#include "uGUI_colors.h"
#include "font_4x6.h"
#include "font_5x8.h"
#include "font_5x12.h"
#include "font_6x8.h"
#include "font_6x10.h"
#include "font_7x12.h"
#include "font_8x12.h"

#include "Color.h"
#include "ChessPieces.h"
#include "task.h"
#include "Menu.h"
#include "../ESP_Project/creds.h"
#include "json.hpp"
using json = nlohmann::json;

#include <cstdlib>
#include <adc_rp2040.h>
#include <uart_rp2040.h>
#include <posix_io.h>
#include <string>

#define BUTTON_DELAY 200

const std::string username = USERNAME;
uint8_t board_pixels[128][128][3];
uint8_t colors[2][3] = {{214,217,181},{131,86,49}};
uint8_t focusColor[3] = {20, 200, 20};
uint8_t errorColor[3] = {200, 20, 20};
uint8_t selectColor[3] = {50, 50, 200};
uint8_t focusedSquare[2] = {0, 0};
uint8_t selectedSquare[2] = {0, 0};
bool pieceSelected = false;
bool myTurn = false;
bool hasMoved = false;


enum Player_t {
    PLAYER_BLACK,
    PLAYER_WHITE
};

enum ChessPiece_t {
    KING_BLACK,
    KING_WHITE,
    QUEEN_BLACK,
    QUEEN_WHITE,
    BISHOP_BLACK,
    BISHOP_WHITE,
    KNIGHT_BLACK,
    KNIGHT_WHITE,
    ROOK_BLACK,
    ROOK_WHITE,
    PAWN_BLACK,
    PAWN_WHITE,
    NO_PIECE
};

ChessPiece_t board[8][8];

Player_t player = PLAYER_WHITE;

uart_rp2040 uart;
uart_rp2040 uart_esp(1, 20, 21, 4800);
std::vector<std::string> jsonStrings;
std::string msg;
uint8_t responseCount = 0;
std::string gameID = "";

Menu* menu;

void focusSquare(uGUI gui, uint8_t x, uint8_t y, bool markError=false);

void drawBoard(uGUI gui) {
    for (uint8_t i=0;i<128;i++){
        for (uint8_t j=0;j<128;j++){
            gui.DrawPixel(i, j, Color(board_pixels[i][j][0], board_pixels[i][j][1], board_pixels[i][j][2]));
        }
    }
}

void drawPiece(uGUI gui, uint8_t x, uint8_t y, const chess_piece_pixels_t piece) {
    for (uint8_t i=0;i<16;i++){
        for (uint8_t j=0;j<16;j++){
            if (piece[i][j][3] != 0) {
                gui.DrawPixel(i+x*16, j+y*16, Color(piece[i][j][0], piece[i][j][1], piece[i][j][2]));
            }
        }
    }
}

void drawPiece(uGUI gui, uint8_t x, uint8_t y, const ChessPiece_t piece) {
    switch (piece) {
        case KING_BLACK:
            drawPiece(gui, x, y, KING_BLACK_PIXELS);
            break;
        case KING_WHITE:
            drawPiece(gui, x, y, KING_WHITE_PIXELS);
            break;
        case QUEEN_BLACK:
            drawPiece(gui, x, y, QUEEN_BLACK_PIXELS);
            break;
        case QUEEN_WHITE:
            drawPiece(gui, x, y, QUEEN_WHITE_PIXELS);
            break;
        case BISHOP_BLACK:
            drawPiece(gui, x, y, BISHOP_BLACK_PIXELS);
            break;
        case BISHOP_WHITE:
            drawPiece(gui, x, y, BISHOP_WHITE_PIXELS);
            break;
        case KNIGHT_BLACK:
            drawPiece(gui, x, y, KNIGHT_BLACK_PIXELS);
            break;
        case KNIGHT_WHITE:
            drawPiece(gui, x, y, KNIGHT_WHITE_PIXELS);
            break;
        case ROOK_BLACK:
            drawPiece(gui, x, y, ROOK_BLACK_PIXELS);
            break;
        case ROOK_WHITE:
            drawPiece(gui, x, y, ROOK_WHITE_PIXELS);
            break;
        case PAWN_BLACK:
            drawPiece(gui, x, y, PAWN_BLACK_PIXELS);
            break;
        case PAWN_WHITE:
            drawPiece(gui, x, y, PAWN_WHITE_PIXELS);
            break;
        case NO_PIECE:
            break;
    }
}

void drawSquare(uGUI gui, uint8_t x, uint8_t y) {
    for (uint8_t i=x*16;i<x*16+16;i++){
        for (uint8_t j=y*16;j<y*16+16;j++){
            gui.DrawPixel(i, j, Color(board_pixels[i][j][0],board_pixels[i][j][1], board_pixels[i][j][2]));
        }
    }
}

void movePiece(uGUI gui, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    std::string move = (char) (97 + x1) + std::to_string(player==PLAYER_WHITE ? 8-y1 : y1+1) +
            (char) (97 + x2) + std::to_string(player==PLAYER_WHITE ? 8-y2 : y2+1); // Make move in UCI notation
    if ((y2 == 7 || y2 == 0) && (board[x1][y1] == PAWN_WHITE || board[x1][y1] == PAWN_BLACK)) {
        move += "q"; // Auto-Queen fo pawn promotion
    }
    uart.puts(std::string("Move" + move+ "\n\r").c_str());
    jsonStrings.clear();
    uart_esp.puts(std::string("POST:https://lichess.org/api/board/game/"+gameID+"/move/"+move+"\n").c_str());
    for (uint8_t i = 0; i < 100; i++) {
        if (!jsonStrings.empty())
            break;
        uart.puts(".");
        task::sleep(100); // Wait for response
    }
    if (jsonStrings.empty()) {
        uart.puts("Move failed! (no response)\n\r");
        focusSquare(gui, x2, y2, true);
        return;
    }
    json j = json::parse(jsonStrings[0]);
    if(!j.contains("ok")) { // Failed to move or illegal move
        uart.puts("Move failed! (invalid)\n\r");
        focusSquare(gui, x2, y2, true);
        return;
    }
    uart.puts("Move successful!\n\r");
    board[x2][y2] = board[x1][y1];
    board[x1][y1] = NO_PIECE;
    // check for auto queening
    if ((y2 == 7 || y2 == 0) && (board[x2][y2] == PAWN_BLACK || board[x2][y2] == PAWN_WHITE)) {
        board[x2][y2] = player == PLAYER_WHITE ? QUEEN_WHITE : QUEEN_BLACK;
    }
    // check for castling
    if (abs(x2 - x1) > 1 && (board[x2][y2] == KING_WHITE || board[x2][y2] == KING_BLACK)) {
        uint8_t rookX = x2 == 6 ? 5 : 3;
        uint8_t rookXOld = x2 == 6 ? 7 : 0;
        drawSquare(gui, rookXOld, y2);
        drawPiece(gui, rookX, y2, player == PLAYER_WHITE ? ROOK_WHITE : ROOK_BLACK);
        board[rookX][y2] = player == PLAYER_WHITE ? ROOK_WHITE : ROOK_BLACK;
        board[rookXOld][y2] = NO_PIECE;
    }
    drawSquare(gui, x1, y1);
    drawSquare(gui, x2, y2);
    drawPiece(gui, x2, y2, board[x2][y2]);
    myTurn = false;
    hasMoved = true;
}

void initBoard(uGUI gui) {
    if (player == PLAYER_WHITE) {
        drawPiece(gui, 0, 0, ROOK_BLACK_PIXELS);
        board[0][0] = ROOK_BLACK;
        drawPiece(gui, 1, 0, KNIGHT_BLACK_PIXELS);
        board[1][0] = KNIGHT_BLACK;
        drawPiece(gui, 2, 0, BISHOP_BLACK_PIXELS);
        board[2][0] = BISHOP_BLACK;
        drawPiece(gui, 3, 0, QUEEN_BLACK_PIXELS);
        board[3][0] = QUEEN_BLACK;
        drawPiece(gui, 4, 0, KING_BLACK_PIXELS);
        board[4][0] = KING_BLACK;
        drawPiece(gui, 5, 0, BISHOP_BLACK_PIXELS);
        board[5][0] = BISHOP_BLACK;
        drawPiece(gui, 6, 0, KNIGHT_BLACK_PIXELS);
        board[6][0] = KNIGHT_BLACK;
        drawPiece(gui, 7, 0, ROOK_BLACK_PIXELS);
        board[7][0] = ROOK_BLACK;
        for (uint8_t i = 0; i < 8; i++) {
            drawPiece(gui, i, 1, PAWN_BLACK_PIXELS);
            board[i][1] = PAWN_BLACK;
        }
        drawPiece(gui, 0, 7, ROOK_WHITE_PIXELS);
        board[0][7] = ROOK_WHITE;
        drawPiece(gui, 1, 7, KNIGHT_WHITE_PIXELS);
        board[1][7] = KNIGHT_WHITE;
        drawPiece(gui, 2, 7, BISHOP_WHITE_PIXELS);
        board[2][7] = BISHOP_WHITE;
        drawPiece(gui, 3, 7, QUEEN_WHITE_PIXELS);
        board[3][7] = QUEEN_WHITE;
        drawPiece(gui, 4, 7, KING_WHITE_PIXELS);
        board[4][7] = KING_WHITE;
        drawPiece(gui, 5, 7, BISHOP_WHITE_PIXELS);
        board[5][7] = BISHOP_WHITE;
        drawPiece(gui, 6, 7, KNIGHT_WHITE_PIXELS);
        board[6][7] = KNIGHT_WHITE;
        drawPiece(gui, 7, 7, ROOK_WHITE_PIXELS);
        board[7][7] = ROOK_WHITE;
        for (uint8_t i = 0; i < 8; i++) {
            drawPiece(gui, i, 6, PAWN_WHITE_PIXELS);
            board[i][6] = PAWN_WHITE;
        }
    } else {
        drawPiece(gui, 0, 7, ROOK_BLACK_PIXELS);
        board[0][7] = ROOK_BLACK;
        drawPiece(gui, 1, 7, KNIGHT_BLACK_PIXELS);
        board[1][7] = KNIGHT_BLACK;
        drawPiece(gui, 2, 7, BISHOP_BLACK_PIXELS);
        board[2][7] = BISHOP_BLACK;
        drawPiece(gui, 3, 7, QUEEN_BLACK_PIXELS);
        board[3][7] = QUEEN_BLACK;
        drawPiece(gui, 4, 7, KING_BLACK_PIXELS);
        board[4][7] = KING_BLACK;
        drawPiece(gui, 5, 7, BISHOP_BLACK_PIXELS);
        board[5][7] = BISHOP_BLACK;
        drawPiece(gui, 6, 7, KNIGHT_BLACK_PIXELS);
        board[6][7] = KNIGHT_BLACK;
        drawPiece(gui, 7, 7, ROOK_BLACK_PIXELS);
        board[7][7] = ROOK_BLACK;
        for (uint8_t i = 0; i < 8; i++) {
            drawPiece(gui, i, 6, PAWN_BLACK_PIXELS);
            board[i][6] = PAWN_BLACK;
        }
        drawPiece(gui, 0, 0, ROOK_WHITE_PIXELS);
        board[0][0] = ROOK_WHITE;
        drawPiece(gui, 1, 0, KNIGHT_WHITE_PIXELS);
        board[1][0] = KNIGHT_WHITE;
        drawPiece(gui, 2, 0, BISHOP_WHITE_PIXELS);
        board[2][0] = BISHOP_WHITE;
        drawPiece(gui, 3, 0, QUEEN_WHITE_PIXELS);
        board[3][0] = QUEEN_WHITE;
        drawPiece(gui, 4, 0, KING_WHITE_PIXELS);
        board[4][0] = KING_WHITE;
        drawPiece(gui, 5, 0, BISHOP_WHITE_PIXELS);
        board[5][0] = BISHOP_WHITE;
        drawPiece(gui, 6, 0, KNIGHT_WHITE_PIXELS);
        board[6][0] = KNIGHT_WHITE;
        drawPiece(gui, 7, 0, ROOK_WHITE_PIXELS);
        board[7][0] = ROOK_WHITE;
        for (uint8_t i = 0; i < 8; i++) {
            drawPiece(gui, i, 1, PAWN_WHITE_PIXELS);
            board[i][1] = PAWN_WHITE;
        }
    }
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 2; j < 6; j++) {
            board[i][j] = NO_PIECE;
        }
    }
}

void selectSquare(uGUI gui, uint8_t x, uint8_t y) {
    drawSquare(gui, selectedSquare[0], selectedSquare[1]);
    drawPiece(gui, selectedSquare[0], selectedSquare[1], board[selectedSquare[0]][selectedSquare[1]]);
    for (uint8_t i = 0; i < 16; i++) {
        for (uint8_t j = 0; j < 16; j++) {
            gui.DrawPixel(i+x*16, j+y*16, Color(selectColor));
        }
    }
    drawPiece(gui, x, y, board[x][y]);
    selectedSquare[0] = x;
    selectedSquare[1] = y;
}

void focusSquare(uGUI gui, uint8_t x, uint8_t y, bool markError) {
    drawSquare(gui, focusedSquare[0], focusedSquare[1]);
    drawPiece(gui, focusedSquare[0], focusedSquare[1], board[focusedSquare[0]][focusedSquare[1]]);
    if(pieceSelected && focusedSquare[0] == selectedSquare[0] && focusedSquare[1] == selectedSquare[1]) {
        selectSquare(gui, selectedSquare[0], selectedSquare[1]);
    }
    for (uint8_t i = 0; i < 16; i++) {
        for (uint8_t j = 0; j < 16; j++) {
            if (markError) {
                gui.DrawPixel(i+x*16, j+y*16, Color(errorColor));
            } else {
                gui.DrawPixel(i+x*16, j+y*16, Color(focusColor));
            }

        }
    }
    drawPiece(gui, x, y, board[x][y]);
    focusedSquare[0] = x;
    focusedSquare[1] = y;
}


bool createChallengeBlocking(const String& challenger, uint16_t challengeTimeout) {
    jsonStrings.clear();
    uart.puts("Challenging " + challenger + "\n");
    String req_str = "POST:https://lichess.org/api/challenge/" + challenger + "\n";
    uart_esp.puts(req_str.c_str());
    task::sleep(5000); // wait for response
    if(jsonStrings.empty())
        return false; //failed to challenge
    json j = json::parse(jsonStrings[0]);
    gameID = j["challenge"]["id"];
    String menuName = "Challenge...";
    for (uint64_t i = 0; i < challengeTimeout; i+=500) {
        req_str = "GET:https://lichess.org/api/board/game/stream/";
        req_str += gameID.c_str();
        req_str += "\n";
        uart_esp.puts(req_str.c_str());
        for (uint8_t i2 = 0; i2 < 20; i2++) {
            if(!jsonStrings.empty() && i2 > 5)
                break;
            menu->RefreshLoadMenu(menuName, i==0);
            task::sleep(500);
            uart.puts(".");
        } // Wait for all responses
        if (!jsonStrings.empty()) { // Check if accepted
            for (auto & jsonString : jsonStrings) {
                json j2 = json::parse(jsonString);
                if (j2.contains("id")) {
                    uart.puts("Challenge was accepted!\n\r");
                    return true;
                }
            }
            jsonStrings.clear();
        }
    }
    uart.puts("Cancel Challenge to " + challenger + "\n");
    req_str = "POST:https://lichess.org/api/challenge/";
    req_str += gameID.c_str();
    req_str += "\n";
    uart_esp.puts(req_str.c_str());
    return false; //Failed to create challenge or have it accepted before timeout
}

std::vector<std::string> getFriends() {
    jsonStrings.clear();
    std::vector<std::string> result;
    String req_str = "GET:https://lichess.org/api/rel/following\n";
    uart_esp.puts(req_str.c_str());
    String menuName = "Challenge...";
    for (uint8_t i = 0; i < 120; i++) {
        if(!jsonStrings.empty() && i >= (10+responseCount*2))
            break;
        menu->RefreshLoadMenu(menuName, i==0);
        task::sleep(500);
        uart.puts(".");
    } // Wait for all responses
    uart.puts("Friends: ");
    for (auto & jsonString : jsonStrings) {
        json j = json::parse(jsonString);
        std::string username = j["username"];
        uart.puts(username.c_str());
        uart.puts("\t");
        result.push_back(username);
    }
    uart.puts("\r\n");
    return result;
}

std::vector<std::vector<std::string>> getChallenges() {
    jsonStrings.clear();
    std::vector<std::vector<std::string>> result;
    String req_str = "GET:https://lichess.org/api/challenge\n";
    uart_esp.puts(req_str.c_str());
    String menuName = "Accept...";
    for (uint8_t i = 0; i < 120; i++) {
        if(!jsonStrings.empty() && i > 5)
            break;
        menu->RefreshLoadMenu(menuName, i==0);
        task::sleep(500);
        uart.puts(".");
    } // Wait for all responses
    uart.puts("Challenges:\n\r");
    for (auto & jsonString : jsonStrings) {
        json jChallenges = json::parse(jsonString);
        json jInChallenges = jChallenges["in"];
        for (auto& element : jInChallenges) {
            std::vector<std::string> challengeObject = {element["challenger"]["name"], element["id"]};
            std::string displayChallenge = challengeObject[0] + " "  + challengeObject[1];
            if (displayChallenge.length() > 15) {
                displayChallenge = displayChallenge.substr(0, 13) + "...";
            }
            challengeObject.push_back(displayChallenge);
            uart.puts("Challenge: ");
            uart.puts(displayChallenge.c_str());
            uart.puts("\n\r");
            result.push_back(challengeObject);
        }
    }
    uart.puts("\r\n");
    return result;
}

Player_t getColor() {
    jsonStrings.clear();
    uart_esp.puts(std::string("GET:https://lichess.org/api/board/game/stream/"+gameID+"\n").c_str());
    task::sleep(1000);
    while (jsonStrings.empty()) {
        task::sleep(500);
        uart.puts(".");
    }
    uart.puts("Got response\r\n");
    json j = json::parse(jsonStrings[0]);
    uart.puts(std::string("Made json object from: "+ jsonStrings[0] + "\n\r").c_str());
    jsonStrings.clear();
    bool isWhite = std::string(j["white"]["name"]) == username;
    uart.puts("Got color\r\n");
    if (isWhite) {
        return PLAYER_WHITE;
    } else {
        return PLAYER_BLACK;
    }
}

std::vector<std::string> splitString(const std::string& str, const char delimiter) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string token;

    while (std::getline(iss, token, delimiter)) {
        if (!token.empty()) {
            result.push_back(token);
        }
    }

    return result;
}

void makeEnemyMove(uGUI gui, std::string enemyMove) {
    // it just works, source: trust me bro
    uint8_t x1, x2, y1, y2 = 0;
    x1 = enemyMove.at(0) - 'a';
    y1 = enemyMove.at(1) - '1';
    x2 = enemyMove.at(2) - 'a';
    y2 = enemyMove.at(3) - '1';
    // transform coords when player black
    if (player == PLAYER_WHITE) {
        y1 = 7 - y1;
        y2 = 7 - y2;
    }
    ChessPiece_t startPiece = board[x1][y1];
    ChessPiece_t endPiece = startPiece;
    // check if pawn promotion
    if (enemyMove.length() == 5) {
        switch (enemyMove.at(4)) {
            case 'q':
                endPiece = player == PLAYER_BLACK ? QUEEN_BLACK : QUEEN_WHITE;
                break;
            case 'n':
                endPiece = player == PLAYER_BLACK ? KNIGHT_BLACK : KNIGHT_WHITE;
                break;
            case 'b':
                endPiece = player == PLAYER_BLACK ? BISHOP_BLACK : BISHOP_WHITE;
                break;
            case 'r':
                endPiece = player == PLAYER_BLACK ? ROOK_BLACK : ROOK_WHITE;
                break;
        }

    }
    // check if castling
    if ((startPiece == KING_WHITE || startPiece == KNIGHT_BLACK) && abs(x1 - x2) > 1) {
        // "links" lang
        ChessPiece_t rook = player == PLAYER_WHITE ? ROOK_BLACK : ROOK_WHITE;
        drawSquare(gui, x1, y1);
        uint8_t rookXOld = x2 == 6 ? 7 : 0;
        drawSquare(gui, rookXOld, y2);
        uint8_t rookX = x2 - x1 < 0 ? x2+1 : x2-1;
        drawPiece(gui, x2, y2, endPiece);
        drawPiece(gui, rookX, y2, rook);
        board[rookX][y2] = rook;
        board[x2][y2] = endPiece;
        board[x1][y1] = NO_PIECE;
        board[rookXOld][y1] = NO_PIECE;
        // board[]
    } else {
        board[x2][y2] = endPiece;
        board[x1][y1] = NO_PIECE;
        drawSquare(gui, x1, y1);
        drawSquare(gui, x2, y2);
        drawPiece(gui, x2, y2, endPiece);
    }
    drawBoard(gui);
    for (uint8_t x = 0; x < 8; x++) {
        for (uint8_t y = 0; y < 8; y++) {
            drawPiece(gui, x, y, board[x][y]);
        }
    }
}

void waitForEnemyMove(uGUI gui) {
    std::string enemyMove;
    while (true) {
        uart.puts("Waiting for enemy turn\n\r");
        jsonStrings.clear();
        std::string req_str = "GET:https://lichess.org/api/board/game/stream/" + gameID + "\n";
        uart_esp.puts(req_str.c_str());
        String menuName = "";
        for (uint8_t i = 0; i < 120; i++) {
            if(!jsonStrings.empty() && i > 5)
                break;
            menu->RefreshLoadMenu(menuName);
            task::sleep(500);
            uart.puts(".");
        } // Wait for all responses
        bool expectedMoveCountEven = player == PLAYER_WHITE;
        if (!jsonStrings.empty()) { // Check if enemy moved
            uart.puts("1");
            json j = json::parse(jsonStrings[0]);
            uart.puts("2");
            if (j.contains("state") && j["state"].contains("moves")) {
                uart.puts("3");
                std::string moves = j["state"]["moves"];
                uart.puts("4");
                auto moveList = splitString(moves, ' ');
                uart.puts("5");
                if (moveList.empty()) {
                    uart.puts("No moves yet");
                    continue;
                }
                enemyMove = moveList.back();
                uart.puts("6");
                bool moveCountEven = moveList.size() % 2 == 0;
                uart.puts("7\n\r");
                if (moveCountEven == expectedMoveCountEven) {
                    uart.puts("Enemy has moved!\n\r");
                    break;
                } else {
                    uart.puts("Enemy hasn't moved yet!\n\r");
                }
            }
        }
    }
    makeEnemyMove(gui, enemyMove);
}




int main(){
    posix_io::inst.register_stdout(uart);
    posix_io::inst.register_stderr(uart);


    // Setup SPI interface
    gpio_rp2040_pin lcd_cs (5);
    spi_rp2040  spi(0, 4, 7, 22, lcd_cs);
    spi.setSpeed(30000000);

    // Setup LCD driver
    gpio_rp2040_pin lcd_rst(9);
    gpio_rp2040_pin lcd_dc (2);
    st7735s_drv lcd(spi, lcd_rst, lcd_dc, st7735s_drv::Crystalfontz_128x128);

    // Setup uGUI
    uGUI gui(lcd);

    lcd.clearScreen(0x0);

    adc_rp2040_channel joy_X(1);
    uint16_t offsetX = joy_X.adcReadRaw();
    adc_rp2040_channel joy_Y(3);
    uint16_t offsetY = joy_Y.adcReadRaw();

    gpio_rp2040_pin s1(16);
    gpio_rp2040_pin s2(17);

    for (uint8_t i=0;i<128;i++){
        for (uint8_t j=0;j<128;j++){
            for (uint8_t k=0;k<3;k++){
                bool isWhite = (i/16 +j/16)%2 == 0;
                if (isWhite){
                    board_pixels[i][j][k] =colors[0][k];
                } else{
                    board_pixels[i][j][k] =colors[1][k];
                }

            }
        }
    }
    uart.puts("Hello World\n\r");

    uart_esp.uartAttachIrq([&](char c) {
        if (c == '\n') {
            uart.puts("I received a message from ESP8266: ");
            uart.puts(msg.c_str());
            uart.puts("\r\n");
            if (!msg.empty() && msg.c_str()[0] == '{') {
                jsonStrings.push_back(msg.c_str());
                responseCount++;
            }
            msg.clear();
        } else {
            msg += c;
        }
    });


    menu = new Menu(gui, joy_Y, offsetY, s1, s2);
    String menuName;
    std::vector<std::string> choices;
    int8_t choice;
    while (true) {
        while (true) {
            menuName = "Main Menu!";
            choices = {"Make Challenge", "Accept Challenge"};
            choice = menu->GetMenuChoice(menuName, choices, false);
            if (choice == 0) {
                choices = getFriends();
                menuName = "Challenge...";
                choice = menu->GetMenuChoice(menuName, choices, true);
                uart.puts(".");
                if (choice == -1) // Resume to main menu
                    continue;
                if (!createChallengeBlocking(choices[choice].c_str(), 60000))
                    continue; // Resume to main menu because challenge was not accepted
                uart.puts(std::string("GameID: " + gameID + "\n\r").c_str());
                break;
            } else {
                choices.clear();
                menuName = "Accept...";
                auto challenges = getChallenges();
                choices.reserve(challenges.size());
                for (auto & challenge : challenges) {
                    choices.push_back(challenge[2]);
                }
                if (challenges.empty()) {
                    std::string noChallenges = "No Challenges";
                    choices.push_back(noChallenges);
                }
                choice = menu->GetMenuChoice(menuName, choices, true);
                if (choice == -1) // Resume to main menu
                    continue;
                if (!challenges.empty()) { // Only continue to game when challenge accepted
                    gameID = challenges[choice][1];
                    jsonStrings.clear();
                    uart_esp.puts(std::string("POST:https://lichess.org/api/challenge/"+gameID+"/accept\n").c_str());
                    while (jsonStrings.empty())
                        task::sleep(100); // Wait for successful challenge accept
                    jsonStrings.clear();
                    uart.puts(std::string("GameID: " + gameID + "\n\r").c_str());
                    break;
                }
            }
        }
        uart.puts("\n\r"+menuName+" -> "+choices[choice].c_str()+"\n\r");
        hasMoved = false;
        player = getColor();
        myTurn = player == PLAYER_WHITE;
        for (uint8_t i = 0; i < 10; i++) {
            menu->RefreshLoadMenu(menuName);
            task::sleep(500);
            uart.puts(".");
        } // Wait for all responses
        jsonStrings.clear();

        drawBoard(gui);
        initBoard(gui);

        uint8_t cursorX = 0;
        uint8_t cursorY = 0;
        bool selectChanged = false;
        focusSquare(gui, cursorX, cursorY);

        uart.puts("Starting chess game...\n\r");

        while (true) {
            if (!myTurn) {
                waitForEnemyMove(gui);
                myTurn = true;
            };

            task::sleep(20);

            if(pieceSelected && selectChanged) {
                selectChanged = false;
                selectSquare(gui, selectedSquare[0], selectedSquare[1]);
            }

            // read in a new ADC value
            uint16_t valX = joy_X.adcReadRaw();
            uint16_t valY = joy_Y.adcReadRaw();

            if ((valX - offsetX) < -1000 && cursorX > 0) {
                cursorX--;
                task::sleep(20);
                focusSquare(gui, cursorX, cursorY);
                uint8_t buttonPassedTime = 0;
                while ((joy_X.adcReadRaw() - offsetX) < -1000 && buttonPassedTime < BUTTON_DELAY) {
                    task::sleep(1);
                    buttonPassedTime++;
                }
                task::sleep(10);
            }
            if ((valX - offsetX) > 1000 && cursorX < 7) {
                cursorX++;
                task::sleep(20);
                focusSquare(gui, cursorX, cursorY);
                uint8_t buttonPassedTime = 0;
                while ((joy_X.adcReadRaw() - offsetX) > 1000 && buttonPassedTime < BUTTON_DELAY) {
                    task::sleep(1);
                    buttonPassedTime++;
                }
                task::sleep(10);
            }
            if ((valY - offsetY) < -1000 && cursorY < 7) {
                cursorY++;
                task::sleep(20);
                focusSquare(gui, cursorX, cursorY);
                uint8_t buttonPassedTime = 0;
                while ((joy_Y.adcReadRaw() - offsetY) < -1000 && buttonPassedTime < BUTTON_DELAY) {
                    task::sleep(1);
                    buttonPassedTime++;
                }
                task::sleep(10);
            }
            if ((valY - offsetY) > 1000 && cursorY > 0) {
                cursorY--;
                task::sleep(20);
                focusSquare(gui, cursorX, cursorY);
                uint8_t buttonPassedTime = 0;
                while ((joy_Y.adcReadRaw() - offsetY) > 1000 && buttonPassedTime < BUTTON_DELAY) {
                    task::sleep(1);
                    buttonPassedTime++;
                }
                task::sleep(10);
            }
            if (!s1.gpioRead()) {
                task::sleep(20);
                while (!s1.gpioRead());
                task::sleep(10);
                menuName = "Game Paused!";
                choices = {"Resume", "Abort/Resign"};
                choice = menu->GetMenuChoice(menuName, choices, true);
                if (choice == 1) {
                    if (!hasMoved) {
                        uart.puts("Abort game\n\r\n\n");
                        uart_esp.puts(std::string("https://lichess.org/api/board/game/"+gameID+"/abort\n").c_str());
                    } else {
                        uart.puts("Resign game\n\r\n\n");
                        uart_esp.puts(std::string("https://lichess.org/api/board/game/"+gameID+"/resign\n").c_str());
                    }
                    break;
                } else {
                    drawBoard(gui);
                    for (uint8_t x = 0; x < 8; x++) {
                        for (uint8_t y = 0; y < 8; y++) {
                            drawPiece(gui, x, y, board[x][y]);
                        }
                    }
                }
            }
            if (!s2.gpioRead()) {
                selectChanged = true;
                if(pieceSelected) {
                    if(selectedSquare[0] == cursorX && selectedSquare[1] == cursorY) {
                        // deselect
                        pieceSelected = false;
                        drawSquare(gui, cursorX, cursorY);
                        drawPiece(gui, cursorX, cursorY, board[cursorX][cursorY]);
                    } else {
                        // make move
                        movePiece(gui, selectedSquare[0], selectedSquare[1], cursorX, cursorY);
                        focusSquare(gui, cursorX, cursorY);
                        pieceSelected = false;
                    }
                } else {
                    if(board[cursorX][cursorY] != NO_PIECE) {
                        pieceSelected = true;
                        selectedSquare[0] = cursorX;
                        selectedSquare[1] = cursorY;
                    }
                }
                task::sleep(20);
                while (!s2.gpioRead());
                task::sleep(10);
            }
        }
    }
}
