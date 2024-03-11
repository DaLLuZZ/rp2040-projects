//
// Created by LME on 19.01.2024.
//

#ifndef MCT_RP2040_DISPLAY_H
#define MCT_RP2040_DISPLAY_H
// Board includes
#include "boostxl_eduMKII.h"
// Driver includes
#include "st7735s_drv.h"
#include "uGUI.h"
#include "task.h"

extern const uint16_t pfeil_rechts[16384];
extern const uint16_t pfeil_links[16384];
extern const uint16_t pfeil_hoch[16384];
extern const uint16_t pfeil_runter[16384];

#include "gpio_rp2040.h"
#include "spi_rp2040.h"
#include "st7735s_drv.h"
#include "uGUI.h"
#include "uGUI_colors.h"
#include "font_4x6.h"
#include "font_5x8.h"
#include "font_5x12.h"
#include "font_6x8.h"
#include "font_6x10.h"
#include "font_7x12.h"
#include "font_8x12.h"
#include "font_22x36.h"
#include "yahal_String.h"

#include <cstdlib>

class Display {
public:
    void showAction(String action);

    void showText(String text);
};


#endif //MCT_RP2040_DISPLAY_H
