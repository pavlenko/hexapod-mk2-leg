#ifndef TIMER4_H
#define TIMER4_H

#include "Timer16Bit.h"

#if (TIMER_16BIT_COUNT < 3)
#error "You CPU does not support Timer 4"
#endif

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

#endif //TIMER4_H
