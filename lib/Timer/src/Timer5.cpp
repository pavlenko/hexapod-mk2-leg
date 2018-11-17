#include "Timer5.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#if (TIMER_16BIT_COUNT > 2)

Timer5Class::Timer5Class(): Timer16BitClass() {
    _TIMSKn = {
        .value  = &TIMSK5,
        .TOIEn  = TOIE5,
        .OCIEnA = OCIE5A,
        .OCIEnB = OCIE5B,
        .OCIEnC = OCIE5C,
        .ICIEn  = ICIE5,
    };

    _TCCRnA = &TCCR5A;
    _TCCRnB = &TCCR5B;
    TCNTn   = &TCNT5;
    OCRnA   = &OCR5A;
    OCRnB   = &OCR5B;
    OCRnC   = &OCR5C;
    _channelCount = 3;
}

Timer5Class Timer5;

ISR(TIMER5_COMPA_vect)
{
    if (Timer5._onCompareMatchA) {
        Timer5._onCompareMatchA();
    }
}
ISR(TIMER5_COMPB_vect)
{
    if (Timer5._onCompareMatchB) {
        Timer5._onCompareMatchB();
    }
}
ISR(TIMER5_COMPC_vect)
{
    if (Timer5._onCompareMatchC) {
        Timer5._onCompareMatchC();
    }
}
ISR(TIMER5_OVF_vect)
{
    if (Timer5._onOverflow) {
        Timer5._onOverflow();
    }
}
ISR(TIMER5_CAPT_vect)
{
    if (Timer5._onCaptureInput) {
        Timer5._onCaptureInput();
    }
}

#endif //TIMER_16BIT_COUNT