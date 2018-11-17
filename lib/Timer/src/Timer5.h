#ifndef TIMER5_H
#define TIMER5_H

#include "Timer16Bit.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#if (TIMER_16BIT_COUNT > 2)

ISR(TIMER5_COMPA_vect);
ISR(TIMER5_COMPB_vect);
ISR(TIMER5_COMPC_vect);
ISR(TIMER5_OVF_vect);
ISR(TIMER5_CAPT_vect);

class Timer5Class: public Timer16BitClass {
    friend void TIMER5_COMPA_vect();
    friend void TIMER5_COMPB_vect();
    friend void TIMER5_COMPC_vect();
    friend void TIMER5_OVF_vect();
    friend void TIMER5_CAPT_vect();
public:
    Timer5Class();
};

extern Timer5Class Timer5;

#endif //TIMER_16BIT_COUNT

#endif //TIMER5_H
