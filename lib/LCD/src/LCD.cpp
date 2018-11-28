#include "LCD.h"
#include "LCDFont5x7.h"

#include <avr/pgmspace.h>
#include <stdlib.h>

#define LCD_MIN(_a_, _b_) (_a_ > _b_ ? _b_ : _a_)
#define LCD_MAX(_a_, _b_) (_a_ < _b_ ? _b_ : _a_)

LCD::LCD(uint8_t *buffer, uint8_t width, uint8_t height, void (*write)(uint8_t)) {
    _buffer = buffer;
    _width  = width;
    _height = height;
    _write  = write;

    _size = (uint16_t) (_width * _height / 8);
}

void LCD::clear() {
    for (uint16_t i = 0; i < _size; i++) {
        *(_buffer + i) = 0x00;
    }
}

void LCD::string(const char *string, uint8_t x, uint8_t y, bool wrap) {
    int shift = (y % 8);
    uint16_t index;

    while (*string != 0x00) {
        index = (uint16_t) (x + (y / 8) * _width);

        for (uint8_t i = 0; i < 5; i++) {
            uint8_t column = pgm_read_byte(&LCDFont5x7[(*string) - 0x20][i]);

            if (index + i < _size) {
                if (shift == 0) {
                    // Single row render
                    *(_buffer + index + i) = column;
                } else {
                    // First row render
                    *(_buffer + index + i) = column << shift;

                    if (index + i + _width < _size) {
                        // Second row render
                        *(_buffer + index + i + _width) = column >> (8 - shift);
                    }
                }
            }
        }

        // Go to next char
        string++;

        x += 5;

        index = (x + (y / 8) * _width);
        if (index < _size) {
            if (shift == 0) {
                // Single row render
                *(_buffer + index) = 0x00;
            } else {
                // First row render
                *(_buffer + index) = 0x00;

                if (index + _width < _size) {
                    // Second row render
                    *(_buffer + index + _width) = 0x00;
                }
            }
        }

        x++;

        if (wrap && x > (_width - 5)) {
            x = 0;
            y += 8;
        }
    }
}

void LCD::pixel(uint8_t x, uint8_t y, bool value) {
    if (x < _width && y < _height) {
        int shift = (y % 8);
        int index = (x + (y / 8) * _width);

        if (value) {
            *(_buffer + index) |=  (1 << shift);
        } else {
            *(_buffer + index) &= ~(1 << shift);
        }
    }
}

void LCD::line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    int diffX = abs(x1 - x0);
    int diffY = abs(y1 - y0);

    int stepX = (x0 > x1) ? -1 : 1;
    int stepY = (y0 > y1) ? -1 : 1;

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
    this->line(x1, y1, x1, y2);
    this->line(x2, y1, x2, y2);
    this->line(x1, y1, x2, y1);
    this->line(x1, y2, x2, y2);
}

void LCD::circle(uint8_t x0, uint8_t y0, uint8_t radius) {
    int8_t x   = (uint8_t) (radius - 1);
    int8_t y   = 0;
    int8_t dx  = 1;
    int8_t dy  = 1;
    int8_t err = dx - (radius << 1);

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

void LCD::bitmap(const uint8_t *bitmap, uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool vertical) {
    //TODO need to check
    int16_t byteWidth = (width + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    for (int8_t j = 0; j < height; j++, y++) {
        for (int8_t i = 0; i < width; i++ ) {
            if (i & 7) {
                //byte <<= 1;
                byte >>= 1;
            } else {
                byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
            }

            //this->pixel(x + i, y, (byte & 0x80) == 0);
            this->pixel(x + i, y, (byte & 0x01) == 0);
        }
    }
}

// Represents char "E"
const static uint8_t bitmapH_8x5[] PROGMEM = {
    0xFF, 0x80, 0xFF, 0x80, 0xFF,
};

// Represents char "M"
const static uint8_t bitmapV_5x8[] PROGMEM = {
    0xFF, 0x80, 0xFF, 0x080, 0xFF,
};

void LCD::bitmap(uint8_t x0, uint8_t y0, LCDBitmap_t bitmap) {
    auto scanX = (uint8_t) ((bitmap.width + 7) / 8);
    auto scanY = (uint8_t) ((bitmap.height + 7) / 8);

    uint8_t byte = 0;

    for (uint8_t x = 0; x < bitmap.width; x++) {
        for (uint8_t y = 0; y < bitmap.height; y++) {
            if (x & 7) {
                // Shift byte - used in horizontal byte bitmaps
                byte <<= 1;
            } else {
                // Load bitmap next byte
                byte = pgm_read_byte(&(bitmap.data)[x * scanX + y / 8]);//TODO check
            }

            if (y & 7) {
                // Shift byte - used in vertical byte bitmaps
                byte <<= 1;
            } else {
                // Load bitmap next byte
                byte = pgm_read_byte(&(bitmap.data)[y * scanY + x / 8]);//TODO check
            }

            if (byte & 0x80) {
                // Set pixel for MSB? byte order
                this->pixel(x0 + x, y0 + y, true);
            }

            if (byte & 0x01) {
                // Set pixel for LSB? byte order
                this->pixel(x0 + x, y0 + y, true);
            }
        }
    }
}

void LCD::flush() {
    for (uint16_t i = 0; i < _size; i++) {
        _write(*(_buffer + i));
    }
}
