#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <cstdint>
#include <string>
#include "boostxl_eduMKII.h"

#include "st7735s_drv.h"
#include "pixel_stream_rgb565.h"
#include "Tiles/tile.h"
#include "visual_helpers/charpattern.h"
#include "visual_helpers/visual_items.h"
#include "visual_helpers/pixel_stream_rgb565_const.h"
#include "timer_rp2040.h"

class Visualizer{
private:

    timer_rp2040 timer = timer_rp2040(0);
    bool toggle = false;
    static st7735s_drv create_lcd() {

        // Inject dependencies
        boostxl_eduMKII edu;
        edu.need_led_rgb = true;
        edu.need_lcd     = true;
        edu.inject();

        // Switch on backlight
        edu.led_red().gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);
        // Setup SPI interface
        edu.lcd_spi().setSpeed(30000000);

        // Setup LCD driver
        st7735s_drv lcd(edu.lcd_spi(), edu.lcd_rst(), edu.lcd_dc(),
                        st7735s_drv::Crystalfontz_128x128);

        lcd.clearScreen(0x000000);
        return lcd;
    }

    /// @brief toggles the normal or mirrored image of the torch
    void toggleTorches(){
        if(toggle){
            toggle = false;
            displayTorch(4, 92);
            displayMirroredTorch(117, 92);
        }else{
            toggle = true;
            displayMirroredTorch(4, 92);
            displayTorch(117, 92);
        }
    }

    /// @brief Starts an animation for torches (used in start- and endscreen)
    void startTorchAnimation() {
        timer.setPeriod(100000, TIMER::PERIODIC);
        timer.setCallback([&]() { toggleTorches(); });
        displayMirroredTorch(4, 92);
        displayTorch(117, 92);
        timer.start();
    }
    /// @brief displays an image of a torch at the given coordinates
    /// @param x x-coords 
    /// @param y y-coords
    void displayTorch(int x, int y){
        auto stream = getTorch();
        uint16_t *const data = stream.data();
        size_t size = stream.size();
        auto ps = pixel_stream_rgb565(data, size);
        screen.drawArea(x, y, x + 7, y +31, ps);
    }

    /// @brief displays a mirrored image of a torch at the given coordinates
    /// @param x x-coords
    /// @param y y-coords
    void displayMirroredTorch(int x, int y){
        auto stream = getMirroredTorch();
        uint16_t *const data = stream.data();
        size_t size = stream.size();
        auto ps = pixel_stream_rgb565(data, size);
        screen.drawArea(x, y, x + 7, y +31, ps);
    }


