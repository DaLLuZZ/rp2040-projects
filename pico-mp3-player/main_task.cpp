#include "main_task.h"

main_task::main_task()
        : task("Main", MAIN_STACKSIZE),
          _cs(SD_CS_PIN),
          _spi(SD_SPI0, SD_MISO_PIN, SD_MOSI_PIN, SD_SCLK_PIN, _cs),
          _sd(_spi),
          _next(NEXT_BUTTON),
          _part(PART_BUTTON),
          _ps(PICO_PS),
          _fs(_sd) {
    _next.gpioMode(GPIO::INPUT | GPIO::PULLUP);
    _part.gpioMode(GPIO::INPUT | GPIO::PULLUP);
    _ps.gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);
}

void main_task::run() {
    pcm_pwm_rp2040_drv pcm_drv(AUDIO_LEFT, AUDIO_RIGHT);
    sd_reader_task sd_reader;
    mp3_decoder_task decoder(pcm_drv, sd_reader);

    // Mount the SD card. If the part button is pressed
    // during reset, partition 2 wil be used. Otherwise,
    // the auto partition mode is used.
    FatFs::FRESULT res = _fs.mount(_part == 0 ? 2 : 0);

    // Find first MP3 file
    assert(res == FatFs::FR_OK);
    res = _fs.findfirst(&_dir, &_finfo, "", "*.mp3");

    // Loop over all MP3 files
    while (res == FatFs::FR_OK && _finfo.fname[0]) {
        // Open the MP3 file
        res = _fs.open(&_file, _finfo.fname, FA_OPEN_EXISTING | FA_READ);
        assert(res == FatFs::FR_OK);

        // Start the SD reader and decoder tasks
        // to play the song :)
        sd_reader.start(&_fs, &_file);
        decoder.start();

        // Wait until file has been played. Check
        // for NEXT-button to skip to next file
        while (sd_reader.isAlive() || decoder.isAlive()) {
            if (_next.gpioRead() == LOW) {
                // Stop playing the current title
                sd_reader.force_eof();
            }
            sleep(200);
        }
        // Wait until button is released, so we only
        // skip _one_ song...
        while (_next.gpioRead() == LOW) sleep(100);

        // Close the MP3 file and find next one
        _fs.close(&_file);
        res = _fs.findnext(&_dir, &_finfo);
    }
    // Unmount the SD card
    _fs.umount();
}
