#include "SPI.h"

#include <avr/io.h>
#include <stdint.h>

void SPIClass::initialize(SPIConnection_t connection) {
    this->connection = connection;
}

void SPIClass::setBusMode(SPIBusMode mode) {
    //uint8_t mask = (uint8_t) ~(_BV(connection.SS) | _BV(connection.SCK) | _BV(connection.MOSI) | _BV(connection.MISO));

    if (SPI_BUS_MASTER == mode) {
        *(connection.DDR) |= _BV(connection.MOSI) | _BV(connection.SCK);
    } else {
        //*(connection.DDR) = (uint8_t) ((*(connection.DDR) & mask) | _BV(connection.MISO));
    }

    SPCR = (uint8_t) ((SPCR & ~_BV(MSTR)) | (mode << MSTR));
}

void SPIClass::setClockSource(SPIClockSource clockSource, bool x2) {
    SPCR = (uint8_t) ((SPCR & 0b11111100) | clockSource);
    SPSR = (uint8_t) ((SPSR & 0b11111110) | x2);
}

void SPIClass::setDataOrder(SPIDataOrder dataOrder) {
    SPCR = (uint8_t) ((SPCR & ~_BV(DORD)) | (dataOrder << DORD));
}

void SPIClass::setDataMode(SPIDataMode dataMode) {
    SPCR = (uint8_t) ((SPCR & 0b11110011) | (dataMode << CPHA));
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

SPIClass SPI;