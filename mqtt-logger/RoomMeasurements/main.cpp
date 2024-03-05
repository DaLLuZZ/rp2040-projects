#include <posix_io.h>
#include <font_8x12.h>
#include <spi_rp2040.h>
#include <st7735s_drv.h>
#include <uart_rp2040.h>
#include "Room.h"
#include "yahal_String.h"
#include "task_monitor.h"


void initializeUart(uart_rp2040 &uart);

void initializeDisplay(gpio_rp2040_pin &lcdBacklight, spi_rp2040 &spi, st7735s_drv &lcd, uGUI &gui);

void enforceCorrectBoot();

int main() {
    Room room{};

    // backchannel uart for the rp2040
    uart_rp2040 uart;

    initializeUart(uart);

    // UART which is connected to the ESP8266
    uart_rp2040 uart_esp{1, 20, 21, 115200};

    // LCD background led
    gpio_rp2040_pin lcdBacklight{13};

    // SPI interface
    gpio_rp2040_pin lcd_cs{5};
    spi_rp2040 spi{0, 4, 7, 22, lcd_cs};

    // LCD driver
    gpio_rp2040_pin lcd_rst{9};
    gpio_rp2040_pin lcd_dc{2};
    st7735s_drv lcd{spi, lcd_rst, lcd_dc, st7735s_drv::Crystalfontz_128x128};

    // uGUI
    uGUI gui(lcd);

    initializeDisplay(lcdBacklight, spi, lcd, gui);

    enforceCorrectBoot();

    // Receive message handler: All received chars are collected in a String. When '\n' is received, the whole message is printed out.
    String msg;
    uart_esp.uartAttachIrq([&msg](char c) {
        if (c == '\n' &&  !msg.empty()) {
            printf("Received a message from ESP8266: %s\n", msg.c_str());
            msg.clear();
        } else {
            // Add char to message
            msg += c;
        }
    });

    // Wait for ESP8266 to connect to MQTT
    task::sleep(5000);

    // Endless loop which sends the measurement values to the ESP8266.
    // The messages that are send back from ESP8266 are printed in the receive message handler above!
    while (true) {
        task::sleep(200);

        // get the temperature from the sensor, convert it to a string and print it on the LCD
        String temperature = doubleToString(room.getTemperature(), 2) + " °C";
        gui.PutString(35, 40, temperature + "  ");

        // send a message with the temperature to the ESP8266 and print the message on backchannel uart
        String temperatureMsg = "Temperature: " + temperature + "\n";
        printf("%s", ("\nSending message to ESP8266: " + temperatureMsg).c_str());
        uart_esp.puts(temperatureMsg.c_str());

        task::sleep(200);

        // get the brightness from the sensor, convert it to a string and print it on the LCD
        String brightness = doubleToString(room.getBrightness(), 2) + " Lux";
        gui.PutString(35, 80, brightness + "  ");

        // send a message with the brightness to the ESP8266 and print the message on backchannel uart
        String brightnessMsg = "Brightness: " + brightness + "\n";
        printf("%s", ("\nSending message to ESP8266: " + brightnessMsg).c_str());
        uart_esp.puts(brightnessMsg.c_str());

        task::sleep(5000);
    }
}

/**
 * GPIO3 is connected to ESP8266 GPIO15, which controls the boot-mode of the ESP8266.
 * The WiFiTick-board pulls this pin to LOW, but the EDU-MKII pulls it to HIGH (because it is the READY-line of the temperature sensor).
 * So force this pin to low so the ESP8266 is booting correctly.
 */
void enforceCorrectBoot() {
    // pull the pin to low to make sure that the ESP8266 is booting correctly
    gpio_rp2040_pin gpio3(3);
    gpio3.gpioMode(GPIO::OUTPUT | GPIO::INIT_LOW);

    // Reset the ESP8266
    gpio_rp2040_pin esp_reset(6);
    esp_reset.gpioMode(GPIO::OUTPUT | GPIO::INIT_LOW);
    task::sleep(200);
    esp_reset.gpioWrite(HIGH);
    task::sleep(200);
}

/**
 * Funktion to initialize backchannel UART (default 115200 baud)
 * Registers the backchannel uart for printf.
 * @param uart backchannel uart for the rp2040
 */
void initializeUart(uart_rp2040 &uart) {
    posix_io::inst.register_stdout(uart);
    posix_io::inst.register_stderr(uart);
}

/**
 * Funktion to initialize the LCD display.
 * @param lcdBacklight gpio pin for the LCD background led
 * @param spi the SPI interface
 * @param lcd the LCD driver
 * @param gui uGui to set background color and font size
 */
void initializeDisplay(gpio_rp2040_pin &lcdBacklight, spi_rp2040 &spi, st7735s_drv &lcd, uGUI &gui) {
    // Enable the LCD background led
    lcdBacklight.setMode(GPIO::OUTPUT);
    lcdBacklight.gpioWrite(true);

    // Setup SPI interface
    spi.setSpeed(30000000);

    // Setup LCD driver
    lcd.clearScreen(0x0);

    // Setup uGUI
    gui.FontSelect(&FONT_8X12);
    gui.FillScreen(C_BLACK);
    gui.FontSetHSpace(0);
    gui.SetForecolor(C_WHITE);
    gui.SetBackcolor(C_BLACK);
}