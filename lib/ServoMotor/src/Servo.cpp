#include "Servo.h"

Servo::Servo(volatile uint8_t *port, uint8_t pin) {
    this->port = port;
    this->pin  = pin;
}

Servo::Servo(volatile uint8_t *port, uint8_t pin, uint16_t min, uint16_t max) {
    this->port = port;
    this->pin  = pin;
    this->min  = min ?: SERVOMOTOR_PULSE_MIN;
    this->max  = max ?: SERVOMOTOR_PULSE_MAX;
}

void Servo::attach() {
    //TODO not yet implemented
}

void Servo::detach() {
    //TODO not yet implemented
}

uint16_t Servo::getMin() {
    return this->min;
}

void Servo::setMin(uint16_t value) {
    this->min = value;
}

uint16_t Servo::getMax() {
    return this->max;
}

void Servo::setMax(uint16_t value) {
    this->max = value;
}

uint8_t Servo::getAngle() {
    return 0;
}

void Servo::setAngle(uint8_t value) {
    //TODO not yet implemented
}

uint16_t Servo::getMicroseconds() {
    return 0;
}

void Servo::setMicroseconds(uint16_t value) {
    //TODO not yet implemented
}
