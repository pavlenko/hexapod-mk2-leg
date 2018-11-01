#include "TWI.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/twi.h>

#define TWI_SEND_START() (TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE) | _BV(TWSTA))
#define TWI_SEND_STOP()  (TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE) | _BV(TWSTO))
#define TWI_SEND_ACK()   (TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE) | _BV(TWEA))
#define TWI_SEND_NACK()  (TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE))
//#define TWISendTransmit() (TWCR = _BV(1<<TWINT)|_BV(1<<TWEN)|_BV(1<<TWIE))

static volatile uint8_t state;
static volatile uint8_t error;

static void (*onTransmitHandler) ();
static void (*onReceiveHandler) (uint8_t *data, uint8_t length);
static void (*onRequestHandler) ();

static uint8_t txBufferData[TWI_BUFFER_LENGTH];
static volatile uint8_t txBufferIndex;
static volatile uint8_t txBufferLength;

static uint8_t rxBufferData[TWI_BUFFER_LENGTH];
static volatile uint8_t rxBufferIndex;

void TWIClass::setAddress(uint8_t address) {
    TWAR = address << 1;
}

void TWIClass::receive(uint8_t address, uint8_t length) {
    //TODO
}

void TWIClass::transmit(uint8_t address, uint8_t *data, uint8_t length) {
    //TODO
}

void TWIClass::setOnTransmitHandler(void (*handler_ptr) ()) {
    onTransmitHandler = handler_ptr;
}

void TWIClass::setOnReceiveHandler(void (*handler_ptr) (uint8_t *, uint8_t)) {
    onReceiveHandler = handler_ptr;
}

void TWIClass::setOnRequestHandler(void (*handler_ptr) ()) {
    onRequestHandler = handler_ptr;
}

TWIClass TWI;

