#ifndef TIMER0_H
#define TIMER0_H

#include "Timer8Bit.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#if defined (TIMER0_COMP_vect)
ISR(TIMER0_COMP_vect);
#else
ISR(TIMER0_COMPA_vect);
ISR(TIMER0_COMPB_vect);
#endif
ISR(TIMER0_OVF_vect);

class Timer0Class: public Timer8BitClass {
#if defined (TIMER0_COMP_vect)
    friend void TIMER0_COMP_vect();
#else
    friend void TIMER0_COMPA_vect();
    friend void TIMER0_COMPB_vect();
#endif
    friend void TIMER0_OVF_vect();
public:
    Timer0Class();
#if defined (__AVR_ATmega128__) || defined (__AVR_ATmega64__)
    void setClockSource(Timer0ClockSource_t clockSource);
#endif
};

extern Timer0Class Timer0;

#endif //TIMER0_H
