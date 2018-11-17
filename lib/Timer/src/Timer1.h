#ifndef TIMER1_H
#define TIMER1_H

#include "Timer16Bit.h"

#include <avr/interrupt.h>
#include <avr/io.h>

ISR(TIMER1_COMPA_vect);
ISR(TIMER1_COMPB_vect);
#if defined (TIMER1_COMPC_vect)
ISR(TIMER1_COMPC_vect);
#endif
ISR(TIMER1_OVF_vect);
ISR(TIMER1_CAPT_vect);

class Timer1Class: public Timer16BitClass {
    friend void TIMER1_COMPA_vect();
    friend void TIMER1_COMPB_vect();
#if defined (TIMER1_COMPC_vect)
    friend void TIMER1_COMPC_vect();
#endif
    friend void TIMER1_OVF_vect();
    friend void TIMER1_CAPT_vect();
public:
    Timer1Class();
};

extern Timer1Class Timer1;

#endif //TIMER1_H
