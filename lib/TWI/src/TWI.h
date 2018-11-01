#ifndef TWI_H
#define TWI_H

#include <stdint.h>

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

class TWIClass
{
public:
    /**
     * Set slave address
     *
     * @param address
     */
    void setAddress(uint8_t address);

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
     * Send data to slave
     *
     * TODO maybe use on transmit handler for non-block send complete callback
     *
     * @param address
     * @param data
     * @param length
     */
    void transmit(uint8_t address, uint8_t *data, uint8_t length);

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
