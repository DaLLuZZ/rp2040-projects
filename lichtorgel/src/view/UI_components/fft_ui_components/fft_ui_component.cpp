#include "fft_ui_component.h"


//// Constructor, utilizing the  member initializer list
fft_ui_component::fft_ui_component(uGUI _gui, int _x, int _y, int _size_x, int _size_y)
    : ui_component(_gui, _x, _y, _size_x, _size_y) { }

//// Draw the grid for the fft-plot
void fft_ui_component::setup() {
    _parent.FontSelect(&FONT_6X8);
    _parent.FontSetHSpace(0);

    //// Frame
    _parent.DrawFrame(x, y, x + size_x, y + size_y, FRAME_COLOR_UNFOCUSED);

    //// Axis
    _parent.DrawLine(x0, y1, x1, y1, grid_color); // x-Axis
    _parent.DrawLine(x0, y0, x0, y1, grid_color); // y-Axis

    //// x-axis ticks
    int single_tick_size = (size_x - 4) / 4;
    for(int i = 0; i < 5; i++){
        //// Long ticks
        int x_longT = x + BORDER_SIZE + i * single_tick_size;
        _parent.DrawLine(x_longT, y1, x_longT, y1 + TICK_LENGTH, grid_color);

        //// Short ticks
        int x_shortT = x_longT + single_tick_size / 2;
        _parent.DrawLine(x_shortT, y1, x_shortT, y1 + TICK_LENGTH / 2, grid_color);

        //// Number label
        if(i == 0) {
            _parent.PutString(x_longT + 1, y1 + 4, to_string(i).c_str());
        }
        else {
            _parent.PutString(x_longT - 3, y1 + 4, to_string(i).c_str());
        }
    }

    //// x-axis label
    _parent.SetForecolor(FFT_BAR_COLOR);
    _parent.PutString(99, y1 + 4, "kHz");
    _parent.SetForecolor(DEFAULT_TEXT_COLOR);
}

//// Draw the fft result as a bar chart
void fft_ui_component::update() {
    int bins = size_x - 4;
    int step_size = 128 / bins;

    for(int i = 0; i < bins; i++) {
        int index = i * step_size;
        int y = std::min(y1-y0, (int)((data[index] + data[index + 1]) >> 9));
        int xc = i + x + BORDER_SIZE;
        _parent.DrawLine(xc, y0,    xc, y1-y-1, DEFAULT_BACKGROUND_COLOR);
        _parent.DrawLine(xc, (y1-y), xc, y1,FFT_BAR_COLOR);
    }
}
