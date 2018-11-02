#include "EEPROM.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/eeprom.h>
#include <stdint.h>

//TODO write buffer item: [address, byte] or maybe [address, length, byte, byte, ...]

typedef struct {
    uint16_t address;
    uint8_t *data;
    uint8_t length;
} EEPROM_WRITE_FRAME;

static EEPROM_WRITE_FRAME frames[1];

EEPROMClass EEPROM;

ISR(EE_RDY_vect){
    //TODO write eeprom value to buffer byte by byte, use our own ready flag for check is ready to read/write
}

