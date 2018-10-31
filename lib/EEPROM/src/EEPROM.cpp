#include "EEPROM.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdint.h>

//TODO how to use interrupts to read/write operations
uint8_t EEPROMClass::read(const uint8_t *address) {
    // Wait until write completed
    while (EECR & _BV(EEWE));

    // Set address to read
    EEAR = *address;

    // Start read
    EECR |= _BV(EERE);

    // Return value
    return EEDR;
}

EEPROMClass EEPROM;

ISR(EE_RDY_vect){
    //TODO write eeprom value to buffer byte by byte, use our own ready flag for check is ready to read/write
}

