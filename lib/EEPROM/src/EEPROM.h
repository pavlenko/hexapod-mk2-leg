#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

#ifndef EEMEM
#define EEMEM __attribute__((section(".eeprom")))
#endif

class EEPROMClass
{
private:
    void (*_onWriteCompleteHandler) ();
public:
    /**
     * @param address
     * @param value
     */
    void read(uint16_t address, uint8_t *value);

    /**
     * @param address
     * @param value
     */
    void read(uint16_t address, uint16_t *value);

    /**
     * @param address
     * @param value
     */
    void read(uint16_t address, uint32_t *value);

    /**
     * @param address
     * @param value
     */
    void read(uint16_t address, float *value);

    /**
     * @param address
     * @param data
     * @param length
     */
    void read(uint16_t address, uint8_t *data, uint8_t length);

    void write(uint8_t *__p, uint8_t __value);

    void write(uint16_t *__p, uint16_t __value);

    void write(uint32_t *__p, uint32_t __value);

    void write(float *__p, float __value);

    void write(const void *__src, void *__dst, uint8_t __n);

    void flush();

    /**
     * @param handler_ptr
     */
    void setOnWriteCompleteHandler(void (*handler_ptr) ());
};

extern EEPROMClass EEPROM;

#endif //EEPROM_H
