#include "HWTimer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include <stdint.h>

HWTimer8Bit::HWTimer8Bit() = default;

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

#if defined (TIMER0_COMP_vect)
ISR(TIMER0_COMP_vect)
{
    if (Timer0.compareMatchAFunction != NULL) {
        Timer0.compareMatchAFunction();
    }
}
#else
ISR(TIMER0_COMPA_vect)
{
    if (HWTimer0.compareMatchAFunction != NULL) {
        HWTimer0.compareMatchAFunction();
    }
}
ISR(TIMER0_COMPB_vect)
{
    if (HWTimer0.compareMatchBFunction != NULL) {
        HWTimer0.compareMatchBFunction();
    }
}
#endif
ISR(TIMER0_OVF_vect)
{
    if (HWTimer0.overflowFunction != NULL) {
        HWTimer0.overflowFunction();
    }
}

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

#if defined (TIMER2_COMP_vect)
ISR(TIMER2_COMP_vect)
{
    if (HWTimer2.compareMatchAFunction != NULL) {
        HWTimer2.compareMatchAFunction();
    }
}
#else
ISR(TIMER2_COMPA_vect)
{
    if (HWTimer2.compareMatchAFunction != NULL) {
        HWTimer2.compareMatchAFunction();
    }
}
ISR(TIMER2_COMPB_vect)
{
    if (HWTimer2.compareMatchBFunction != NULL) {
        HWTimer2.compareMatchBFunction();
    }
}
#endif
ISR(TIMER2_OVF_vect)
{
    if (HWTimer2.overflowFunction != NULL) {
        HWTimer2.overflowFunction();
    }
}
#endif

HWTimer16Bit::HWTimer16Bit() = default;

HWTimer1Class::HWTimer1Class(): HWTimer16Bit() {
#if defined (TIMSK)
    _TIMSKn = &TIMSK;
#else
    _TIMSKn = &TIMSK1;
#endif
    _TCCRnA = &TCCR1A;
    _TCCRnB = &TCCR1B;
    _TCNTn  = &TCNT1;
    _OCRnA  = &OCR1A;
    _OCRnB  = &OCR1B;
#if defined (OCR1CH)
    _OCRnC  = &OCR1C;
    _channelCount = 3;
#else
    _OCRnC  = NULL;
    _channelCount = 2;
#endif
}

HWTimer1Class HWTimer1;

ISR(TIMER1_COMPA_vect)
{
    if (HWTimer1.compareMatchAFunction != NULL) {
        HWTimer1.compareMatchAFunction();
    }
}
ISR(TIMER1_COMPB_vect)
{
    if (HWTimer1.compareMatchBFunction != NULL) {
        HWTimer1.compareMatchBFunction();
    }
}
#if defined (TIMER1_COMPC_vect)
ISR(TIMER1_COMPC_vect)
{
    if (HWTimer1.compareMatchCFunction != NULL) {
        HWTimer1.compareMatchCFunction();
    }
}
#endif
ISR(TIMER1_OVF_vect)
{
    if (HWTimer1.overflowFunction != NULL) {
        HWTimer1.overflowFunction();
    }
}
ISR(TIMER1_CAPT_vect)
{
    if (HWTimer1.captureEventFunction != NULL) {
        HWTimer1.captureEventFunction();
    }
}

#if (HW_TIMER_16BIT_COUNT > 1)
HWTimer3Class::HWTimer3Class(): HWTimer16Bit() {
#if defined (TIMSK)
    _TIMSKn = &ETIMSK;
#else
    _TIMSKn = &TIMSK3;
#endif
    _TCCRnA = &TCCR3A;
    _TCCRnB = &TCCR3B;
    _TCNTn  = &TCNT3;
    _OCRnA  = &OCR3A;
    _OCRnB  = &OCR3B;
#if defined (OCR3C)
    _OCRnC  = &OCR3C;
    _channelCount = 3;
#else
    _OCRnC  = NULL;
    _channelCount = 2;
#endif
}

HWTimer3Class HWTimer3;

ISR(TIMER3_COMPA_vect)
{
    if (HWTimer3.compareMatchAFunction != NULL) {
        HWTimer3.compareMatchAFunction();
    }
}
ISR(TIMER3_COMPB_vect)
{
    if (HWTimer3.compareMatchBFunction != NULL) {
        HWTimer3.compareMatchBFunction();
    }
}
ISR(TIMER3_COMPC_vect)
{
    if (HWTimer3.compareMatchCFunction != NULL) {
        HWTimer3.compareMatchCFunction();
    }
}
ISR(TIMER3_OVF_vect)
{
    if (HWTimer3.overflowFunction != NULL) {
        HWTimer3.overflowFunction();
    }
}
ISR(TIMER3_CAPT_vect)
{
    if (HWTimer3.captureEventFunction != NULL) {
        HWTimer3.captureEventFunction();
    }
}
#endif

#if (HW_TIMER_16BIT_COUNT > 2)
HWTimer4Class::HWTimer4Class(): HWTimer16Bit() {
    _TIMSKn = &TIMSK4;
    _TCCRnA = &TCCR4A;
    _TCCRnB = &TCCR4B;
    _TCNTn  = &TCNT4;
    _OCRnA  = &OCR4A;
    _OCRnB  = &OCR4B;
    _OCRnC  = &OCR4C;
    _channelCount = 3;
}

HWTimer4Class HWTimer4;

ISR(TIMER4_COMPA_vect)
{
    if (HWTimer4.compareMatchAFunction != NULL) {
        HWTimer4.compareMatchAFunction();
    }
}
ISR(TIMER4_COMPB_vect)
{
    if (HWTimer4.compareMatchBFunction != NULL) {
        HWTimer4.compareMatchBFunction();
    }
}
ISR(TIMER4_COMPC_vect)
{
    if (HWTimer4.compareMatchCFunction != NULL) {
        HWTimer4.compareMatchCFunction();
    }
}
ISR(TIMER4_OVF_vect)
{
    if (HWTimer4.overflowFunction != NULL) {
        HWTimer4.overflowFunction();
    }
}
ISR(TIMER4_CAPT_vect)
{
    if (HWTimer4.captureEventFunction != NULL) {
        HWTimer4.captureEventFunction();
    }
}

HWTimer5Class::HWTimer5Class(): HWTimer16Bit() {
    _TIMSKn = &TIMSK5;
    _TCCRnA = &TCCR5A;
    _TCCRnB = &TCCR5B;
    _TCNTn  = &TCNT5;
    _OCRnA  = &OCR5A;
    _OCRnB  = &OCR5B;
    _OCRnC  = &OCR5C;
    _channelCount = 3;
}

HWTimer5Class HWTimer5;

ISR(TIMER5_COMPA_vect)
{
    if (HWTimer5.compareMatchAFunction != NULL) {
        HWTimer5.compareMatchAFunction();
    }
}
ISR(TIMER5_COMPB_vect)
{
    if (HWTimer5.compareMatchBFunction != NULL) {
        HWTimer5.compareMatchBFunction();
    }
}
ISR(TIMER5_COMPC_vect)
{
    if (HWTimer5.compareMatchCFunction != NULL) {
        HWTimer5.compareMatchCFunction();
    }
}
ISR(TIMER5_OVF_vect)
{
    if (HWTimer5.overflowFunction != NULL) {
        HWTimer5.overflowFunction();
    }
}
ISR(TIMER5_CAPT_vect)
{
    if (HWTimer5.captureEventFunction != NULL) {
        HWTimer5.captureEventFunction();
    }
}
#endif
