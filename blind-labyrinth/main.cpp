#include "string"
#include <posix_io.h>
#include "uart_rp2040.h"
#include <cmath>
#include "gameMaster.h"
#include "SDTools/sdWriter.h"
#include "SDTools/sdreader.h"


int main(){


    uart_rp2040 uart;
    posix_io::inst.register_stdin(uart);
    posix_io::inst.register_stdout(uart);
    posix_io::inst.register_stderr(uart);





    auto gm = new GameMaster();
    while(true){
        gm->startPhaseTitle();
        gm->startPhaseMap();
        gm->startPhaseMode();
        gm->startPhaseLabyrinth();
        gm->startPhaseEnd();
    }




}
