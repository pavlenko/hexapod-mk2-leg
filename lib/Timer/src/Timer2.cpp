#include "Timer2.h"

#include <avr/interrupt.h>
#include <avr/io.h>

Timer2Class::Timer2Class() : Timer8BitClass() {
    TCNTn = &TCNT2;
#if defined (TIMSK)
    _TIMSKn = {
        .value  = &TIMSK,
        .TOIEn  = TOIE2,
        .OCIEnA = OCIE2,
        .OCIEnB = -1,
        .OCIEnC = -1,
        .ICIEn  = -1,
    };

    _TCCRnA = &TCCR2;
    _TCCRnB = &TCCR2;
    OCRnA   = &OCR2;
    OCRnB   = NULL;
    _channelCount = 1;
#else
    _TIMSKn = {
        .value  = &TIMSK2,
        .TOIEn  = TOIE2,
        .OCIEnA = OCIE2A,
        .OCIEnB = OCIE2B,
        .OCIEnC = -1,
        .ICIEn  = -1,
    };

    _TCCRnA = &TCCR2A;
    _TCCRnB = &TCCR2B;
    OCRnA   = &OCR2A;
    OCRnB   = &OCR2B;
    _channelCount = 2;
#endif
}

#if defined (__AVR_ATmega128__) || defined (__AVR_ATmega64__)
#else
void Timer2Class::setClockSource(Timer2ClockSource_t clockSource) {
    TimerClass::setClockSource((TimerClockSource_t) ((uint8_t) clockSource));
}
#endif

Timer2Class Timer2;

#if defined (TIMER2_COMP_vect)
ISR(TIMER2_COMP_vect)
{
    if (Timer2._onCompareMatchA) {
        Timer2._onCompareMatchA();
    }
}
#else
ISR(TIMER2_COMPA_vect)
{
    if (Timer2._onCompareMatchA) {
        Timer2._onCompareMatchA();
    }
}
ISR(TIMER2_COMPB_vect)
{
    if (Timer2._onCompareMatchB) {
        Timer2._onCompareMatchB();
    }
}
#endif
ISR(TIMER2_OVF_vect)
{
    if (Timer2._onOverflow) {
        Timer2._onOverflow();
    }
}