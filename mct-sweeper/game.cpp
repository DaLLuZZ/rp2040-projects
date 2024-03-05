//
// Created by tobia on 10.01.2024.
//

#include "gpio_rp2040.h"
#include "spi_rp2040.h"
#include "ssd2119_drv.h"
#include "timer_rp2040.h"
#include "uGUI.h"
#include "uGUI_colors.h"
#include "task.h"

#include <cstdlib>
#include <cstdio>

#include "uart_rp2040.h"
#include "posix_io.h"

// Custom Imports
#include "gui/GameGUI.h"
#include "Enums.h"

/**
 * Main method of the game. Method will initialize a instance of the class GameGUI and start the gameloop.
 * Inside the gameloop the program will wait for a new user input and pass the touch coordinates to the game's UI.
 * @return
 */
int main(){
    uart_rp2040 uart;
    posix_io::inst.register_stdout(uart);
    posix_io::inst.register_stderr(uart);
    printf("----------- START PROGRAM -----------\n");

    GameGUI interface;
    GameState state = menu;
    interface.draw(state);
    printf("DEBUG   STARTING GAMELOOP\n");
    while(true){
        if (state == stop)
            break;
        int x, y;
        interface.waitForTouch(x,y);
        if (!interface.processTouch(x,y)){
            switch (state) {
                case menu:
                    state = game;
                    break;
                case game:
                    state = end;
                    break;
                case end:
                    state = stop;
                    break;
            }
            interface.draw(state);
        }
    }
}
