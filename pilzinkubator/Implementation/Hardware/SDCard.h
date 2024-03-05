//
// Created by steffen on 19.01.24.
//

#ifndef IMPLEMENTATION_SDCARD_H
#define IMPLEMENTATION_SDCARD_H

#include "ff.h"
#include "gpio_rp2040.h"
#include "posix_io.h"
#include "sd_spi_drv.h"
#include "spi_rp2040.h"

#include "yahal_String.h"

#include <cassert>
#include <cstdio>

// Hardware configuration.
#define MISO_PIN  8
#define MOSI_PIN 11
#define SCLK_PIN 10
#define CS_PIN    3
#define SPI1      1

class SDCard {
    gpio_rp2040_pin cs;     // CS Line of SPI interface
    spi_rp2040      spi;
    sd_spi_drv*     sd = nullptr;     // SD card low level driver

    FatFs* fs = nullptr;// FatFs driver

    bool mounted = false;

    void initFS() {
        delete fs;
        delete sd;
        sd = new sd_spi_drv(spi);
        fs = new FatFs(*sd);
    }
public:
    SDCard() : cs(CS_PIN), spi(SPI1, MISO_PIN, MOSI_PIN, SCLK_PIN, cs) {
        //initFS();
    }

    ~SDCard() {
        delete fs;
        delete sd;
    }

    bool mount();
    bool unmount();

    bool hasSDCard();
    bool hasFile(const char* filename);

    String readFile(const char* filename);
    bool writeFile(const char* filename, String text, bool append);
};


#endif //IMPLEMENTATION_SDCARD_H
