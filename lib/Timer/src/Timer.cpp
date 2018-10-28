#include "Timer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

Timer0Class::Timer0Class() = default;

void Timer0Class::setClockSource(ClockSource clockSource) {
#if defined(TCCR0B)
    TCCR0B = (uint8_t) ((TCCR0B & TIMER_CLOCK_MASK) | clockSource);
#elif defined(TCCR0)
    TCCR0 = (uint8_t) ((TCCR0 & TIMER_CLOCK_MASK) | clockSource);
#endif
}

void Timer0Class::setInterruptHandler(Timer0Interrupt interrupt, void (*handler_ptr) ()) {
    this->handlers[interrupt] = handler_ptr;

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

uint8_t Timer0Class::getValue() {
    return TCNT0;
}

void Timer0Class::setValue(uint8_t value) {
    TCNT0 = value;
}

void Timer0Class::triggerInterrupt(Timer0Interrupt interrupt) {
    if (this->handlers[interrupt]) {
        this->handlers[interrupt]();
    }
}

Timer0Class Timer0;

ISR(TIMER0_OVF_vect){
    Timer0.triggerInterrupt(TIMER0_OVERFLOW_INTERRUPT);
}

#if defined(OCR0A)
ISR(TIMER0_COMPA_vect){
    Timer0.triggerInterrupt(TIMER0_COMPARE_A_INTERRUPT);
}

ISR(TIMER0_COMPB_vect){
    Timer0.triggerInterrupt(TIMER0_COMPARE_B_INTERRUPT);
}
#elif defined(OCR0)
ISR(TIMER0_COMP_vect){
    Timer0.triggerInterrupt(TIMER0_COMPARE_INTERRUPT);
}
#endif


