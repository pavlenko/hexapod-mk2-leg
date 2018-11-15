#include "EEPROM.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/eeprom.h>
#include <util/atomic.h>
#include <stdint.h>

typedef struct {
    uint16_t address;
    uint8_t data[4];
    uint8_t index: 4;
    uint8_t length: 4;
} EEPROMWriteFrame_t;

static volatile bool async = false;

static EEPROMWriteFrame_t writeBufferData[EEPROM_WRITE_BUFFER_LENGTH];
static volatile uint8_t writeBufferIndex;
static volatile uint8_t writeBufferLength;

static inline void writeFrame(EEPROMWriteFrame_t *frame) {
    EEAR = (*frame).address + (*frame).index;
    EEDR = (*frame).data[(*frame).index];

    (*frame).index++;

    EECR |= _BV(EEMWE); //<-- EEMWE must be set separately before set EEWE
    EECR |= _BV(EEWE) | _BV(EERIE);
}

static inline void _read(uint16_t address, uint8_t *data, uint8_t length) {
    while (writeBufferLength > 0 && (EECR & _BV(EEWE)));

    for (uint8_t i = 0; i < length; i++) {
        EEAR  = address++;
        EECR |= _BV(EERE);

        *(data++) = EEDR;
    }
}

static inline void _write(uint16_t address, uint8_t *data, uint8_t length) {
    if (async) {
        for (uint8_t i = 0; i < length; i++) {
            while (EECR & _BV(EEWE));

            EEAR = address++;
            EEDR = *(data + i);

            EECR |= _BV(EEMWE); //<-- EEMWE must be set separately before set EEWE
            EECR |= _BV(EEWE);
        }
    } else if (writeBufferLength < EEPROM_WRITE_BUFFER_LENGTH) {
        EEPROMWriteFrame_t frame = {
                .address = address,
                .index   = 0,
                .length  = length,
        };

        for (uint8_t i = 0; i < length; i++) {
            frame.data[i] = *(data++);
        }

        writeBufferData[writeBufferLength++] = frame;
    }
}

ISR(EE_RDY_vect){
    if (writeBufferIndex < writeBufferLength) {
        // Check if frame written -> write next frame
        if (writeBufferData[writeBufferIndex].index == writeBufferData[writeBufferIndex].length) {
            writeBufferIndex++;
        }

        writeFrame(&writeBufferData[writeBufferIndex]);
    } else if (EEPROM._onWriteCompleteHandler) {
        async = false;

        writeBufferIndex  = 0;
        writeBufferLength = 0;

        EEPROM._onWriteCompleteHandler();
    }
}

void EEPROMClass::read(uint16_t address, uint8_t *value) {
    _read(address, value, 1);
}

void EEPROMClass::read(uint16_t address, uint16_t *value) {
    _read(address, (uint8_t *) value, 2);
}

void EEPROMClass::read(uint16_t address, uint32_t *value) {
    _read(address, (uint8_t *) value, 4);
}

void EEPROMClass::read(uint16_t address, float *value) {
    _read(address, (uint8_t *) value, 4);
}

void EEPROMClass::write(uint16_t address, uint8_t *value) {
    _write(address, value, 1);
}

void EEPROMClass::write(uint16_t address, uint16_t *value) {
    _write(address, (uint8_t *) value, 2);
}

void EEPROMClass::write(uint16_t address, uint32_t *value) {
    _write(address, (uint8_t *) value, 4);
}

void EEPROMClass::write(uint16_t address, float *value) {
    _write(address, (uint8_t *) value, 4);
}

void EEPROMClass::start() {
    async = true;

    writeBufferIndex  = 0;
    writeBufferLength = 0;
}

void EEPROMClass::flush() {
    if (writeBufferLength > 0) {
        writeFrame(&writeBufferData[writeBufferIndex]);
    }
}

void EEPROMClass::setOnWriteCompleteHandler(void (*handler_ptr)()) {
    _onWriteCompleteHandler = handler_ptr;
}

EEPROMClass EEPROM;