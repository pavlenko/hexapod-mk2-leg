#include "Timer1.h"

#include <avr/interrupt.h>
#include <avr/io.h>

Timer1Class::Timer1Class(): Timer16BitClass() {
#if defined (TIMSK)
    _TIMSKn = {
        .value  = &TIMSK,
        .TOIEn  = TOIE1,
        .OCIEnA = OCIE1A,
        .OCIEnB = OCIE1B,
#if defined (OCR1C)
        .OCIEnC = OCIE1C,
#else
        .OCIEnC = -1,
#endif
        .ICIEn  = TICIE1,
    };
#else
    _TIMSKn = {
        .value  = &TIMSK1,
        .TOIEn  = TOIE1,
        .OCIEnA = OCIE1A,
        .OCIEnB = OCIE1B,
        .OCIEnC = OCIE1C,
        .ICIEn  = ICIE1,
    };
#endif

    _TCCRnA = &TCCR1A;
    _TCCRnB = &TCCR1B;
    TCNTn   = &TCNT1;
    OCRnA   = &OCR1A;
    OCRnB   = &OCR1B;
#if defined (OCR1C)
    OCRnC   = &OCR1C;
    _channelCount = 3;
#else
    OCRnC   = NULL;
    _channelCount = 2;
#endif
}

Timer1Class Timer1;

ISR(TIMER1_COMPA_vect)
{
    if (Timer1._onCompareMatchA) {
        Timer1._onCompareMatchA();
    }
}
ISR(TIMER1_COMPB_vect)
{
    if (Timer1._onCompareMatchB) {
        Timer1._onCompareMatchB();
    }
}
#if defined (TIMER1_COMPC_vect)
ISR(TIMER1_COMPC_vect)
{
    if (Timer1._onCompareMatchC) {
        Timer1._onCompareMatchC();
    }
}
#endif
ISR(TIMER1_OVF_vect)
{
    if (Timer1._onOverflow) {
        Timer1._onOverflow();
    }
}
ISR(TIMER1_CAPT_vect)
{
    if (Timer1._onCaptureInput) {
        Timer1._onCaptureInput();
    }
}