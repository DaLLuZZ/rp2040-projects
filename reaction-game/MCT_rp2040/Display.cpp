//
// Created by LME on 19.01.2024.
//

#include "Display.h"


void Display::showAction(String action) {
    // Abhängigkeiten einfügen
    boostxl_eduMKII edu;
    edu.need_led_rgb = true;
    edu.need_lcd = true;
    edu.inject();

    // Hintergrundbeleuchtung einschalten
    edu.led_red().gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);
    // SPI Interface einstellen
    edu.lcd_spi().setSpeed(30000000);

    if (action == "up" || action == "down" || action == "left" ||
        action == "right") { //In dem Fall Anzeige eines PNGs nötig
        // LCD Treiber einstellen
        st7735s_drv lcd(edu.lcd_spi(), edu.lcd_rst(), edu.lcd_dc(),
                        st7735s_drv::Crystalfontz_128x128);

        // uGui einstellen
        uGUI gui(lcd);

        // Bitmap Objekt einstellen
        uGUI::BMP bmp;
        bmp.height = 128;
        bmp.width = 128;
        bmp.bpp = 16;
        bmp.colors = BMP_RGB565;

        if (action == "right") {
            bmp.p = pfeil_rechts;
        } else if (action == "left") {
            bmp.p = pfeil_links;
        } else if (action == "up") {
            bmp.p = pfeil_hoch;
        } else if (action == "down") {
            bmp.p = pfeil_runter;
        }

        // Bitmap Bild anzeigen
        gui.DrawBMP(0, 0, &bmp);
    } else { //wenn Formen und Text direkt angezeigt werden soll
        // SPI Interface einstellen
        gpio_rp2040_pin lcd_cs(5);
        spi_rp2040 spi(0, 4, 7, 22, lcd_cs);
        spi.setSpeed(30000000);

        // LCD Treiber einstellen
        gpio_rp2040_pin lcd_rst(9);
        gpio_rp2040_pin lcd_dc(2);
        st7735s_drv lcd(spi, lcd_rst, lcd_dc, st7735s_drv::Crystalfontz_128x128);

        // uGui einstellen
        uGUI gui(lcd);

        lcd.clearScreen(0x0);
        gui.SetForecolor(C_RED);
        if (action == "joystick") {
            gui.FillCircle(64, 64, 10, C_RED);
        } else {
            gui.FontSelect(&FONT_22X36);
            gui.PutString(47, 47, action.c_str());
        }
        gui.DrawCircle(64, 64, 63, C_YELLOW);
        gui.DrawCircle(64, 64, 62, C_YELLOW);
        gui.DrawCircle(64, 64, 61, C_YELLOW);
        gui.DrawCircle(64, 64, 60, C_YELLOW);
    }
}

void Display::showText(String text) {
    // Abhängigkeiten einfügen
    boostxl_eduMKII edu;
    edu.need_led_rgb = true;
    edu.need_lcd = true;
    edu.inject();

    // Hintergrundbeleuchtung einschalten
    edu.led_red().gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);
    // SPI Interface einstellen
    gpio_rp2040_pin lcd_cs(5);
    spi_rp2040 spi(0, 4, 7, 22, lcd_cs);
    spi.setSpeed(30000000);

    // LCD Treiber einstellen
    gpio_rp2040_pin lcd_rst(9);
    gpio_rp2040_pin lcd_dc(2);
    st7735s_drv lcd(spi, lcd_rst, lcd_dc, st7735s_drv::Crystalfontz_128x128);

    // uGui einstellen
    uGUI gui(lcd);
    lcd.clearScreen(0x0);
    gui.SetForecolor(C_RED);
    gui.FontSelect(&FONT_7X12);
    //Überschrift anzeigen
    gui.PutString(10, 40, "Reaktionsspiel");
    gui.SetForecolor(C_YELLOW);

    //jeweiligen Text anzeigen
    if (text == "Startbildschirm") {
        gui.PutString(10, 80, "Zum Starten S1 drücken");
    } else if (text == "Restart") {
        gui.PutString(10, 80, "Zum Neustarten S1 drücken");
    } else if (text == "Verbinde") {
        gui.PutString(18, 80, "Verbinde ...");
    } else if (text == "Starten") {
        gui.PutString(12, 80, "Spiel starten");
    } else if (text == "Zeit") {
        gui.PutString(15, 80, "Zeit vorbei");
    } else if (text == "falsch") {
        gui.PutString(10, 80, "falsche Aktion");
    }
}
