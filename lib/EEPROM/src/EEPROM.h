#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

class EEPROMClass
{
public:
    uint8_t read(const uint8_t *address);
};

extern EEPROMClass EEPROM;

#endif //EEPROM_H
