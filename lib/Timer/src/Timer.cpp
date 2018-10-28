#include "Timer.h"

#include <avr/interrupt.h>
#include <avr/io.h>

Timer0Class::Timer0Class() {
    this->onOverflow = nullptr;
}

void Timer0Class::setClockSource(ClockSource clockSource) {
#if defined(TCCR0B)
    TCCR0B = (uint8_t) ((TCCR0B & TIMER_CLOCK_MASK) | clockSource);
#elif defined(TCCR0)
    TCCR0 = (uint8_t) ((TCCR0 & TIMER_CLOCK_MASK) | clockSource);
#endif
}

uint8_t Timer0Class::getValue() {
    return TCNT0;
}

void Timer0Class::setValue(uint8_t value) {
    TCNT0 = value;
}

Timer0Class Timer0;

ISR(TIMER0_OVF_vect){
    if (Timer0.onOverflow) {
        Timer0.onOverflow();
    }
}


