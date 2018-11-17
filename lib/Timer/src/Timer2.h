#ifndef TIMER2_H
#define TIMER2_H

#include "Timer8Bit.h"

#if (TIMER_8BIT_COUNT < 2)
#error "You CPU does not support Timer 2"
#endif

#include <avr/interrupt.h>
#include <avr/io.h>

#if defined (TIMER2_COMP_vect)
ISR(TIMER2_COMP_vect);
#else
ISR(TIMER2_COMPA_vect);
ISR(TIMER2_COMPB_vect);
#endif
ISR(TIMER2_OVF_vect);

class Timer2Class: public Timer8BitClass {
#if defined (TIMER2_COMP_vect)
    friend void TIMER2_COMP_vect();
#else
    friend void TIMER2_COMPA_vect();
    friend void TIMER2_COMPB_vect();
#endif
    friend void TIMER2_OVF_vect();
public:
    Timer2Class();
#if defined (__AVR_ATmega128__) || defined (__AVR_ATmega64__)
#else
    void setClockSource(Timer2ClockSource_t clockSource);
#endif
};

extern Timer2Class Timer2;

#endif //TIMER2_H