ISR(TWI_vect)
{
    switch(TW_STATUS) {
        // All Master
        case TW_START:     // sent start condition
        case TW_REP_START: // sent repeated start condition
            // copy device address and r/w bit to output register and ack
            TWDR = twi_slarw;
            TWI_SEND_ACK();
            break;

#if TWI_ENABLE_MASTER == 1
            // Master Transmitter
        case TW_MT_SLA_ACK:  // slave receiver acked address
        case TW_MT_DATA_ACK: // slave receiver acked data
            // if there is data to send, send it, otherwise stop
            if (twi_masterBufferIndex < twi_masterBufferLength) {
                // copy data to output register and ack
                TWDR = twi_masterBuffer[twi_masterBufferIndex++];
                TWI_SEND_ACK();
            } else {
                if (twi_sendStop) {
                    TWI_SEND_STOP();
                    state = TWI_STATE_READY;
                } else {
                    twi_inRepStart = true;    // we're gonna send the START
                    // don't enable the interrupt. We'll generate the start, but we
                    // avoid handling the interrupt until we're in the next transaction,
                    // at the point where we would normally issue the start.
                    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
                    state = TWI_STATE_READY;
                }
            }
            break;
        case TW_MT_SLA_NACK:  // address sent, nack received
            error = TW_MT_SLA_NACK;
            TWI_SEND_STOP();
            state = TWI_STATE_READY;
            break;
        case TW_MT_DATA_NACK: // data sent, nack received
            error = TW_MT_DATA_NACK;
            TWI_SEND_STOP();
            state = TWI_STATE_READY;
            break;
        case TW_MT_ARB_LOST: // lost bus arbitration
            error = TW_MT_ARB_LOST;
            TWI_SEND_ACK();
            state = TWI_STATE_READY;
            break;

            // Master Receiver
        case TW_MR_DATA_ACK: // data received, ack sent
            // put byte into buffer
            twi_masterBuffer[twi_masterBufferIndex++] = TWDR;
        case TW_MR_SLA_ACK:  // address sent, ack received
            // ack if more bytes are expected, otherwise nack
            if (twi_masterBufferIndex < twi_masterBufferLength) {
                TWI_SEND_ACK();
            } else {
                TWI_SEND_NACK();
            }
            break;
        case TW_MR_DATA_NACK: // data received, nack sent
            // put final byte into buffer
            twi_masterBuffer[twi_masterBufferIndex++] = TWDR;
            if (twi_sendStop) {
                TWI_SEND_STOP();
                state = TWI_STATE_READY;
            } else {
                twi_inRepStart = true;	// we're gonna send the START
                // don't enable the interrupt. We'll generate the start, but we
                // avoid handling the interrupt until we're in the next transaction,
                // at the point where we would normally issue the start.
                TWCR = _BV(TWINT) | _BV(TWSTA)| _BV(TWEN) ;
                state = TWI_STATE_READY;
            }
            break;
        case TW_MR_SLA_NACK: // address sent, nack received
            TWI_SEND_STOP();
            state = TWI_STATE_READY;
            break;
            // TW_MR_ARB_LOST handled by TW_MT_ARB_LOST case
#endif

#if TWI_ENABLE_SLAVE == 1
        // Slave Receiver
        case TW_SR_SLA_ACK:   // addressed, returned ack
        case TW_SR_GCALL_ACK: // addressed generally, returned ack
        case TW_SR_ARB_LOST_SLA_ACK:   // lost arbitration, returned ack
        case TW_SR_ARB_LOST_GCALL_ACK: // lost arbitration, returned ack
            state = TWI_STATE_SLAVE_RX;
            // indicate that rx buffer can be overwritten and ack
            rxBufferIndex = 0;
            TWI_SEND_ACK();
            break;
        case TW_SR_DATA_ACK:       // data received, returned ack
        case TW_SR_GCALL_DATA_ACK: // data received generally, returned ack
            // if there is still room in the rx buffer
            if (rxBufferIndex < TWI_BUFFER_LENGTH){
                // put byte in buffer and ack
                rxBufferData[rxBufferIndex++] = TWDR;
                TWI_SEND_ACK();
            } else {
                // otherwise nack
                TWI_SEND_NACK();
            }
            break;
        case TW_SR_STOP: // stop or repeated start condition received
            // ack future responses and leave slave receiver state
            TWI_SEND_ACK();
            state = TWI_STATE_READY;

            // put a null char after data if there's room
            if (rxBufferIndex < TWI_BUFFER_LENGTH) {
                rxBufferData[rxBufferIndex] = '\0';//TODO check if this is needed
            }
            // callback to user defined callback
            if (onReceiveHandler) {
                onReceiveHandler(rxBufferData, rxBufferIndex);
            }

            rxBufferIndex = 0;
            break;
        case TW_SR_DATA_NACK:       // data received, returned nack
        case TW_SR_GCALL_DATA_NACK: // data received generally, returned nack
            // nack back at master
            TWI_SEND_NACK();
            break;

        // Slave Transmitter
        case TW_ST_SLA_ACK:          // addressed, returned ack
        case TW_ST_ARB_LOST_SLA_ACK: // arbitration lost, returned ack
            state = TWI_STATE_SLAVE_TX;

            txBufferIndex  = 0;
            txBufferLength = 0;

            if (onRequestHandler) {
                // Call user function to fill tx buffer (via TWI.transmit(data, length))
                onRequestHandler();
            }

            if (0 == txBufferLength) {
                // If buffer empty - fill empty response as fallback
                txBufferLength = 1;
                txBufferData[0] = 0x00;
            }
        case TW_ST_DATA_ACK: // byte sent, ack returned
            TWDR = txBufferData[txBufferIndex++];
            // if there is more to send, ack, otherwise nack
            if (txBufferIndex < txBufferLength){
                TWI_SEND_ACK();
            } else {
                TWI_SEND_NACK();
            }
            break;
        case TW_ST_DATA_NACK: // received nack, we are done
        case TW_ST_LAST_DATA: // received ack, but we are done already!
            // ack future responses
            TWI_SEND_ACK();
            state = TWI_STATE_READY;
            break;
#endif

        // All
        case TW_NO_INFO:   // no state information
            break;
        case TW_BUS_ERROR: // bus error, illegal stop/start
            error = TW_BUS_ERROR;
            TWI_SEND_STOP();
            state = TWI_STATE_READY;
            break;
    }
}