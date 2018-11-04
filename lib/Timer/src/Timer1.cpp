#include "Timer1.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

Timer1Class::Timer1Class() = default;

void Timer1Class::setClockSource(Timer1ClockSource clockSource) {
    TCCR1B = (uint8_t) ((TCCR1B & TIMER1_CLOCK_MASK) | clockSource);
}

void Timer1Class::setWaveformGenerationMode(Timer1WaveformGenerationMode mode) {
    TCCR1A = (uint8_t) ((TCCR1A & 0b11111100) | (mode & 0b0011));
    TCCR1B = (uint8_t) ((TCCR1B & 0b00011000) | ((mode & 0b1100) << 3));
}

void Timer1Class::setCompareOutputModeA(Timer1OutputCompareMode mode) {
    TCCR1A = (uint8_t) ((TCCR1A & 0b00111111) | (mode << 6));
}

void Timer1Class::setCompareOutputModeB(Timer1OutputCompareMode mode) {
    TCCR1A = (uint8_t) ((TCCR1A & 0b11001111) | (mode << 4));
}

void Timer1Class::setInterruptHandler(Timer1Interrupt interrupt, void (*handler_ptr) ()) {
    this->handlers[interrupt] = handler_ptr;

    //TODO check registers
    if (handler_ptr) {
#if defined(TIMSK0)
        TIMSK0 |= _BV(interrupt);
#elif defined(TIMSK)
        TIMSK |= _BV(interrupt);
#endif
    } else {
#if defined(TIMSK0)
        TIMSK0 &= ~_BV(interrupt);
#elif defined(TIMSK)
        TIMSK &= ~_BV(interrupt);
#endif
    }
}

uint16_t Timer1Class::getValue() {
    return TCNT1;
}

void Timer1Class::setValue(uint16_t value) {
    TCNT1 = value;
}

uint16_t Timer1Class::getCompareValueA() {
    return OCR1A;
}

void Timer1Class::setCompareValueA(uint16_t value) {
    OCR1A = value;
}

uint16_t Timer1Class::getCompareValueB() {
    return OCR1B;
}

void Timer1Class::setCompareValueB(uint16_t value) {
    OCR1B = value;
}

uint16_t Timer1Class::getCaptureValue() {
    return ICR1;
}

void Timer1Class::setCaptureValue(uint16_t value) {
    ICR1 = value;
}

void Timer1Class::triggerInterrupt(Timer1Interrupt interrupt) {
    if (this->handlers[interrupt]) {
        this->handlers[interrupt]();
    }
}

Timer1Class Timer1;

ISR(TIMER1_OVF_vect){
    Timer1.triggerInterrupt(TIMER1_OVERFLOW_INTERRUPT);
}

ISR(TIMER1_COMPA_vect){
    Timer1.triggerInterrupt(TIMER1_COMPARE_A_INTERRUPT);
}

ISR(TIMER1_COMPB_vect){
    Timer1.triggerInterrupt(TIMER1_COMPARE_B_INTERRUPT);
}

ISR(TIMER1_CAPT_vect){
    Timer1.triggerInterrupt(TIMER1_CAPTURE_INTERRUPT);
}
