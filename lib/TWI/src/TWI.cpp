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
static volatile uint8_t addressAndRW;
static volatile uint8_t isRepeatedStart;
static volatile uint8_t sendStop;

static void (*onTransmitHandler) ();
static void (*onReceiveHandler) ();
static void (*onRequestHandler) ();

static uint8_t txBufferData[TWI_BUFFER_LENGTH];
static volatile uint8_t txBufferIndex;
static volatile uint8_t txBufferLength;

static uint8_t rxBufferData[TWI_BUFFER_LENGTH];
static volatile uint8_t rxBufferIndex;
static volatile uint8_t rxBufferLength;

void TWIClass::enable() {
    state = TWI_STATE_READY;
    sendStop = 1;
    isRepeatedStart = 0;

    //TODO activate internal pull-up resistors OR use external and don't bother about that

    // Set default prescaler value
    this->setPrescaler(TWI_PRESCALLER_NONE);

    // Set default frequency
    this->setFrequency(TWI_DEFAULT_FREQUENCY);

    // Enable twi module, acks, and twi interrupt
    TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
}

void TWIClass::disable() {
    // Disable twi module, acks, and twi interrupt
    TWCR &= ~(_BV(TWEN) | _BV(TWIE) | _BV(TWEA));

    //TODO de-activate internal pull-up resistors OR use external and don't bother about that
}

void TWIClass::setAddress(uint8_t address) {
    TWAR = address << 1;
}

void TWIClass::setPrescaler(TWIPrescaller value) {
    TWSR = (uint8_t) ((TWSR & 0b11111100) | value);
}

void TWIClass::setFrequency(uint32_t frequency) {
    /**
     * From documentation of ATMega 128
     * SCL frequency = F_CPU / ( 16 + 2 * TWBR * 4 * ( TWSR & ( (1 << TWPS1) | (1 << TWPS0) ) ) )
     */
    TWBR = (uint8_t) (((F_CPU / frequency) - 16) / 2);
}

void TWIClass::read(uint8_t *value) {
    if (rxBufferIndex < rxBufferLength) {
        *value = rxBufferData[rxBufferIndex++];
    } else {
        error = TWI_ERROR_READ;
    }
}

void TWIClass::read(uint16_t *value) {
    this->read((uint8_t *) &value, 2);//TODO <-- check cast pointer is valid
}

void TWIClass::read(uint32_t *value) {
    this->read((uint8_t *) &value, 4);//TODO <-- check cast pointer is valid
}

void TWIClass::read(float *value) {
    this->read((uint8_t *) &value, 4);//TODO <-- check cast pointer is valid
}

void TWIClass::read(uint8_t *data, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        this->read(&data[i]);
    }
}

void TWIClass::write(uint8_t value) {
    if (txBufferLength < TWI_BUFFER_LENGTH) {
        txBufferData[txBufferIndex] = value;

        txBufferIndex++;
        txBufferLength++;
    } else {
        error = TWI_ERROR_WRITE;
    }
}

void TWIClass::write(uint16_t value) {
    this->write((uint8_t *) &value, 2);//TODO <-- check cast pointer is valid
}

void TWIClass::write(uint32_t value) {
    this->write((uint8_t *) &value, 4);//TODO <-- check cast pointer is valid
}

void TWIClass::write(float value) {
    this->write((uint8_t *) &value, 4);//TODO <-- check cast pointer is valid
}

void TWIClass::write(uint8_t *data, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        this->write(data[i]);
    }
}

void TWIClass::receive(uint8_t address, uint8_t length) {
    uint8_t i;

    /*if (TWI_BUFFER_LENGTH < length) {
        return 0;
    }*/

    // wait until twi is ready, become master receiver
    /*while (TWI_READY != twi_state) {
        continue;
    }*/

    state = TWI_STATE_MASTER_RX;

    //TODO twi_sendStop = sendStop;

    error = TWI_ERROR_NONE;

    rxBufferIndex  = 0;
    rxBufferLength = 0;

    // This is not intuitive, read on...
    // On receive, the previously configured ACK/NACK setting is transmitted in
    // response to the received byte before the interrupt is signalled.
    // Therefor we must actually set NACK when the _next_ to last byte is
    // received, causing that NACK to be sent in response to receiving the last
    // expected byte of data.

    addressAndRW = (uint8_t) ((address << 1) | TW_READ);

    if (isRepeatedStart) {
        /**
         * If we're in the repeated start state, then we've already sent the start, (@@@ we hope),
         * and the TWI state machine is just waiting for the address byte.
         *
         * We need to remove ourselves from the repeated start state before we enable interrupts, since the ISR is
         * ASYNC, and we could get confused if we hit the ISR before cleaning up.
         *
         * Also, don't enable the START interrupt.
         *
         * There may be one pending from the repeated start that we sent ourselves, and that would really confuse things.
         *
         * Remember, we're dealing with an ASYNC ISR.
         */
        isRepeatedStart = 0;

        do {
            TWDR = addressAndRW;
        } while(TWCR & _BV(TWWC));

        // Enable INTs, but not START
        TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);
    } else {
        // Enable INTs and send START
        TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);
    }

    // wait for read operation to complete
    /*while (TWI_MRX == twi_state){
        continue;
    }

    if (twi_masterBufferIndex < length)
        length = twi_masterBufferIndex;

    // copy twi buffer to data
    for(i = 0; i < length; ++i){
        data[i] = twi_masterBuffer[i];
    }

    return length;*/
}

