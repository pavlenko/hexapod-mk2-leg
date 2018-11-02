#ifndef TWI_H
#define TWI_H

#include <stdint.h>
#include <stddef.h>

#ifndef TWI_DEFAULT_FREQUENCY
#define TWI_DEFAULT_FREQUENCY 100000UL
#endif

#ifndef TWI_BUFFER_LENGTH
#define TWI_BUFFER_LENGTH 32
#endif

#ifndef TWI_ENABLE_MASTER
#define TWI_ENABLE_MASTER 1
#endif

#ifndef TWI_ENABLE_SLAVE
#define TWI_ENABLE_SLAVE 1
#endif

#define TWI_STATE_READY     0
#define TWI_STATE_MASTER_RX 1
#define TWI_STATE_MASTER_TX 2
#define TWI_STATE_SLAVE_RX  3
#define TWI_STATE_SLAVE_TX  4

#define TWI_ERROR_NONE  0xFF
#define TWI_ERROR_WRITE 0xF1
#define TWI_ERROR_READ  0xF2

enum TWIPrescaller {
    TWI_PRESCALLER_NONE,
    TWI_PRESCALLER_DIVIDE_BY_4,
    TWI_PRESCALLER_DIVIDE_BY_16,
    TWI_PRESCALLER_DIVIDE_BY_64,
};

class TWIClass
{
public:
    /**
     * Enable TWI module
     */
    void enable();

    /**
     * Disable TWI module
     */
    void disable();

    /**
     * Set slave address
     *
     * @param address
     */
    void setAddress(uint8_t address);

    /**
     * Set prescaler value
     *
     * @param value
     */
    void setPrescaler(TWIPrescaller value);

    /**
     * Set TWI frequency to update bit-rate
     *
     * @param frequency
     */
    void setFrequency(uint32_t frequency);

    /**
     * Read value from TX buffer to pointer
     *
     * @param value
     */
    void read(uint8_t *value);

    /**
     * Read value from TX buffer to pointer
     *
     * @param value
     */
    void read(uint16_t *value);

    /**
     * Read value from TX buffer to pointer
     *
     * @param value
     */
    void read(uint32_t *value);

    /**
     * Read value from TX buffer to pointer
     *
     * @param value
     */
    void read(float *value);

    /**
     * Read array from TX buffer to pointer
     *
     * @param value
     */
    void read(uint8_t *data, uint8_t length);

    /**
     * Write value to TX buffer
     *
     * @param value
     */
    void write(uint8_t value);

    /**
     * Write value to TX buffer
     *
     * @param value
     */
    void write(uint16_t value);

    /**
     * Write value to TX buffer
     *
     * @param value
     */
    void write(uint32_t value);

    /**
     * Write value to TX buffer
     *
     * @param value
     */
    void write(float value);

    /**
     * Write values array to TX buffer
     *
     * @param data
     * @param length
     */
    void write(uint8_t *data, uint8_t length);

    /**
     * Receive data from slave
     *
     * TODO maybe use on receive handler for non-block read
     *
     * @param address
     * @param length
     */
    void receive(uint8_t address, uint8_t length);

    /**
     * Prepare transmitting
     */
    void start();

    /**
     * Transmit data to slave
     *
     * @param address
     */
    void transmit(uint8_t address);

    /**
     * Set handler for master transmit data completed event
     *
     * @param handler_ptr
     */
    void setOnTransmitHandler(void (*handler_ptr) ());

    /**
     * Set handler for slave receive data completed event
     *
     * @param handler_ptr
     */
    void setOnReceiveHandler(void (*handler_ptr) (uint8_t *data, uint8_t length));

    /**
     * Set handler for slave transmit data requested event
     *
     * @param handler_ptr
     */
    void setOnRequestHandler(void (*handler_ptr) ());
};

extern TWIClass TWI;

#endif //TWI_H
