#include "SPI.h"

#include <avr/io.h>
#include <stdint.h>

void SPIClass::initialize(SPIConnection_t connection, SPISettings_t settings) {
    // Configure SPI pins for master mode as output
    *(connection.DDR) |= _BV(connection.MOSI) | _BV(connection.SCK);

    // Configure SPI settings
    SPCR = (settings.dataOrder << DORD) | (settings.clockMode << CPHA) | (settings.clockSource << SPR0);

    // Set SPI master mode
    SPCR |= _BV(MSTR);
}

void SPIClass::initialize(SPIConnection_t connection) {
    // Configure SPI pins for slave mode
    *(connection.DDR) |= _BV(connection.MISO);

    // Set SPI master mode
    SPCR &= ~_BV(MSTR);
}
