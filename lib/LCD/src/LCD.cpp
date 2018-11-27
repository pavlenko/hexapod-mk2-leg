#include "LCD.h"
#include "LCDFont5x7.h"

#include <avr/pgmspace.h>
#include <stdlib.h>

LCD::LCD(uint8_t *buffer, uint8_t width, uint8_t height, void (*write)(uint8_t byte)) {
    _buffer = buffer;
    _width  = width;
    _height = height;
    _write  = write;
}

void LCD::clear() {
    for (int i = 0; i < (_width * _height / 8); i++) {
        *(_buffer + i) = 0x00;
    }
}

//TODO check string in display boundaries or maybe use pixel method for automatically fit lcd size
void LCD::string(const char *string, uint8_t x, uint8_t y) {
    int shift = (y % 8), index;

    while (*string != 0x00) {
        index = (x + (y / 8) * _width);

        for (uint8_t i = 0; i < 5; i++) {
            uint8_t column = pgm_read_byte(&LCDFont5x7[(*string) - 0x20][i]);

            if (shift == 0) {
                // Single row render
                *(_buffer + index + i) = column;
            } else {
                // First row render
                *(_buffer + index + i) = column << shift;

                // Second row render
                *(_buffer + index + i + _width) = column >> (8 - shift);
            }
        }

        // Go to next char
        string++;

        x += 5;

        index = (x + (y / 8) * _width);
        if (shift == 0) {
            // Single row render
            *(_buffer + index) = 0x00;
        } else {
            // First row render
            *(_buffer + index) = 0x00;

            // Second row render
            *(_buffer + index + _width) = 0x00;
        }

        x++;

        if (x > (_width - 5)) {//TODO optional wrap
            x = 0;
            y += 8;
        }
    }
}

void LCD::pixel(int x, int y, bool value) {
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        int shift = (y % 8);
        int index = (x + (y / 8) * _width);

        if (value) {
            *(_buffer + index) |=  (1 << shift);
        } else {
            *(_buffer + index) &= ~(1 << shift);
        }
    }
}

void LCD::line(int x0, int y0, int x1, int y1) {
    int diffX = abs(x1 - x0);
    int diffY = abs(y1 - y0);

    int stepX = (x0 > x1) ? -1 : 1;
    int stepY = (y0 > y1) ? -1 : 1;

    //TODO check if this works
    diffY <<= 1; // diffY is now 2*diffY
    diffX <<= 1; // diffX is now 2*diffX

    this->pixel(x0, y0, true);

    if (diffX > diffY) {
        // Optimization for draw line with width > height
        int fraction = diffY - (diffX >> 1);

        while (x0 != x1) {
            if (fraction >= 0) {
                y0       += stepY;
                fraction -= diffX;
            }

            x0       += stepX;
            fraction += diffY;

            this->pixel(x0, y0, true);
        }
    } else {
        // Optimization for draw line with width < height
        int fraction = diffX - (diffY >> 1);

        while (y0 != y1) {
            if (fraction >= 0) {
                x0       += stepX;
                fraction -= diffY;
            }

            y0       += stepY;
            fraction += diffX;

            this->pixel(x0, y0, true);
        }
    }
}

void LCD::rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
//TODO not yet implemented
}

void LCD::circle(int x0, int y0, int radius) {
    int x   = radius - 1;
    int y   = 0;
    int dx  = 1;
    int dy  = 1;
    int err = dx - (radius << 1);

    while (x >= y)
    {
        this->pixel(x0 + x, y0 + y, true);
        this->pixel(x0 + y, y0 + x, true);
        this->pixel(x0 - y, y0 + x, true);
        this->pixel(x0 - x, y0 + y, true);
        this->pixel(x0 - x, y0 - y, true);
        this->pixel(x0 - y, y0 - x, true);
        this->pixel(x0 + y, y0 - x, true);
        this->pixel(x0 + x, y0 - y, true);

        if (err <= 0) {
            y++;
            err += dy;
            dy  += 2;
        } else {
            x--;
            dx  += 2;
            err += dx - (radius << 1);
        }
    }
}

void LCD::bitmap(uint8_t *bitmap, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    //TODO calculate row shift, min row, max row

    for (int i = 0; i < (_width * _height / 8); i++) {
        *(_buffer + (x * y)) = pgm_read_byte(&bitmap[i]);
    }
}

void LCD::flush() {
    for (int i = 0; i < (_width * _height / 8); i++) {
        _write(*(_buffer + i));
    }
}
