#include "Timer1.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

Timer1Class::Timer1Class() = default;

void Timer1Class::setClockSource(Timer1ClockSource clockSource) {
    TCCR1B = (uint8_t) ((TCCR1B & TIMER1_CLOCK_MASK) | clockSource);
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

//TODO atomic
uint16_t Timer1Class::getValue() {
    return TCNT1;
}

//TODO atomic
void Timer1Class::setValue(uint16_t value) {
    TCNT1 = value;
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
