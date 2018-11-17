#ifndef TIMER4_H
#define TIMER4_H

#include "Timer16Bit.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#if (TIMER_16BIT_COUNT > 2)

ISR(TIMER4_COMPA_vect);
ISR(TIMER4_COMPB_vect);
ISR(TIMER4_COMPC_vect);
ISR(TIMER4_OVF_vect);
ISR(TIMER4_CAPT_vect);

class Timer4Class: public Timer16BitClass {
    friend void TIMER4_COMPA_vect();
    friend void TIMER4_COMPB_vect();
    friend void TIMER4_COMPC_vect();
    friend void TIMER4_OVF_vect();
    friend void TIMER4_CAPT_vect();
public:
    Timer4Class();
};

extern Timer4Class Timer4;

#endif //TIMER_16BIT_COUNT

#endif //TIMER4_H
