#include "PCD8544.h"

void PCD8544::initialize(PCD8544Pin_t reset, PCD8544Pin_t dc) {
    *(reset.DDR) |= _BV(reset.PIN);
    *(dc.DDR)    |= _BV(dc.PIN);

    //Configure control pins
    pinMode(scePin, OUTPUT);
    pinMode(sdinPin, OUTPUT);
    pinMode(sclkPin, OUTPUT);

    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);

    // Reset the LCD to a known state
    *(reset.PORT) &= ~_BV(reset.PIN);
    *(reset.PORT) |= _BV(reset.PIN);

    LCDWrite(LCD_COMMAND, 0x21); //Tell LCD extended commands follow
    LCDWrite(LCD_COMMAND, 0xB0); //Set LCD Vop (Contrast)
    LCDWrite(LCD_COMMAND, 0x04); //Set Temp coefficent
    LCDWrite(LCD_COMMAND, 0x14); //LCD bias mode 1:48 (try 0x13)
    //We must send 0x20 before modifying the display control mode
    LCDWrite(LCD_COMMAND, 0x20);
    LCDWrite(LCD_COMMAND, 0x0C); //Set display control, normal mode.
}

void PCD8544::setContrast(uint8_t contrast) {
    // Tell LCD that extended commands follow
    this->write(PCD8544_DC_COMMAND, 0x21);

    // Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
    this->write(PCD8544_DC_COMMAND, (uint8_t) (0x80 | contrast));

    // Set display mode
    this->write(PCD8544_DC_COMMAND, 0x20);
}
