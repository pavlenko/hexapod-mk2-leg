#include "ServoMotor.h"

#include <stdint.h>

#define SERVOMOTOR_TOTAL 10 //TODO

#define SERVOMOTOR_INVALID_INDEX 255

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
        this->index = SERVOMOTOR_INVALID_INDEX;
    }
}

uint8_t ServoMotor::attach(volatile uint8_t *port, uint8_t pin) {
    if (this->index != SERVOMOTOR_INVALID_INDEX) {
        servos[this->index].port = port;

        servos[this->index].pin.number   = pin;
        servos[this->index].pin.attached = 1;
    }

    return this->index;
}

void ServoMotor::detach() {
    servos[this->index].pin.attached = 0;
}

uint16_t ServoMotor::getMicroseconds() {
    //TODO
}

void ServoMotor::setMicroseconds(uint16_t value) {
    //TODO
}

uint16_t ServoMotor::getDegree() {
    //TODO
}

void ServoMotor::setDegree(uint16_t value) {

}