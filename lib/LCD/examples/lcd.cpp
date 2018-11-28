#include <LCD.h>

#include <stdint.h>

static uint8_t LCDBuffer[504];

void draw(uint8_t){}

// Represents char "E"
const static uint8_t test_bitmapH_8x5_data[] PROGMEM = {
        0xFF,
        0x80,
        0xFF,
        0x80,
        0xFF,
};

LCDBitmap_t test_bitmapH_8x5 = {.data = test_bitmapH_8x5_data, .width = 8, .height = 5, .horizontal = false, .msb = true};

// Represents char "M"
const static uint8_t test_bitmapH_5x8_data[] PROGMEM = {
        0xFF, 0x80, 0xFF, 0x080, 0xFF,
};

LCDBitmap_t test_bitmapH_5x8 = {.data = test_bitmapH_5x8_data, .width = 5, .height = 8, .horizontal = true, .msb = true};

int main() {
    // Initialize LCD graphics library with buffer, display size, draw callback
    LCD lcd = LCD(LCDBuffer, 84, 48, draw);

    lcd.bitmap(0, 0, test_bitmapH_5x8);
    lcd.bitmap(16, 0, test_bitmapH_8x5);
}
