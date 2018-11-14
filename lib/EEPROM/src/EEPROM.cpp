#include "EEPROM.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/eeprom.h>
#include <util/atomic.h>
#include <stdint.h>

//TODO write buffer item: [address, byte] or maybe [address, length, byte, byte, ...]

typedef struct {
    uint16_t address;
    uint8_t data[4];
    uint8_t length;
} EEPROM_WRITE_FRAME;

static EEPROM_WRITE_FRAME writeBufferData[8];
static volatile uint8_t writeBufferIndex;
static volatile uint8_t writeBufferLength;

EEPROMClass EEPROM;

ISR(EE_RDY_vect){}

void EEPROMClass::read(uint16_t address, uint8_t *value) {
    // Wait until previous write completed
    while(EECR & _BV(EEWE));

    EEAR  = address;
    EECR |= _BV(EERE);

    *value = EEDR;
}

void EEPROMClass::read(uint16_t address, uint16_t *value) {
    this->read(address, (uint8_t *) value, 2);
}

void EEPROMClass::read(uint16_t address, uint32_t *value) {
    this->read(address, (uint8_t *) value, 4);
}

void EEPROMClass::read(uint16_t address, float *value) {
    this->read(address, (uint8_t *) value, 4);
}

void EEPROMClass::read(uint16_t address, uint8_t *data, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        this->read(address, data + i);
    }
}

void EEPROMClass::write(uint16_t address, uint8_t *value) {
    this->write(address, (uint8_t *) value, 1);
}

void EEPROMClass::write(uint16_t address, uint16_t *value) {
    this->write(address, (uint8_t *) value, 2);
}

void EEPROMClass::write(uint16_t address, uint32_t *value) {
    this->write(address, (uint8_t *) value, 4);
}

void EEPROMClass::write(uint16_t address, float *value) {
    this->write(address, (uint8_t *) value, 4);
}

void EEPROMClass::write(uint16_t address, uint8_t *data, uint8_t length) {
    //TODO check length
    EEPROM_WRITE_FRAME frame = {.address = address, .length = 2};

    for (uint8_t i = 0; i < length; i++) {
        frame.data[i] = *(data + i);
    }

    writeBufferData[writeBufferIndex] = frame;

    writeBufferIndex++;
    writeBufferLength++;
}

void EEPROMClass::start() {
    writeBufferIndex  = 0;
    writeBufferLength = 0;
}

void EEPROMClass::flush() {
    if (writeBufferLength > 0) {
        EEAR = writeBufferData[writeBufferIndex].address;
        EEDR = writeBufferData[writeBufferIndex].data[0];

        EECR |= _BV(EEMWE); //<-- EEMWE must be set separately before set EEWE
        EECR |= _BV(EEWE);
    }
}

void EEPROMClass::setOnWriteCompleteHandler(void (*handler_ptr)()) {
    _onWriteCompleteHandler = handler_ptr;
}

