#include "HWTimer.h"

HWTimer0Class::HWTimer0Class() : HWTimer8Bit() {
    _TCNTn = &TCNT0;

#if defined (TIMSK)
    _TIMSKn = &TIMSK;
    _TCCRnA = &TCCR0;
    _TCCRnB = &TCCR0;
    _OCRnA  = &OCR0;
    _OCRnB  = NULL;

    _channelCount = 1;
#else
    _TIMSKn = &TIMSK0;
    _TCCRnA = &TCCR0A;
    _TCCRnB = &TCCR0B;
    _OCRnA  = &OCR0A;
    _OCRnB  = &OCR0B;

    _channelCount = 2;
#endif
}

HWTimer0Class HWTimer0;

#if HW_TIMER_8BIT_COUNT == 2
HWTimer2Class::HWTimer2Class() : HWTimer8Bit() {
    _TCNTn = &TCNT2;

#if defined (TIMSK)
    _TIMSKn = &TIMSK;
    _TCCRnA = &TCCR2;
    _TCCRnB = &TCCR2;
    _OCRnA  = &OCR2;
    _OCRnA  = NULL;

    _channelCount = 1;
#else
    _TIMSKn = &TIMSK2;
    _TCCRnA = &TCCR2A;
    _TCCRnB = &TCCR2B;
    _OCRnA  = &OCR2A;
    _OCRnB  = &OCR2B;

    _channelCount = 2;
#endif
}

HWTimer2Class HWTimer2;
#endif

