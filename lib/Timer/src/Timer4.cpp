#include "Timer4.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#if (TIMER_16BIT_COUNT > 2)

Timer4Class::Timer4Class(): Timer16BitClass() {
    _TIMSKn = {
        .value  = &TIMSK4,
        .TOIEn  = TOIE4,
        .OCIEnA = OCIE4A,
        .OCIEnB = OCIE4B,
        .OCIEnC = OCIE4C,
        .ICIEn  = ICIE4,
    };

    _TCCRnA = &TCCR4A;
    _TCCRnB = &TCCR4B;
    TCNTn   = &TCNT4;
    OCRnA   = &OCR4A;
    OCRnB   = &OCR4B;
    OCRnC   = &OCR4C;
    _channelCount = 3;
}

Timer4Class Timer4;

ISR(TIMER4_COMPA_vect)
{
    if (Timer4._onCompareMatchA) {
        Timer4._onCompareMatchA();
    }
}
ISR(TIMER4_COMPB_vect)
{
    if (Timer4._onCompareMatchB) {
        Timer4._onCompareMatchB();
    }
}
ISR(TIMER4_COMPC_vect)
{
    if (Timer4._onCompareMatchC) {
        Timer4._onCompareMatchC();
    }
}
ISR(TIMER4_OVF_vect)
{
    if (Timer4._onOverflow) {
        Timer4._onOverflow();
    }
}
ISR(TIMER4_CAPT_vect)
{
    if (Timer4._onCaptureInput) {
        Timer4._onCaptureInput();
    }
}

#endif //TIMER_16BIT_COUNT