#include <LCD.h>

#include <stdint.h>

static uint8_t LCDBuffer[504];

void draw(){}

int main() {
    // Initialize LCD graphics library with buffer, display size, draw callback
    LCD lcd = LCD(LCDBuffer, 84, 48, draw);
}
