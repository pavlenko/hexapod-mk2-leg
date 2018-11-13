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
    //TODO rewrite methods to use simple uint16_t address instead of pointers to emem
    uint8_t read(const uint8_t *__p);

    uint16_t read(const uint16_t *__p);

    uint32_t read(const uint32_t *__p);

    float read(const float *__p);

    void read(void *__dst, const void *__src, uint8_t __n);

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
