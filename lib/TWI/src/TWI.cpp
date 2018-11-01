#include "TWI.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/twi.h>

static volatile uint8_t state;
static volatile uint8_t error;

static void (*onReceiveHandler) ();
static void (*onRequestHandler) ();

void TWIClass::setOnReceiveHandler(void (*handler_ptr) ()) {
    onReceiveHandler = handler_ptr;
}

void TWIClass::setOnRequestHandler(void (*handler_ptr) ()) {
    onRequestHandler = handler_ptr;
}

TWIClass TWI;

ISR(TWI_vect)
{
    switch(TW_STATUS){
        // All Master
        case TW_START:     // sent start condition
        case TW_REP_START: // sent repeated start condition
            // copy device address and r/w bit to output register and ack
            TWDR = twi_slarw;
            twi_reply(1);
            break;

#if TWI_ENABLE_MASTER == 1
        // Master Transmitter
        case TW_MT_SLA_ACK:  // slave receiver acked address
        case TW_MT_DATA_ACK: // slave receiver acked data
            // if there is data to send, send it, otherwise stop
            if(twi_masterBufferIndex < twi_masterBufferLength){
                // copy data to output register and ack
                TWDR = twi_masterBuffer[twi_masterBufferIndex++];
                twi_reply(1);
            }else{
                if (twi_sendStop)
                    twi_stop();
                else {
                    twi_inRepStart = true;	// we're gonna send the START
                    // don't enable the interrupt. We'll generate the start, but we
                    // avoid handling the interrupt until we're in the next transaction,
                    // at the point where we would normally issue the start.
                    TWCR = _BV(TWINT) | _BV(TWSTA)| _BV(TWEN) ;
                    state = TWI_STATE_READY;
                }
            }
            break;
        case TW_MT_SLA_NACK:  // address sent, nack received
            error = TW_MT_SLA_NACK;
            twi_stop();
            break;
        case TW_MT_DATA_NACK: // data sent, nack received
            error = TW_MT_DATA_NACK;
            twi_stop();
            break;
        case TW_MT_ARB_LOST: // lost bus arbitration
            error = TW_MT_ARB_LOST;
            twi_releaseBus();
            break;

        // Master Receiver
        case TW_MR_DATA_ACK: // data received, ack sent
            // put byte into buffer
            twi_masterBuffer[twi_masterBufferIndex++] = TWDR;
        case TW_MR_SLA_ACK:  // address sent, ack received
            // ack if more bytes are expected, otherwise nack
            if(twi_masterBufferIndex < twi_masterBufferLength){
                twi_reply(1);
            }else{
                twi_reply(0);
            }
            break;
        case TW_MR_DATA_NACK: // data received, nack sent
            // put final byte into buffer
            twi_masterBuffer[twi_masterBufferIndex++] = TWDR;
            if (twi_sendStop)
                twi_stop();
            else {
                twi_inRepStart = true;	// we're gonna send the START
                // don't enable the interrupt. We'll generate the start, but we
                // avoid handling the interrupt until we're in the next transaction,
                // at the point where we would normally issue the start.
                TWCR = _BV(TWINT) | _BV(TWSTA)| _BV(TWEN) ;
                state = TWI_STATE_READY;
            }
            break;
        case TW_MR_SLA_NACK: // address sent, nack received
            twi_stop();
            break;
            // TW_MR_ARB_LOST handled by TW_MT_ARB_LOST case
#endif

#if TWI_ENABLE_SLAVE == 1
        // Slave Receiver
        case TW_SR_SLA_ACK:   // addressed, returned ack
        case TW_SR_GCALL_ACK: // addressed generally, returned ack
        case TW_SR_ARB_LOST_SLA_ACK:   // lost arbitration, returned ack
        case TW_SR_ARB_LOST_GCALL_ACK: // lost arbitration, returned ack
            // enter slave receiver mode
            state = TWI_STATE_SLAVE_RX;
            // indicate that rx buffer can be overwritten and ack
            twi_rxBufferIndex = 0;
            twi_reply(1);
            break;
        case TW_SR_DATA_ACK:       // data received, returned ack
        case TW_SR_GCALL_DATA_ACK: // data received generally, returned ack
            // if there is still room in the rx buffer
            if(twi_rxBufferIndex < TWI_BUFFER_LENGTH){
                // put byte in buffer and ack
                twi_rxBuffer[twi_rxBufferIndex++] = TWDR;
                twi_reply(1);
            }else{
                // otherwise nack
                twi_reply(0);
            }
            break;
        case TW_SR_STOP: // stop or repeated start condition received
            // ack future responses and leave slave receiver state
            twi_releaseBus();
            // put a null char after data if there's room
            if(twi_rxBufferIndex < TWI_BUFFER_LENGTH){
                twi_rxBuffer[twi_rxBufferIndex] = '\0';
            }
            // callback to user defined callback
            if (onReceiveHandler) {
                onReceiveHandler(twi_rxBuffer, twi_rxBufferIndex);
            }
            // since we submit rx buffer to "wire" library, we can reset it
            twi_rxBufferIndex = 0;
            break;
        case TW_SR_DATA_NACK:       // data received, returned nack
        case TW_SR_GCALL_DATA_NACK: // data received generally, returned nack
            // nack back at master
            twi_reply(0);
            break;

        // Slave Transmitter
        case TW_ST_SLA_ACK:          // addressed, returned ack
        case TW_ST_ARB_LOST_SLA_ACK: // arbitration lost, returned ack
            // enter slave transmitter mode
            state = TWI_STATE_SLAVE_TX;
            // ready the tx buffer index for iteration
            twi_txBufferIndex = 0;
            // set tx buffer length to be zero, to verify if user changes it
            twi_txBufferLength = 0;
            // request for txBuffer to be filled and length to be set
            // note: user must call twi_transmit(bytes, length) to do this
            if (onRequestHandler) {
                onRequestHandler();
            }
            // if they didn't change buffer & length, initialize it
            if(0 == twi_txBufferLength){
                twi_txBufferLength = 1;
                twi_txBuffer[0] = 0x00;
            }
            // transmit first byte from buffer, fall
        case TW_ST_DATA_ACK: // byte sent, ack returned
            // copy data to output register
            TWDR = twi_txBuffer[twi_txBufferIndex++];
            // if there is more to send, ack, otherwise nack
            if(twi_txBufferIndex < twi_txBufferLength){
                twi_reply(1);
            }else{
                twi_reply(0);
            }
            break;
        case TW_ST_DATA_NACK: // received nack, we are done
        case TW_ST_LAST_DATA: // received ack, but we are done already!
            // ack future responses
            twi_reply(1);
            // leave slave receiver state
            state = TWI_STATE_READY;
            break;
#endif

        // All
        case TW_NO_INFO:   // no state information
            break;
        case TW_BUS_ERROR: // bus error, illegal stop/start
            error = TW_BUS_ERROR;
            twi_stop();
            break;
    }
}