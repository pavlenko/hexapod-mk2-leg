#include "ServoMotor.h"

#include <util/atomic.h>
#include <stdint.h>

#define SERVOMOTOR_TOTAL 10 //TODO

//TODO check dependency of timers prescaller
#define TICKS_PER_US() (F_CPU / 1000000L)
#define US_TO_TICKS(_val_) ((uint16_t) (_val_ * TICKS_PER_US()))
#define TICKS_TO_US(_val_) ((uint16_t) (_val_ / TICKS_PER_US()))

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

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

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

uint16_t ServoMotor::getAngle() {
    return (uint16_t) map(this->getMicroseconds(), this->min, this->max, 0, 180);
}

void ServoMotor::setAngle(uint16_t value) {
    this->setMicroseconds((uint16_t) map(value, 0, 180, this->min, this->max));
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
