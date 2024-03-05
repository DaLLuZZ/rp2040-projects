//
// Created by maurr on 28.02.2024.
//

#ifndef LABYRINTH_SDWRITER_H
#define LABYRINTH_SDWRITER_H


#include "ff.h"
#include "gpio_rp2040.h"
#include "posix_io.h"
#include "sd_spi_drv.h"
#include "spi_rp2040.h"
#include "uart_rp2040.h"
#include "yahal_String.h"

#include <cstdio>
#include <cassert>
#include <cstring>

// Hardware configuration.
#define MISO_PIN  8
#define MOSI_PIN 11
#define SCLK_PIN 10
#define CS_PIN    3
#define SPI1      1

/// @brief A helper class for writing data to a SD card consisting of static functions.
class SDWriter {
public:
    /// @brief Used to write the labyrinth data on the sd card as 16 rows of 16x int,int,.. followed by a row consisting of the highscore, and a row with "#"" to mark the end of the document
    /// @param fileName the name of the file to be written
    /// @param labData the labyrinthmap in storage format
    /// @param highscore the highscore to be saved
    /// @return 0 if write was successful
    static int WriteLab(const std::string &fileName, const std::vector<int8_t> &labData, uint16_t highscore = 42) {


        gpio_rp2040_pin cs(CS_PIN);
        spi_rp2040 spi(SPI1, MISO_PIN, MOSI_PIN, SCLK_PIN, cs);
        sd_spi_drv sd(spi);
        FatFs fs(sd);

        posix_io::inst.register_fileio(fs);

        assert(fs.mount() == FatFs::FR_OK);
        std::string s = "/maps/" + fileName;
        // Open file for reading and writing
        FILE *f = fopen(s.c_str(), "w+");
        assert(f != NULL);

        for (int i = 0; i < labData.size(); ++i) {
            if(i == 0){
                fprintf(f, "%d", labData[i]);
            }else if(i % 16 == 0){
                fprintf(f,"%s%d","\n", labData[i]);
            }else {
                fprintf(f, "%s%d", ",", labData[i]);
            }

        }
        fprintf(f, "\n");
        fprintf(f, "%d", highscore);
        fprintf(f, "\n#");


        fclose(f);

        assert(fs.umount() == FatFs::FR_OK);

        return 0;
    }
};

#endif //LABYRINTH_SDWRITER_H
