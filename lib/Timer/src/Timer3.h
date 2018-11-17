#ifndef TIMER3_H
#define TIMER3_H

#include "Timer16Bit.h"

#if (TIMER_16BIT_COUNT < 2)
#error "You CPU does not support Timer 3"
#endif

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

#endif //TIMER3_H
