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
///////////////////////////////////////////
// Play a single MP3 file from flash memory
///////////////////////////////////////////
//
// main() only prepares the infrastructure, namely:
//  - redirects stdout/stderr
//  - starts the main task
//  - starts the task monitor
//  - starts the scheduler
// The program logic is contained in the main_task
// and not here!

#include "uart_rp2040.h"
#include "posix_io.h"
#include "task.h"
#include "task_monitor.h"
#include "Model/model.h"
#include "UI.h"
#include "gpio_rp2040.h"

int main() {
    // Redirect stdout to our backchannel UART, so
    // we can see the output of the task monitor
    uart_rp2040 uart;
    posix_io::inst.register_stdout(uart);
    posix_io::inst.register_stderr(uart);
    Model model{};

    LCD_Setup LS;
    ssd2119_drv lcd(LS._spi, LS._lcd_rst, LS._lcd_dc, ssd2119_drv::Kentec_K350QVG);

    printf("Setup complete\n");
    fflush(stdout);

    //initial Samples
    Sample *samples = model.getSamples();
    samples[0] = Sample(Song::ACHIEVEMENT_WIN_DRUM);
    samples[1] = Sample(Song::ANGELIC_DRUM);
    samples[2] = Sample(Song::BAD_JOKE_DRUM);
    samples[3] = Sample(Song::BASS_FUTURISTIC);

    //configure Player Task so UI has Access to it
    task Player([&model]() {
        printf("Start playing\n");
        fflush(stdout);
        while (true) {
            model.play();
        }
    }, "Player Task");

    // Start UITask task as privileged task
    task UITask([&Player, &model, &lcd]() {

        Touch touch = UI::setUpTouch(lcd);
        UI ui(&model, lcd, touch);
        ui.selectInstrumentUI(&model);
        ui.buildUI();

        bool rebuild = false;
        while (true) {
            rebuild = ui.runUI(&Player);
            if(rebuild) {
                ui.buildUI();
            }
            task::sleep(10);
        }

    }, "UITask task", 5000);
    UITask.start(150, true);



    // Start the Task monitor,
    task_monitor monitor;
    monitor.start();

    // Start the multitasking
    task::start_scheduler();

}

