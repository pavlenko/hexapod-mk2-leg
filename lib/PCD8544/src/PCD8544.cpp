#include "PCD8544.h"

#include <SPI.h>

void PCD8544Class::initialize(PCD8544Pin_t reset, PCD8544Pin_t dc, SPISlaveSelect_t ss) {
    _reset = reset;
    _dc    = dc;
    _ss    = ss;

    // Configure non-SPI bus pins
    *(_reset.DDR) |= _BV(_reset.PIN);
    *(_dc.DDR)    |= _BV(_dc.PIN);

    // Configure SPI bus
    SPI.setDataMode(SPI_MODE_SAMPLE_RISING_NORMAL);
    SPI.setDataOrder(SPI_DATA_ORDER_MSB_FIRST);
    SPI.setBusMode(SPI_BUS_MASTER);

    // Connect to LCD
    SPI.start(_ss);

    // Reset the LCD to a known state
    *(_reset.PORT) &= ~_BV(_reset.PIN);
    *(_reset.PORT) |= _BV(_reset.PIN);

    this->write(PCD8544_DC_COMMAND, PCD8544_FUNCTION_SET(0, 0, 1));  // <-- Tell LCD extended commands follow
    this->write(PCD8544_DC_COMMAND, PCD8544_VOP(0x30));              // <-- Set LCD Vop (Contrast)
    this->write(PCD8544_DC_COMMAND, PCD8544_TEMP_COEFFICIENT(0x03)); // <-- Set Temp coefficient
    this->write(PCD8544_DC_COMMAND, PCD8544_BIAS(0x04));             // <-- Set LCD bias mode 1:48 (can try 0x03)

    this->write(PCD8544_DC_COMMAND, PCD8544_FUNCTION_SET(0, 0, 0));  // <-- Tell LCD normal commands follow
    this->write(PCD8544_DC_COMMAND, PCD8544_DISPLAY_CONTROL(1, 0));  // <-- Set display normal mode, not inverse
}

void PCD8544Class::write(PCD8544_DC dc, uint8_t data) {
    // Tell the LCD that we are writing either to data or a command
    if (PCD8544_DC_DATA == dc) {
        *(_dc.PORT) |= _BV(_dc.PIN);
    } else {
        *(_dc.PORT) &= ~_BV(_dc.PIN);
    }

    //*(_ss.PORT) &= ~_BV(_ss.SS);//TODO are this need ???

    SPI.transfer(data);

    //*(_ss.PORT) |= _BV(_ss.SS);//TODO are this need ???
}

PCD8544Class PCD8544;
