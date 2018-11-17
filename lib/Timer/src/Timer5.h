#ifndef TIMER5_H
#define TIMER5_H

#include "Timer16Bit.h"

#if (TIMER_16BIT_COUNT < 3)
#error "You CPU does not support Timer 5"
#endif

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

#endif //TIMER5_H
