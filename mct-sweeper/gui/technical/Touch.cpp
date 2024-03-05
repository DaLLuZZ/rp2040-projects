//
// Created by tobia on 01.02.2024.
//

#include "Touch.h"

/**
 * Constructor of the class touch. Touch can be used to listen to user input via the touch screen. Constructor only initializes necessary attributes.
 *
 * Original Author: A. Terstegge
 *
 * @param lcd The lcd-Display with touch function.
 * @param gui The gui to draw the calibration.
 */
Touch::Touch(lcd_interface &lcd, uGUI &gui) : lcd(lcd), gui(gui), XN(2), YN(3), XP(27), YP(26), isCalibrated(false) {};

/**
 * Method to check if the touchscreen had been touched.
 *
 * Original Author: A. Terstegge
 *
 * @return Bool if a touch had been registered.
 */
bool Touch::pressed() {
    uint16_t x, y;
    this->getRawXY(x, y);
    return x>400 && y>400;
}

/**
 * Function to calibrate the screen. Calibration will print Instructions to the gui and show 3 Point where the user should touch the display.
 * After that the factors for the transformation can be calculated.
 */
void Touch::calibrate() {
    // Debugging
    printf("DEBUG   --CALIBRATION STARTED--\n");
    uint16_t screenSizeX = lcd.getSizeX()-1;
    uint16_t screenSizeY = lcd.getSizeY()-1;
    printf("DEBUG       SCREENSIZE X=%d, Y=%d\n", screenSizeX, screenSizeY);

    // Resetting Screen
    gui.FillScreen(C_BLACK);
    // Write Calibration Title
    String calibrationTitle = "CALIBRATION";
    gui.FontSelect(&FONT_10X16);
    gui.SetForecolor(C_WHITE);
    gui.PutString((int16_t)(screenSizeX/2 - calibrationTitle.size() * FONT_10X16.char_width / 2), (int16_t)(screenSizeY/10), calibrationTitle);
    // Write Calibration Instruction
    String calibrationInstruction = "Please tap the circles ";
    gui.FontSelect(&FONT_7X12);
    gui.SetForecolor(C_WHITE);
    gui.PutString((int16_t)(screenSizeX/2 - calibrationInstruction.size() * FONT_7X12.char_width / 2), (int16_t)(screenSizeY * 3/10), calibrationInstruction);


    // First Calibration Point
    gui.FillCircle(int(screenSizeX * 0.2), int(screenSizeY * 0.2), 5, C_BLUE);
    while(!pressed()){}
    task::sleep(10); // Debounce
    getRawXY(ax, ay);
    while(pressed()){}
    printf("            CALIBRATE A Ax=%d, Ay=%d \n", ax, ay);
    gui.FillCircle(int(screenSizeX * 0.2), int(screenSizeY * 0.2), 5, C_GREEN);

    // Second Calibration Point
    gui.FillCircle(int(screenSizeX * 0.8), int(screenSizeY * 0.2), 5, C_BLUE);
    while(!pressed()){}
    task::sleep(10); // Debounce
    getRawXY(bx, by);
    while(pressed()){}
    printf("            CALIBRATE B Bx=%d, By=%d \n", bx, by);
    gui.FillCircle(int(screenSizeX * 0.8), int(screenSizeY * 0.2), 5, C_GREEN);

    // Third Calibration Point
    gui.FillCircle(int(screenSizeX * 0.2), int(screenSizeY * 0.8), 5, C_BLUE);
    while(!pressed()){}
    task::sleep(10); // Debounce
    getRawXY(cx, cy);
    while(pressed()){}
    printf("            CALIBRATE C Cx=%d, Cy=%d \n", cx, cy);
    gui.FillCircle(int(screenSizeX * 0.2), int(screenSizeY * 0.8), 5, C_GREEN);

    // Calculate transformation factors
    sx = (screenSizeX * 0.6) / (bx - ax);
    sy = (screenSizeY * 0.6) / (cy - ay);
    tx = screenSizeX * 0.2 - (ax * sx);
    ty = screenSizeY * 0.2 - (ay * sy);

    // Reset screen
    gui.FillScreen(C_BLACK);
    printf("DEBUG   --CALIBRATION FINISHED--\n");
    isCalibrated = true;
}


/**
 * Method to wait for a user input via touch. One touch-input consists of the initial contact as well as the letting go.
 *
 * Original Author: A. Terstegge
 *
 * @param x x-Coordinate of the touch will be written in this variable.
 * @param y y-Coordinate of the touch will be written in this variable.
 */
void Touch::waitForPress(uint16_t &x, uint16_t &y) {
    if(!isCalibrated){return;};
    while(!pressed()){}
    task::sleep(10); // Debounce
    getRawXY(x, y);
    while(pressed()){}
    transformLinear(x,y);
}

/**
 * Method to get raw Touch-Coordinates via ADC
 *
 * Original Author: A. Terstegge
 * 
 * @param x x-Coordinate of the touch will be written in this variable.
 * @param y y-Coordinate of the touch will be written in this variable.
 */
void Touch::getRawXY(uint16_t &x, uint16_t &y) {
    XN.gpioMode(GPIO::OUTPUT | GPIO::INIT_LOW);
    XP.gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);
    YN.gpioMode(GPIO::INPUT);
    YP.gpioMode(GPIO::INPUT | GPIO::PULLDOWN);
    task::sleep(10);
    x = adc_rp2040::inst.adcReadRaw(0);
    YN.gpioMode(GPIO::OUTPUT | GPIO::INIT_LOW);
    YP.gpioMode(GPIO::OUTPUT | GPIO::INIT_HIGH);
    XN.gpioMode(GPIO::INPUT);
    XP.gpioMode(GPIO::INPUT | GPIO::PULLDOWN);
    task::sleep(10);
    y = adc_rp2040::inst.adcReadRaw(2);
}

/**
 * Method to map the touch input to the corresponding pixel value of the screen
 *
 * @param x x-Coordinate of the touch will be written in this variable.
 * @param y y-Coordinate of the touch will be written in this variable.
 */
void Touch::transformLinear(uint16_t &x, uint16_t &y) const {
    uint16_t xnew = (x * sx) + tx;
    uint16_t ynew = (y * sy) + ty;
    if (xnew < 0 && xnew > gui.GetXDim() && ynew < 0 && ynew > gui.GetYDim()){ // If Click was translated wrong go to (0,0)
        x = 0;
        y = 0;
    }
    x = xnew;
    y = ynew;
}

/**
 * Debug method to see if the Callibration worked. Won't be accessible from the interface.
 *
 * @param allowedClicks Number of allowed clicks before the normal system will continue.
 */
void Touch::testCalibration(uint16_t allowedClicks) {
    printf("DEBUG   --TESTING CALIBRATION FOR %d CLICKS--\n", allowedClicks);
    gui.FillScreen(C_BLUE);

    uint16_t x, y;
    for (int i = 0; i < allowedClicks; ++i) {
        waitForPress(x, y);
        printf("DEBUG       INPUT x=%d, y=%d\n", x, y);
        gui.FillCircle(x, y, 5, C_WHITE);
    }

    task::sleep(100);
    printf("DEBUG   --ENDING TESTING CALIBRATION--\n");
    gui.FillScreen(C_BLACK);
}
