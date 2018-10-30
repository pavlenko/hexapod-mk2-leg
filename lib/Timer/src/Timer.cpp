#include "Timer.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

#if defined(TCNT2)

Timer2Class::Timer2Class() = default;

void Timer2Class::setClockSource(ClockSource clockSource) {
#if defined(TCCR2B)
    TCCR2B = (uint8_t) ((TCCR2B & TIMER_CLOCK_MASK) | clockSource);
#elif defined(TCCR2)
    TCCR2 = (uint8_t) ((TCCR2 & TIMER_CLOCK_MASK) | clockSource);
#endif
}

void Timer2Class::setInterruptHandler(Timer2Interrupt interrupt, void (*handler_ptr) ()) {
    this->handlers[interrupt] = handler_ptr;

    if (handler_ptr) {
        //TODO
    } else {
        //TODO
    }
}

void Timer2Class::triggerInterrupt(Timer2Interrupt interrupt) {
    if (this->handlers[interrupt]) {
        this->handlers[interrupt]();
    }
}

Timer2Class Timer2;

ISR(TIMER2_OVF_vect){
    Timer2.triggerInterrupt(TIMER2_OVERFLOW_INTERRUPT);
}

#if defined(OCIE2A)

ISR(TIMER2_COMPA_vect){
    Timer2.triggerInterrupt(TIMER2_COMPARE_A_INTERRUPT);
}

ISR(TIMER2_COMPB_vect){
    Timer2.triggerInterrupt(TIMER2_COMPARE_B_INTERRUPT);
}

#elif defined(OCIE2)

ISR(TIMER2_COMP_vect){
    Timer2.triggerInterrupt(TIMER2_COMPARE_INTERRUPT);
}

#endif

#endif //TCNT2

#if defined(TCNT3)

Timer3Class::Timer3Class() = default;

void Timer3Class::setClockSource(ClockSource clockSource) {
    TCCR3B = (uint8_t) ((TCCR3B & TIMER_CLOCK_MASK) | clockSource);
}

void Timer3Class::setInterruptHandler(Timer3Interrupt interrupt, void (*handler_ptr) ()) {
    this->handlers[interrupt] = handler_ptr;

    if (handler_ptr) {
        //TODO
    } else {
        //TODO
    }
}

void Timer3Class::triggerInterrupt(Timer3Interrupt interrupt) {
    if (this->handlers[interrupt]) {
        this->handlers[interrupt]();
    }
}

Timer3Class Timer3;

ISR(TIMER3_OVF_vect){
    Timer3.triggerInterrupt(TIMER3_OVERFLOW_INTERRUPT);
}

#endif //TCNT3

#if defined(TCNT4)

Timer4Class::Timer4Class() = default;

void Timer4Class::setClockSource(ClockSource clockSource) {
    TCCR4B = (uint8_t) ((TCCR4B & TIMER_CLOCK_MASK) | clockSource);
}

void Timer4Class::setInterruptHandler(Timer4Interrupt interrupt, void (*handler_ptr) ()) {
    this->handlers[interrupt] = handler_ptr;

    if (handler_ptr) {
        TIMSK4 |= _BV(interrupt);
    } else {
        TIMSK4 &= ~_BV(interrupt);
    }
}

void Timer4Class::triggerInterrupt(Timer4Interrupt interrupt) {
    if (this->handlers[interrupt]) {
        this->handlers[interrupt]();
    }
}

Timer4Class Timer4;

ISR(TIMER4_OVF_vect){
    Timer4.triggerInterrupt(TIMER4_OVERFLOW_INTERRUPT);
}

ISR(TIMER4_COMPA_vect){
    Timer4.triggerInterrupt(TIMER4_COMPARE_A_INTERRUPT);
}

ISR(TIMER4_COMPB_vect){
    Timer4.triggerInterrupt(TIMER4_COMPARE_B_INTERRUPT);
}

ISR(TIMER4_COMPC_vect){
    Timer4.triggerInterrupt(TIMER4_COMPARE_C_INTERRUPT);
}

ISR(TIMER4_CAPT_vect){
    Timer4.triggerInterrupt(TIMER4_INPUT_CAPTURE_INTERRUPT);
}

#endif //TCNT4

#if defined(TCNT5)

Timer5Class::Timer5Class() = default;

void Timer5Class::setClockSource(ClockSource clockSource) {
    TCCR5B = (uint8_t) ((TCCR5B & TIMER_CLOCK_MASK) | clockSource);
}

void Timer5Class::setInterruptHandler(Timer5Interrupt interrupt, void (*handler_ptr) ()) {
    this->handlers[interrupt] = handler_ptr;

    if (handler_ptr) {
        TIMSK5 |= _BV(interrupt);
    } else {
        TIMSK5 &= ~_BV(interrupt);
    }
}

void Timer5Class::triggerInterrupt(Timer5Interrupt interrupt) {
    if (this->handlers[interrupt]) {
        this->handlers[interrupt]();
    }
}

Timer5Class Timer5;

ISR(TIMER5_OVF_vect){
    Timer5.triggerInterrupt(TIMER5_OVERFLOW_INTERRUPT);
}

ISR(TIMER5_COMPA_vect){
    Timer5.triggerInterrupt(TIMER5_COMPARE_A_INTERRUPT);
}

ISR(TIMER5_COMPB_vect){
    Timer5.triggerInterrupt(TIMER5_COMPARE_B_INTERRUPT);
}

ISR(TIMER5_COMPC_vect){
    Timer5.triggerInterrupt(TIMER5_COMPARE_C_INTERRUPT);
}

ISR(TIMER5_CAPT_vect){
    Timer5.triggerInterrupt(TIMER5_INPUT_CAPTURE_INTERRUPT);
}

#endif //TCNT5