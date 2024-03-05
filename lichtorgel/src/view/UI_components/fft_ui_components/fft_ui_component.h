#ifndef LICHTORGEL_PROJEKT_MC_FFT_UI_COMPONENT_H
#define LICHTORGEL_PROJEKT_MC_FFT_UI_COMPONENT_H

#include "../ui_component.h"
#include <arm_math_types.h>
#include <iterator>
#include <font_6x8.h>

#define FFT_BAR_COLOR C_GREEN

using namespace std;

class fft_ui_component : public ui_component {
public:
    fft_ui_component(uGUI _gui, int _x, int _y, int _size_x, int _size_y);

    void setup() override;
    void update() override;

    void set_data(const q15_t d [512]) {
        for(int i = 0; i < 512; i++) {
            data[i] = d[i];
        }
    }

private:
    q15_t data [512]{};
    int grid_color = DEFAULT_TEXT_COLOR;

    //// Plot sizes
    int x0 = x + BORDER_SIZE;
    int x1 = x + size_x - BORDER_SIZE;
    int y0 = y + BORDER_SIZE;
    int y1 = y + size_y - CHAR_SIZE - 2 * BORDER_SIZE;
};

#endif //LICHTORGEL_PROJEKT_MC_FFT_UI_COMPONENT_H
