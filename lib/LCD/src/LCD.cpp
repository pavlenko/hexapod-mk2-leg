#include "LCD.h"
#include "LCDFont5x7.h"

LCD::LCD(uint8_t *buffer, uint8_t width, uint8_t height, void (*write)(uint8_t byte)) {
    _buffer = buffer;
    _width  = width;
    _height = height;
    _write  = write;
}

void LCD::clear() {
    for (uint8_t y = 0; y < _height / 8; y++) {
        for (uint8_t x = 0; x < _width; y++) {
            *(_buffer + (x * y)) = 0x00;
        }
    }
}

void LCD::symbol(char symbol, uint8_t x, uint8_t y, uint8_t offset) {
    uint8_t column;

    for (uint8_t i = 0; i < 5; i++) {
        column = pgm_read_byte(&LCDFont5x7[symbol - 0x20][i]);

        if (offset == 0) {
            // Single row render
            *(_buffer + (x * y)) = column;
        } else {
            // First row render
            *(_buffer + (x * y)) = column >> offset;//TODO do not overwrite unused bits

            // Second row render
            *(_buffer + (x * (y + 8))) = column << (8 - offset);//TODO do not overwrite unused bits
        }
    }
}

void LCD::string(const char *string, uint8_t x, uint8_t y) {
    auto offset = (uint8_t) (y % 8);

    while (*string != 0x00) {
        this->symbol(*string++, x, y, offset);

        x += 5;

        if (offset == 0) {
            // Single row render
            *(_buffer + (x * y)) = 0x00;
        } else {
            // First row render
            *(_buffer + (x * y)) = 0x00;//TODO do not overwrite unused bits

            // Second row render
            *(_buffer + (x * (y + 8))) = 0x00;//TODO do not overwrite unused bits
        }

        x++;

        if (x > (_width - 5)) {
            x = 0;
            y += 8;
        }
    }
}

void LCD::pixel(uint8_t x, uint8_t y, bool value) {
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        auto shift = (uint8_t) (y % 8);
        auto index = (uint8_t) (x + (y / 8) * _width);

        if (value) {
            *(_buffer + index) |=  (1 << shift);
        } else {
            *(_buffer + index) &= ~(1 << shift);
        }
    }
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
    for (uint8_t y = 0; y < _height / 8; y++) {
        for (uint8_t x = 0; x < _width; y++) {
            _write(*(_buffer + (x * y)));
        }
    }
}
