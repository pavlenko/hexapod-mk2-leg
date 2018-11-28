#ifndef LCD_H
#define LCD_H

#include <stdint.h>

//TODO define bitmap as structure below to store all config in one place
typedef struct {
    const uint8_t *data;
    uint8_t width;
    uint8_t height;
    bool byteOrientation;
    bool byteEndian;
} LCDBitmap_t;

//TODO flush pass entry buffer to callback to allow reset x,y coordinates
//TODO flush part of buffer by define x,y,w,h
class LCD {
private:
    uint8_t *_buffer;
    uint8_t _width;
    uint8_t _height;
    uint16_t _size;
    void (*_write)(uint8_t);
public:
    /**
     * Initialize LCD buffer, size and draw callback
     *
     * @param buffer
     * @param width
     * @param height
     * @param write
     */
    LCD(uint8_t *buffer, uint8_t width, uint8_t height, void (*write)(uint8_t));

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
    void string(const char *string, uint8_t x, uint8_t y, bool wrap = true);

    /**
     * Draw single pixel
     *
     * @param x
     * @param y
     * @param value
     */
    void pixel(uint8_t x, uint8_t y, bool value);

    /**
     * Draw line
     *
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     */
    void line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

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
    void circle(uint8_t x, uint8_t y, uint8_t radius);

    /**
     * Draw bitmap
     *
     * @param bitmap   Byte array
     * @param x        Horizontal offset, px
     * @param y        Vertical offset, px
     * @param width    Width, px
     * @param height   Height, px
     * @param vertical Byte orientation
     */
    void bitmap(const uint8_t *bitmap, uint8_t x, uint8_t y, uint8_t width, uint8_t height, bool vertical = false);

    void bitmap(uint8_t x, uint8_t y, LCDBitmap_t bitmap);

    /**
     * Send buffer to display via draw callback
     */
    void flush();
};

#endif //LCD_H
