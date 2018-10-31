#include "ServoMotor.h"

#include <stdint.h>

#define SERVOMOTOR_TOTAL 10 //TODO

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
    //TODO
}

void ServoMotor::setMIN(uint16_t value) {
    //TODO
}

uint16_t ServoMotor::getMAX() {
    //TODO
}

void ServoMotor::setMAX(uint16_t) {
    //TODO
}

uint16_t ServoMotor::getDegree() {
    //TODO
}

void ServoMotor::setDegree(uint16_t value) {

}

uint16_t ServoMotor::getMicroseconds() {
    //TODO
}

void ServoMotor::setMicroseconds(uint16_t value) {
    //TODO
}
