// If you want to override EEPROM write buffer length you can define size before include header
#define EEPROM_WRITE_BUFFER_LENGTH 16

#include <EEPROM.h>
#include <stdint.h>

#define EEPROM_UINT8_ADDR  0x00
#define EEPROM_UINT16_ADDR 0x01
#define EEPROM_UINT32_ADDR 0x02
#define EEPROM_FLOAT_ADDR  0x03

void onWriteCompleted() {
    //TODO do something after EEPROM buffer written
}

int main() {
    EEPROM.start();// <-- Reset write buffer

    uint8_t value1 = 1;
    EEPROM.write(EEPROM_UINT8_ADDR, &value1);

    uint16_t value2 = 2;
    EEPROM.write(EEPROM_UINT16_ADDR, &value2);

    uint32_t value3 = 3;
    EEPROM.write(EEPROM_UINT32_ADDR, &value3);

    float value4 = 4;
    EEPROM.write(EEPROM_FLOAT_ADDR, &value4);

    EEPROM.setOnWriteCompleteHandler(onWriteCompleted);
    EEPROM.flush();// Write buffer to EEPROM in background
}

