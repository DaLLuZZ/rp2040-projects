#ifndef CONFIG_H
#define CONFIG_H

// GPIOs for SPI interface (SD card)
#define SD_SPI0         0
#define SD_SCLK_PIN     2
#define SD_MOSI_PIN     3
#define SD_MISO_PIN     4
#define SD_CS_PIN       5

// GPIOs for audio output
#define AUDIO_LEFT     16
#define AUDIO_RIGHT    17

// GPIOs for buttons
#define NEXT_BUTTON    10
#define PART_BUTTON    15

// Pico-internal GPIOs
#define PICO_PS        23
#define LED_PIN        25

// Task stack sizes and priorities
#define MAIN_STACKSIZE          6000
#define MP3_DECODER_STACKSIZE   8200
#define SD_READER_STACKSIZE      500
#define MAIN_PRIORITY             50

// Buffer sizes
#define MP3_FRAME_SIZE 2881
#define MP3_BUF_SIZE   4096

#endif // CONFIG_H
