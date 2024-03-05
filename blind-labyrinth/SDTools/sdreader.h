#ifndef SDREADER_H
#define SDREADER_H

#include <sstream>
#include <vector>
#include <cassert>
#include <uart_rp2040.h>

#include "sdreader.h"

#include "ff.h"
#include "gpio_rp2040.h"
#include "posix_io.h"
#include "sd_spi_drv.h"
#include "spi_rp2040.h"

#define MISO_PIN  8
#define MOSI_PIN 11
#define SCLK_PIN 10
#define CS_PIN    3
#define SPI1      1

/// @brief A helper class for reading data from a SD card consisting of static functions.
class SDReader {
public:

    /// @brief Reads the names of the Labyrinth maps from the SD card.

    static std::vector<std::string> ReadLabNamesFromSD() {


        gpio_rp2040_pin cs(CS_PIN);
        spi_rp2040 spi(SPI1, MISO_PIN, MOSI_PIN, SCLK_PIN, cs);
        sd_spi_drv sd(spi);
        FatFs fs(sd);

        posix_io::inst.register_fileio(fs);

        FatFs::DIR dir;

        if(fs.mount() != FatFs::FR_OK){
            return {};
        }
        FatFs::FILEINFO fileInfo;

        std::vector<std::string> result;

        if(fs.findfirst(&dir, &fileInfo, "/maps", "*.csv") != FatFs::FR_OK){
            return {};
        }
        do {
            if (fileInfo.fname[0] == '\0') {
                break;
            }
            if (fileInfo.fattrib & AM_ARC) {
                result.emplace_back(fileInfo.fname);
            }
        } while (fs.findnext(&dir, &fileInfo) == FatFs::FR_OK);
        fs.closedir(&dir);

        assert(fs.umount() == FatFs::FR_OK);

        return result;
    }

    /// @brief reads the map data with the specified name from the sd card including the highscore
    /// @param fileName the name of the file (e.g. "file.cvs")
    /// @return 1D vector with one row per element and the highscore as the last element
    static std::vector<std::string> ReadMapsFromSD(const std::string &fileName) {

        gpio_rp2040_pin cs(CS_PIN);     // CS Line of SPI interface
        spi_rp2040 spi(SPI1, MISO_PIN, MOSI_PIN, SCLK_PIN, cs);
        sd_spi_drv sd(spi);        // SD card low level driver
        FatFs fs(sd);         // FatFs driver

        posix_io::inst.register_fileio(fs);

        assert(fs.mount() == FatFs::FR_OK);
        std::string path = "/maps/" + fileName;

        FILE *f = fopen(path.c_str(), "r+");
        assert(f != NULL);

        std::vector<std::string> result;

        char buf[100], *s;
        do {
            s = fgets(buf, 100, f);
            printf(s);
            result.emplace_back(s);
        } while(*s != '#');


        fclose(f);

        assert(fs.umount() == FatFs::FR_OK);

        printf("%d", result.size());
        result.pop_back();
        return result;
    }
};

#endif // SDREADER_H