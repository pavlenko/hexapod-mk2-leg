#include "Servo.h"

Servo::Servo(volatile uint8_t *port, uint8_t pin) {
    this->port = port;
    this->pin  = pin;
}

Servo::Servo(volatile uint8_t *port, uint8_t pin, uint16_t min, uint16_t max) {
    this->port = port;
    this->pin  = pin;
    this->min  = min;
    this->max  = max;
}

void Servo::attach() {

}

void Servo::detach() {

}

uint16_t Servo::getMIN() {
    return 0;
}

void Servo::setMIN(uint16_t value) {

}

uint16_t Servo::getMAX() {
    return 0;
}

void Servo::setMAX(uint16_t value) {

}

uint8_t Servo::getAngle() {
    return 0;
}

void Servo::setAngle(uint8_t value) {

}

uint16_t Servo::getMicroseconds() {
    return 0;
}

void Servo::setMicroseconds(uint16_t value) {

}
