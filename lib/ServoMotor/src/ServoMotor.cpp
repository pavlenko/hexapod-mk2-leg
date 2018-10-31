#include "ServoMotor.h"

#include <stdint.h>

ServoMotor::ServoMotor() {
    this->index = 0;
}

uint8_t ServoMotor::attach(volatile uint8_t *port, uint8_t pin) {
    //TODO
}

void ServoMotor::detach() {
    //TODO
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