#ifndef TIMER3_H
#define TIMER3_H

#include "Timer16Bit.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#if (TIMER_16BIT_COUNT > 1)

ISR(TIMER3_COMPA_vect);
ISR(TIMER3_COMPB_vect);
ISR(TIMER3_COMPC_vect);
ISR(TIMER3_OVF_vect);
ISR(TIMER3_CAPT_vect);

class Timer3Class: public Timer16BitClass {
    friend void TIMER3_COMPA_vect();
    friend void TIMER3_COMPB_vect();
    friend void TIMER3_COMPC_vect();
    friend void TIMER3_OVF_vect();
    friend void TIMER3_CAPT_vect();
public:
    Timer3Class();
};

extern Timer3Class Timer3;

#endif //TIMER_16BIT_COUNT

#endif //TIMER3_H
