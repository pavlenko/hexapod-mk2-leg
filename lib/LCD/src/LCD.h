#ifndef LCD_H
#define LCD_H

#include <stdint.h>

//TODO flush pass entry buffer to callback to allow reset x,y coordinates
//TODO flush part of buffer by define x,y,w,h
class LCD {
private:
    uint8_t *_buffer;
    uint8_t _width;
    uint8_t _height;
    void (*_write)(uint8_t byte);

    /**
     * Draw single symbol, used in string
     *
     * @param symbol
     * @param x
     * @param y
     */
    void symbol(char symbol, uint8_t x, uint8_t y, uint8_t offset);
public:
    /**
     * Initialize LCD buffer, size and draw callback
     *
     * @param buffer
     * @param width
     * @param height
     * @param write
     */
    LCD(uint8_t *buffer, uint8_t width, uint8_t height, void (*write)(uint8_t byte));

    /**
     * Clear buffer to initial state
     */
    void clear();

    /**
     * Draw string
     *
     * @param string
     * @param x
     * @param y
     */
    void string(const char *string, uint8_t x, uint8_t y);

    /**
     * Draw single pixel
     *
     * @param x
     * @param y
     * @param value
     */
    void pixel(int x, int y, bool value);

    /**
     * Draw line
     *
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     */
    void line(int x1, int y1, int x2, int y2);

    /**
     * Draw rectangle
     *
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     */
    void rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

    /**
     * Draw circle
     *
     * @param x
     * @param y
     * @param radius
     */
    void circle(int x, int y, int radius);

    /**
     * Draw bitmap
     *
     * @param bitmap
     * @param x
     * @param y
     * @param w
     * @param h
     */
    void bitmap(uint8_t *bitmap, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

    /**
     * Send buffer to display via draw callback
     */
    void flush();
};

#endif //LCD_H
