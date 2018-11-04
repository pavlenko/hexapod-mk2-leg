#include "Timer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

void Timer16BitClass::setClockSource(TimerClockSource clockSource) {
    *this->TCCR_B = (uint8_t) ((*this->TCCR_B & 0b00000111) | clockSource);
}

void Timer16BitClass::setWaveformGenerationMode(TimerWaveformGenerationMode mode) {
    *this->TCCR_A = (uint8_t) ((*this->TCCR_A & 0b11111100) | (mode & 0b0011));
    *this->TCCR_B = (uint8_t) ((*this->TCCR_B & 0b00011000) | ((mode & 0b1100) << 3));
}

void Timer16BitClass::setOutputCompareModeA(TimerOutputCompareMode mode) {
    *this->TCCR_A = (uint8_t) ((*this->TCCR_A & 0b00111111) | (mode << 6));
}

void Timer16BitClass::setOutputCompareModeB(TimerOutputCompareMode mode) {
    *this->TCCR_A = (uint8_t) ((*this->TCCR_A & 0b11001111) | (mode << 4));
}

uint16_t Timer16BitClass::getCounterValue() {
    return *this->TCNT;
}

void Timer16BitClass::setCounterValue(uint16_t value) {
    *this->TCNT = value;
}

uint16_t Timer16BitClass::getCompareValueA() {
    return *this->OCR_A;
}

void Timer16BitClass::setCompareValueA(uint16_t value) {
    *this->OCR_A = value;
}

uint16_t Timer16BitClass::getCompareValueB() {
    return *this->OCR_B;
}

void Timer16BitClass::setCompareValueB(uint16_t value) {
    *this->OCR_B = value;
}

uint16_t Timer16BitClass::getCaptureValue() {
    return *this->ICR;
}

void Timer16BitClass::setCaptureValue(uint16_t value) {
    *this->ICR = value;
}

void Timer16BitClass::setInterruptHandler(uint8_t code, TimerInterruptHandler_t handlerPtr) {
    this->handlers[code] = handlerPtr;
}

void Timer16BitClass::triggerInterrupt(uint8_t code) {
    if (this->handlers[code]) {
        this->handlers[code]();
    }
}

Timer1Class::Timer1Class(): Timer16BitClass() {
    *TCCR_A = TCCR1A;
    *TCCR_B = TCCR1B;

    *TCNT  = TCNT1;
    *OCR_A = OCR1A;
    *OCR_B = OCR1B;
    *ICR   = ICR1;
}

Timer1Class Timer1();