///////////////////////////////////////////////////
// MP3 player for RPi Pico and GPIO audio output //
///////////////////////////////////////////////////
//
// This program will read MP3 files from a SD card and
// play them on the RP2040 launchpad. The MP3 decoder is
// implemented in software, so only some kind of PCM
// output is necessary, which is provided by a driver
// which outputs the PCM data as PWM GPIO signals.
//
// The main task will loop over all *.mp3 files and play
// them. The next-button can be used to skip to the next
// song. The paart-button is used to select the partition
// of the SD card during startup.
//
// For every new song, the SD reader and MP3 decoder
// tasks are started again.
//
// Make sure you connect the SD-card PCB to the correct
// SPI interface (see pin assignments in config.h), and
// provide some MP3 files without too high quality
// (128 kbps works fine!)

#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include "config.h"
#include "gpio_rp2040.h"
#include "spi_rp2040.h"
#include "sd_spi_drv.h"
#include "pcm_pwm_rp2040_drv.h"
#include "task.h"
#include "sd_reader_task.h"
#include "mp3_decoder_task.h"
#include "ff.h"
#include <cassert>

class main_task : public task
{
public:
    main_task();
    void run() override;

private:
    gpio_rp2040_pin _cs;    // CS Line of SD card SPI interface
    spi_rp2040      _spi;   // SPI interface used for the SD card reader
    sd_spi_drv      _sd;    // SD card low level driver

    gpio_rp2040_pin _next;  // Next button
    gpio_rp2040_pin _part;  // Partition button
    gpio_rp2040_pin _ps;    // Pico Power save pin

    FatFs           _fs;
    FatFs::DIR      _dir{};
    FatFs::FILEINFO _finfo{};
    FatFs::FILE     _file{};  // MP3 file on SD
};

#endif // MAIN_TASK_H
