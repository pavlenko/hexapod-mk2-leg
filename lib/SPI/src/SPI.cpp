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

void SPIClass::start(SPISlaveSelect_t slaveSelect) {
    // Save slave reference for disconnect after
    this->slaveSelect = slaveSelect;

    // Set pin as output
    *(this->slaveSelect.DDR) |= _BV(this->slaveSelect.SS);

    // Set pin low to select slave
    *(this->slaveSelect.PORT) &= ~_BV(this->slaveSelect.SS);

    // Enable SPI
    SPCR |= _BV(SPE);
}

uint8_t SPIClass::transfer(uint8_t data) {
    // Store data to transmit buffer
    SPDR = data;

    /*
     * The following NOP introduces a small delay that can prevent the wait loop form iterating when running at the
     * maximum speed.
     * This gives about 10% more speed, even if it seems counter-intuitive.
     * At lower speeds it is unnoticed.
     */
    asm volatile("nop");

    // Wait for transfer completed
    while (!(SPSR & _BV(SPIF)));

    // Return received data
    return SPDR;
}

void SPIClass::end() {
    // Disable SPI
    SPCR &= ~_BV(SPE);

    // Set pin high to release slave
    *(this->slaveSelect.PORT) |= _BV(this->slaveSelect.SS);
}