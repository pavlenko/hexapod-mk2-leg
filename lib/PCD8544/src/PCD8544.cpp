#include "PCD8544.h"

#include <SPI.h>

void PCD8544::initialize(PCD8544Pin_t reset, PCD8544Pin_t dc) {
    *(reset.DDR) |= _BV(reset.PIN);
    *(dc.DDR)    |= _BV(dc.PIN);

    //Configure control pins
    pinMode(scePin, OUTPUT);
    pinMode(sdinPin, OUTPUT);
    pinMode(sclkPin, OUTPUT);

    SPI.begin();//TODO start as master
    SPI.setDataMode(SPI_MODE_SAMPLE_RISING_NORMAL);
    SPI.setDataOrder(SPI_DATA_ORDER_MSB_FIRST);

    // Reset the LCD to a known state
    *(reset.PORT) &= ~_BV(reset.PIN);
    *(reset.PORT) |= _BV(reset.PIN);

    this->write(PCD8544_DC_COMMAND, PCD8544_FUNCTION_SET(0, 0, 1));  // <-- Tell LCD extended commands follow
    this->write(PCD8544_DC_COMMAND, PCD8544_VOP(0x30));              // <-- Set LCD Vop (Contrast)
    this->write(PCD8544_DC_COMMAND, PCD8544_TEMP_COEFFICIENT(0x03)); // <-- Set Temp coefficient
    this->write(PCD8544_DC_COMMAND, PCD8544_BIAS(0x04));             // <-- Set LCD bias mode 1:48 (can try 0x03)

    this->write(PCD8544_DC_COMMAND, PCD8544_FUNCTION_SET(0, 0, 1));  // <-- Tell LCD normal commands follow
    this->write(PCD8544_DC_COMMAND, PCD8544_DISPLAY_CONTROL(1, 0));  // <-- Set display normal mode, not inverse
}
