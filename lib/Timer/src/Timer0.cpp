#include "Timer0.h"

#include <avr/interrupt.h>
#include <avr/io.h>

Timer0Class::Timer0Class() : Timer8BitClass() {
    TCNTn = &TCNT0;
#if defined (TIMSK)
    _TIMSKn = {
        .value  = &TIMSK,
        .TOIEn  = TOIE0,
        .OCIEnA = OCIE0,
        .OCIEnB = -1,
        .OCIEnC = -1,
        .ICIEn  = -1,
    };

    _TCCRnA = &TCCR0;
    _TCCRnB = &TCCR0;
    OCRnA   = &OCR0;
    OCRnB   = NULL;
    _channelCount = 1;
#else
    _TIMSKn = {
        .value  = &TIMSK0,
        .TOIEn  = TOIE0,
        .OCIEnA = OCIE0A,
        .OCIEnB = OCIE0B,
        .OCIEnC = -1,
        .ICIEn  = -1,
    };

    _TCCRnA = &TCCR0A;
    _TCCRnB = &TCCR0B;
    OCRnA   = &OCR0A;
    OCRnB   = &OCR0B;
    _channelCount = 2;
#endif
}

#if defined (__AVR_ATmega128__) || defined (__AVR_ATmega64__)
void Timer0Class::setClockSource(Timer0ClockSource_t clockSource) {
    TimerClass::setClockSource((TimerClockSource_t) ((uint8_t) clockSource));
}
#endif

Timer0Class Timer0;

#if defined (TIMER0_COMP_vect)
ISR(TIMER0_COMP_vect)
{
    if (Timer0._onCompareMatchA) {
        Timer0._onCompareMatchA();
    }
}
#else
ISR(TIMER0_COMPA_vect)
{
    if (Timer0._onCompareMatchA) {
        Timer0._onCompareMatchA();
    }
}
ISR(TIMER0_COMPB_vect)
{
    if (Timer0._onCompareMatchB) {
        Timer0._onCompareMatchB();
    }
}
#endif
ISR(TIMER0_OVF_vect)
{
    if (Timer0._onOverflow) {
        Timer0._onOverflow();
    }
}