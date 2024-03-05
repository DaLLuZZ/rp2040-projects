//
// Created by another student on 28.02.2024.
//

#ifndef LABYRINTH_PICELSTREAMRGB565CONST_H
#define LABYRINTH_PICELSTREAMRGB565CONST_H

/// @brief Implementation of pixelstream, pixel_stream_const was using rgb888 and changing the colors on conversion
class pixel_stream_rgb565_const : public pixel_stream
{
public:
    pixel_stream_rgb565_const(uint16_t c) : color(c) { }
    virtual ~pixel_stream_rgb565_const() = default;

    inline color_t getColorType() override {
        return LCD::COLORTYPE_RGB565;
    }
    inline color_t getNext() override {
        return color | LCD::COLORTYPE_RGB565;
    }
    inline void reset() { }

private:
    uint16_t color;
};


#endif //LABYRINTH_PICELSTREAMRGB565CONST_H
