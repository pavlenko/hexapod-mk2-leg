#ifndef TWI_H
#define TWI_H

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
     * Set handler for slave receive data completed event
     *
     * @param handler_ptr
     */
    void setOnReceiveHandler(void (*handler_ptr) ());

    /**
     * Set handler for slave transmit data requested event
     *
     * @param handler_ptr
     */
    void setOnRequestHandler(void (*handler_ptr) ());
};

extern TWIClass TWI;

#endif //TWI_H
