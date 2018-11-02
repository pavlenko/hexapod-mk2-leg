#include <EEPROM.h>
#include <stdint.h>

static EEMEM uint8_t value1_ptr;

int main() {
    uint8_t value1 = EEPROM.read(&value1_ptr);
}

