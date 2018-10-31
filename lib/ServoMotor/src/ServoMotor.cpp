#include "ServoMotor.h"

#include <util/atomic.h>
#include <stdint.h>

#define SERVOMOTOR_TOTAL 10 //TODO

//TODO calculate ticks per microsecond
#define clockCyclesPerMicrosecond() (F_CPU / 1000000L)

#define US_TO_TICKS(_val_) (_val_)
#define TICKS_TO_US(_val_) (_val_)

typedef struct {
    uint8_t number: 3;
    uint8_t attached: 1;
} pin_t;

typedef struct {
    volatile uint8_t *port;
    pin_t pin;
    uint16_t ticks;
} servo_t;

static volatile uint8_t count = 0;
static volatile servo_t servos[SERVOMOTOR_TOTAL];

ServoMotor::ServoMotor() {
    if (count < SERVOMOTOR_TOTAL) {
        this->index = count++;
    } else {
        this->index = SERVOMOTOR_INVALID;
    }
}

uint8_t ServoMotor::attach(volatile uint8_t *port, uint8_t pin) {
    return this->attach(port, pin, SERVOMOTOR_PULSE_MIN, SERVOMOTOR_PULSE_MAX);
}

uint8_t ServoMotor::attach(volatile uint8_t *port, uint8_t pin, uint16_t min, uint16_t max) {
    if (this->index != SERVOMOTOR_INVALID) {
        servos[this->index].port = port;

        servos[this->index].pin.number   = pin;
        servos[this->index].pin.attached = 1;

        //TODO maybe store this values in servos struct
        this->min = min;
        this->max = max;
    }

    return this->index;
}

void ServoMotor::detach() {
    servos[this->index].pin.attached = 0;
}

uint16_t ServoMotor::getMIN() {
    return this->min;
}

void ServoMotor::setMIN(uint16_t value) {
    this->min = value;
}

uint16_t ServoMotor::getMAX() {
    return this->max;
}

void ServoMotor::setMAX(uint16_t value) {
    this->max = value;
}

uint16_t ServoMotor::getDegree() {
    //TODO map us to angle
}

void ServoMotor::setDegree(uint16_t value) {
    //TODO map angle to us
}

uint16_t ServoMotor::getMicroseconds() {
    if (this->index != SERVOMOTOR_INVALID) {
        return TICKS_TO_US(servos[this->index].ticks);
    } else {
        return 0;
    }
}

void ServoMotor::setMicroseconds(uint16_t value) {
    if (this->index != SERVOMOTOR_INVALID) {
        uint16_t ticks = US_TO_TICKS(value);

        ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
            servos[this->index].ticks = ticks;
        }
    }
}
