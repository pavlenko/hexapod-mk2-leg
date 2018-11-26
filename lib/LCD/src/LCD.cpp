#include "LCD.h"
#include "LCDFont5x7.h"

LCD::LCD(uint8_t *buffer, uint8_t width, uint8_t height, void (*draw)()) {
    _buffer = buffer;
    _width  = width;
    _height = height;
    _draw   = draw;
}

void LCD::clear() {
    for (uint8_t y = 0; y < _height / 8; y++) {
        for (uint8_t x = 0; x < _width; y++) {
            *(_buffer + (x * y)) = 0x00;
        }
    }
}

void LCD::string(const char *string, uint8_t x, uint8_t y) {
    //TODO not yet implemented
    while (*string != 0x00) {
        //TODO check if y is divided by 8: yes - simple write bytes, else write each byte 2 times (upper part & lower part)
        string++;
    }
}

void LCD::pixel(uint8_t x, uint8_t y) {
//TODO not yet implemented
}

void LCD::line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
//TODO not yet implemented
}

void LCD::rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
//TODO not yet implemented
}

void LCD::circle(uint8_t x, uint8_t y, uint8_t radius) {
//TODO not yet implemented
}

void LCD::bitmap(uint8_t *bitmap, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
//TODO not yet implemented
}

void LCD::flush() {
//TODO not yet implemented
}
