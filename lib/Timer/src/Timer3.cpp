#include "Timer3.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#if (TIMER_16BIT_COUNT > 1)

Timer3Class::Timer3Class(): Timer16BitClass() {
#if defined (TIMSK)
    _TIMSKn = {
        .value  = &ETIMSK,
        .TOIEn  = TOIE3,
        .OCIEnA = OCIE3A,
        .OCIEnB = OCIE3B,
        .OCIEnC = OCIE3C,
        .ICIEn  = TICIE3,
    };
#else
    _TIMSKn = {
        .value  = &TIMSK3,
        .TOIEn  = TOIE3,
        .OCIEnA = OCIE3A,
        .OCIEnB = OCIE3B,
        .OCIEnC = OCIE3C,
        .ICIEn  = ICIE3,
    };
#endif

    _TCCRnA = &TCCR3A;
    _TCCRnB = &TCCR3B;
    TCNTn   = &TCNT3;
    OCRnA   = &OCR3A;
    OCRnB   = &OCR3B;
#if defined (OCR3C)
    OCRnC   = &OCR3C;
    _channelCount = 3;
#else
    OCRnC   = NULL;
    _channelCount = 2;
#endif
}

Timer3Class Timer3;

ISR(TIMER3_COMPA_vect)
{
    if (Timer3._onCompareMatchA) {
        Timer3._onCompareMatchA();
    }
}
ISR(TIMER3_COMPB_vect)
{
    if (Timer3._onCompareMatchB) {
        Timer3._onCompareMatchB();
    }
}
ISR(TIMER3_COMPC_vect)
{
    if (Timer3._onCompareMatchC) {
        Timer3._onCompareMatchC();
    }
}
ISR(TIMER3_OVF_vect)
{
    if (Timer3._onOverflow) {
        Timer3._onOverflow();
    }
}
ISR(TIMER3_CAPT_vect)
{
    if (Timer3._onCaptureInput) {
        Timer3._onCaptureInput();
    }
}

#endif //TIMER_16BIT_COUNT