#ifndef EEPROM_H
#define EEPROM_H

#include <avr/interrupt.h>
#include <stdint.h>

#ifndef EEMEM
#define EEMEM __attribute__((section(".eeprom")))
#endif

#ifndef EEPROM_WRITE_BUFFER_LENGTH
#define EEPROM_WRITE_BUFFER_LENGTH 8
#endif

ISR(EE_RDY_vect);

class EEPROMClass
{
    friend void EE_RDY_vect();
private:
    void (*_onWriteCompleteHandler) ();
    /**
     * @param address
     * @param data
     * @param length
     */
    void read(uint16_t address, uint8_t *data, uint8_t length);//TODO maybe move outside of class

    /**
     * @param address
     * @param data
     * @param length
     */
    void write(uint16_t address, uint8_t *data, uint8_t length);//TODO maybe move outside of class
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
     * @param value
     */
    void write(uint16_t address, uint8_t *value);

    /**
     * @param address
     * @param value
     */
    void write(uint16_t address, uint16_t *value);

    /**
     * @param address
     * @param value
     */
    void write(uint16_t address, uint32_t *value);

    /**
     * @param address
     * @param value
     */
    void write(uint16_t address, float *value);

    /**
     * Reset write buffer before write
     */
    void start();

    /**
     * Write buffer to EEPROM
     */
    void flush();

    /**
     * @param handler_ptr
     */
    void setOnWriteCompleteHandler(void (*handler_ptr) ());
};

extern EEPROMClass EEPROM;

#endif //EEPROM_H
