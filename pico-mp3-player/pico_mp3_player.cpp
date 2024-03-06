/////////////////////////////
// MP3 player for RPi Pico //
/////////////////////////////
//
// main() only starts the multitasking and
// runs the main task (see main_task.h/cpp)
//
#include "main_task.h"

int main()
{
    // Start Main task as privileged task
    main_task Main;
    Main.start(MAIN_PRIORITY, true);
    // Start the multitasking
    task::start_scheduler();
}