public:
    st7735s_drv screen;
    Visualizer() : screen(create_lcd()) {

    }

    /// @brief stops the timer for the animation
    void stopAnimation(){
        timer.setCallback([&]() {  });
        timer.stop();
    }
    /// @brief visualizes an area at the given coordinates and the specific color
    /// @param x_min x-coord start of area
    /// @param y_min y-coord start of area
    /// @param x_max x-coord end of area
    /// @param y_max y-coord end of area
    /// @param color color to display
    void visualizeMe(int8_t x_min, int8_t y_min, int8_t x_max, int8_t y_max, color_t color){
        pixel_stream_rgb565_const ps = pixel_stream_rgb565_const(color);
        screen.drawArea(x_min, y_min, x_max, y_max, ps);
    }

    /// Displays a char array. Automaticly maths the x2 and y2 to fit the scaling. Normal size /letter is 5x5
    /// \param text The const char* to be displayed
    /// \param text_color RGB565 of the text color
    /// \param background RGB565 of the background color
    /// \param scaling the scaling of the text
    /// \param x start of display area
    /// \param y start of display area
    void displayText(const char* text, uint16_t text_color, uint16_t background, int scaling, int x, int y){
        const auto stream = createColorStream(text, text_color, background, scaling);
        size_t size_of_stream = stream.size();
        auto ps = pixel_stream_rgb565(stream.data(), size_of_stream);
        uint16_t height = STD_CHARMESHSIZE * scaling;
        uint16_t width = size_of_stream / height;
        screen.drawArea(x, y, x + width-1, y + height -1, ps);
    }
    /// @brief Displays the title screen of the game
    void displayTitleScreen(){
        screen.clearScreen(0x0);

        displayText("Blind", 0x2124, 0x0, 4, 0, 8);
        displayText("Labyrinth", 0x0180, 0x0, 2, 6, 36);
        displayText("Press Button", 0xFFFF, 0x0, 1, 24, 68);
        displayText("to enter", 0xFFFF, 0x0, 1, 36, 78);

        startTorchAnimation();
    }

    /// @brief displays the different names of the labyrinths on the SD-Card
    /// @param labNames names of the labyrinths
    void displayChooseLab(const std::vector<std::string>& labNames){
        screen.clearScreen(0x0);
        displayText("Pick Map", 0xFFF, 0x0, 2, 8, 0);
        for(int i = 0; i < labNames.size() && i < 14; ++i){
            size_t pos = labNames[i].find_last_of('.');
            displayText(labNames[i].substr(0, pos).c_str(), 0xFFF, 0x0, 1, 8, 16 + i*8);
        }
    }

    /// @brief displays the different names of the labyrinths on the SD-Card
    /// @param diffNames names of the labyrinths
    void displayChooseModes(const std::vector<std::string>& diffNames){
        screen.clearScreen(0x0);
        displayText("Pick Mode", 0xFFF, 0x0, 2, 8, 0);
        for(int i = 0; i < diffNames.size() && i < 14; ++i){
            displayText(diffNames[i].c_str(), 0xFFF, 0x0, 1, 8, 16 + i*8);
        }
    }

    /// @brief displays and moves the pointer, which indicates the currently selected labyrinth
    /// @param index index to move the pointer to
    /// @param last_index index where pointer was
    void displayChoosePointer(int index, int last_index){
        if(index > 14){
            return;
        }
        auto ps_black = pixel_stream_rgb565_const(0x0);
        auto ps_red = pixel_stream_rgb565_const(0xF800);
        if(last_index >= 0){
            screen.drawArea(2, 18 + 8*last_index, 6, 22 + 8*last_index, ps_black);
        }
        screen.drawArea(2, 18 + 8*index, 6, 22 + 8*index, ps_red);

    }

    void displayLoadingScreen(){
        screen.clearScreen(0x0);
        displayText("Loading...", 0xFFFF, 0x0, 2, 0, 0);
    }

    /// @brief displays the end screen
    /// @param stats stats of the player (steps, time, score)
    void displayEndScreen(std::vector<int> stats, int highscore, std::string mapName){
        screen.clearScreen(0x0);

        displayText("Finish", 0x2124, 0x0, 3, 4, 6);

        displayText((mapName.substr(0, mapName.find_last_of('.'))).c_str(), 0xFFFF, 0x0, 1, 28, 30);

        displayText("Steps", 0xFFFF, 0x0, 1, 28, 42);
        displayText((std::to_string(stats[0])).c_str(), 0xFFFF, 0x0, 1, 70, 42);

        displayText(("Time"), 0xFFFF, 0x0, 1, 28, 52);
        displayText((std::to_string(stats[1]) + " ms").c_str(), 0xFFFF, 0x0, 1, 70, 52);



        displayText("Score", 0xFFFF, 0x0, 1, 28, 62);
        displayText((std::to_string(stats[2])).c_str(), 0xFFFF, 0x0, 1, 70, 62);

        displayText("Press Button", 0xF0F0, 0x0, 1, 24, 74);
        displayText("to continue", 0xF0F0, 0x0, 1, 26, 82);


        displayText(("Highscore"), 0x0180, 0x0, 1, 32, 94);
        displayText((std::to_string(highscore).c_str()), 0xFFFF, 0x0, 2, 34, 106);

        startTorchAnimation();
    }

    void displayBlankScreen(){
        screen.clearScreen(0x0);
    }

    void displayErrorScreen(const char * errorMessage){
        screen.clearScreen(0x0);
        displayText(errorMessage, 0xFFFF, 0x0, 1, 0, 0);
    }


};


#endif // VISUALIZER_H