void TWIClass::start() {
    txBufferIndex  = 0;
    txBufferLength = 0;
}

void TWIClass::transmit(uint8_t address) {
    uint8_t i;

    if (TWI_BUFFER_LENGTH < txBufferLength) {
        return; // Error: buffer overflow
    }

    // wait until twi is ready, become master transmitter
    while (TWI_STATE_READY != state) {
        continue;//TODO check is need
    }

    state = TWI_STATE_MASTER_TX;
    error = TWI_ERROR_NONE;

    //TODO twi_sendStop = sendStop;

    addressAndRW = (uint8_t) ((address << 1) | TW_WRITE);

    if (isRepeatedStart) {
        /**
         * If we're in the repeated start state, then we've already sent the start, (@@@ we hope),
         * and the TWI state machine is just waiting for the address byte.
         *
         * We need to remove ourselves from the repeated start state before we enable interrupts, since the ISR is
         * ASYNC, and we could get confused if we hit the ISR before cleaning up.
         *
         * Also, don't enable the START interrupt.
         *
         * There may be one pending from the repeated start that we sent ourselves, and that would really confuse things.
         *
         * Remember, we're dealing with an ASYNC ISR.
         */
        isRepeatedStart = 0;

        do {
            TWDR = addressAndRW;
        } while (TWCR & _BV(TWWC));

        // Enable INTs, but not START
        TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE);
    } else {
        // Enable INTs and send START
        TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN) | _BV(TWIE) | _BV(TWSTA);
    }

    // wait for write operation to complete
    /*while (wait && (TWI_MTX == twi_state)){
        continue;
    }*/

    /*if (twi_error == 0xFF)
        return 0;	// success
    else if (twi_error == TW_MT_SLA_NACK)
        return 2;	// error: address send, nack received
    else if (twi_error == TW_MT_DATA_NACK)
        return 3;	// error: data send, nack received
    else
        return 4;	// other twi error*/
}

void TWIClass::setOnTransmitHandler(void (*handler_ptr) ()) {
    onTransmitHandler = handler_ptr;
}

void TWIClass::setOnReceiveHandler(void (*handler_ptr) ()) {
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
            TWDR = addressAndRW;
            TWI_SEND_ACK();
            break;

#if TWI_ENABLE_MASTER == 1
        // Master Transmitter
        case TW_MT_SLA_ACK:  // slave receiver acked address
        case TW_MT_DATA_ACK: // slave receiver acked data
            // if there is data to send, send it, otherwise stop
            if (txBufferIndex < txBufferLength) {
                // copy data to output register and ack
                TWDR = txBufferData[txBufferIndex++];
                TWI_SEND_ACK();
            } else {
                if (sendStop) {
                    TWI_SEND_STOP();
                    state = TWI_STATE_READY;

                    if (onTransmitHandler) {
                        onTransmitHandler();//TODO check if valid
                    }
                } else {
                    isRepeatedStart = 1;
                    // we're gonna send the START
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
            rxBufferData[rxBufferIndex++] = TWDR;
        case TW_MR_SLA_ACK:  // address sent, ack received
            // ack if more bytes are expected, otherwise nack
            if (rxBufferIndex < rxBufferLength) {
                TWI_SEND_ACK();
            } else {
                TWI_SEND_NACK();
            }
            break;
        case TW_MR_DATA_NACK: // data received, nack sent
            rxBufferData[rxBufferIndex++] = TWDR;

            if (sendStop) {
                TWI_SEND_STOP();
                state = TWI_STATE_READY;

                if (onReceiveHandler) {
                    onReceiveHandler();
                }
            } else {
                isRepeatedStart = 1;
                // we're gonna send the START
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
                onReceiveHandler();
            }

            rxBufferIndex  = 0;
            rxBufferLength = 0;
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
                // Call user function to fill tx buffer
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