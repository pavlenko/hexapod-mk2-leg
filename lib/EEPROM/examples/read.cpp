#include <EEPROM.h>
#include <stdint.h>

#define EEPROM_UINT8_ADDR  0x00
#define EEPROM_UINT16_ADDR 0x01
#define EEPROM_UINT32_ADDR 0x02
#define EEPROM_FLOAT_ADDR  0x03

int main() {
    uint8_t value1;
    EEPROM.read(EEPROM_UINT8_ADDR, &value1);

    uint16_t value2;
    EEPROM.read(EEPROM_UINT16_ADDR, &value2);

    uint32_t value3;
    EEPROM.read(EEPROM_UINT32_ADDR, &value3);

    float value4;
    EEPROM.read(EEPROM_FLOAT_ADDR, &value4);
}

