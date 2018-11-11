#include "HWTimer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include <stdint.h>

HWTimerClass::HWTimerClass() = default;

void HWTimerClass::setClockSource(HWTimerClockSource_t clockSource) {
    *_TCCRnB = (uint8_t) ((*_TCCRnB & 0b11111000) | clockSource);
}

void HWTimerClass::setInterruptEnabled(HWTimerInterrupt_t interrupt, bool enabled) {
    int8_t flag = -1;

    volatile uint8_t *value = _TIMSKn.value;

    switch (interrupt) {
        case HW_TIMER_INTERRUPT_OVERFLOW:
            flag = _TIMSKn.TOIEn;
            break;
        case HW_TIMER_INTERRUPT_COMPARE_MATCH_A:
            flag = _TIMSKn.OCIEnA;
            break;
        case HW_TIMER_INTERRUPT_COMPARE_MATCH_B:
            flag = _TIMSKn.OCIEnB;
            break;
        case HW_TIMER_INTERRUPT_COMPARE_MATCH_C:
#if defined (ETIMSK)
            // Compare match 3 channel flags for timer 1 and timer 3 both placed in ETIMSK register
            if (value == &TIMSK) {
                value = &ETIMSK;
            }
#endif
            flag = _TIMSKn.OCIEnC;
            break;
        case HW_TIMER_INTERRUPT_CAPTURE_INPUT:
            flag = _TIMSKn.ICIEn;
            break;
    }

    if (flag >= 0) {
        *value = (uint8_t) ((*value & ~_BV(flag)) | (enabled << flag));
    }
}

HWTimer8Bit::HWTimer8Bit() = default;

void HWTimer8Bit::setInterruptHandler(HWTimerInterrupt_t interrupt, void (*handler)()) {
    switch (interrupt) {
        case HW_TIMER_INTERRUPT_OVERFLOW:
            _onOverflow = handler;
            break;
        case HW_TIMER_INTERRUPT_COMPARE_MATCH_A:
            _onCompareMatchA = handler;
            break;
        case HW_TIMER_INTERRUPT_COMPARE_MATCH_B:
            _onCompareMatchB = handler;
            break;
        case HW_TIMER_INTERRUPT_COMPARE_MATCH_C:
        case HW_TIMER_INTERRUPT_CAPTURE_INPUT:
            break;
    }
}

HWTimer0Class::HWTimer0Class() : HWTimer8Bit() {
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
void HWTimer0Class::setClockSource(HWTimer0ClockSource_t clockSource) {
    HWTimerClass::setClockSource((HWTimerClockSource_t) ((uint8_t) clockSource));
}
#endif

HWTimer0Class HWTimer0;

#if defined (TIMER0_COMP_vect)
ISR(TIMER0_COMP_vect)
{
    if (HWTimer0._onCompareMatchA) {
        HWTimer0._onCompareMatchA();
    }
}
#else
ISR(TIMER0_COMPA_vect)
{
    if (HWTimer0._onCompareMatchA) {
        HWTimer0._onCompareMatchA();
    }
}
ISR(TIMER0_COMPB_vect)
{
    if (HWTimer0._onCompareMatchB) {
        HWTimer0._onCompareMatchB();
    }
}
#endif
ISR(TIMER0_OVF_vect)
{
    if (HWTimer0._onOverflow) {
        HWTimer0._onOverflow();
    }
}

#if HW_TIMER_8BIT_COUNT == 2
HWTimer2Class::HWTimer2Class() : HWTimer8Bit() {
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
void HWTimer2Class::setClockSource(HWTimer2ClockSource_t clockSource) {
    HWTimerClass::setClockSource((HWTimerClockSource_t) ((uint8_t) clockSource));
}
#endif

HWTimer2Class HWTimer2;

#if defined (TIMER2_COMP_vect)
ISR(TIMER2_COMP_vect)
{
    if (HWTimer2._onCompareMatchA) {
        HWTimer2._onCompareMatchA();
    }
}
#else
ISR(TIMER2_COMPA_vect)
{
    if (HWTimer2._onCompareMatchA) {
        HWTimer2._onCompareMatchA();
    }
}
ISR(TIMER2_COMPB_vect)
{
    if (HWTimer2._onCompareMatchB) {
        HWTimer2._onCompareMatchB();
    }
}
#endif
ISR(TIMER2_OVF_vect)
{
    if (HWTimer2._onOverflow) {
        HWTimer2._onOverflow();
    }
}
#endif

HWTimer16Bit::HWTimer16Bit() = default;

void HWTimer16Bit::setInterruptHandler(HWTimerInterrupt_t interrupt, void (*handler)()) {
    switch (interrupt) {
        case HW_TIMER_INTERRUPT_OVERFLOW:
            _onOverflow = handler;
            break;
        case HW_TIMER_INTERRUPT_COMPARE_MATCH_A:
            _onCompareMatchA = handler;
            break;
        case HW_TIMER_INTERRUPT_COMPARE_MATCH_B:
            _onCompareMatchB = handler;
            break;
        case HW_TIMER_INTERRUPT_COMPARE_MATCH_C:
            _onCompareMatchC = handler;
            break;
        case HW_TIMER_INTERRUPT_CAPTURE_INPUT:
            _onCaptureInput = handler;
            break;
    }
}

HWTimer1Class::HWTimer1Class(): HWTimer16Bit() {
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

HWTimer1Class HWTimer1;

ISR(TIMER1_COMPA_vect)
{
    if (HWTimer1._onCompareMatchA) {
        HWTimer1._onCompareMatchA();
    }
}
ISR(TIMER1_COMPB_vect)
{
    if (HWTimer1._onCompareMatchB) {
        HWTimer1._onCompareMatchB();
    }
}
#if defined (TIMER1_COMPC_vect)
ISR(TIMER1_COMPC_vect)
{
    if (HWTimer1._onCompareMatchC) {
        HWTimer1._onCompareMatchC();
    }
}
#endif
ISR(TIMER1_OVF_vect)
{
    if (HWTimer1._onOverflow) {
        HWTimer1._onOverflow();
    }
}
ISR(TIMER1_CAPT_vect)
{
    if (HWTimer1._onCaptureInput) {
        HWTimer1._onCaptureInput();
    }
}

#if (HW_TIMER_16BIT_COUNT > 1)
HWTimer3Class::HWTimer3Class(): HWTimer16Bit() {
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

HWTimer3Class HWTimer3;

ISR(TIMER3_COMPA_vect)
{
    if (HWTimer3._onCompareMatchA) {
        HWTimer3._onCompareMatchA();
    }
}
ISR(TIMER3_COMPB_vect)
{
    if (HWTimer3._onCompareMatchB) {
        HWTimer3._onCompareMatchB();
    }
}
ISR(TIMER3_COMPC_vect)
{
    if (HWTimer3._onCompareMatchC) {
        HWTimer3._onCompareMatchC();
    }
}
ISR(TIMER3_OVF_vect)
{
    if (HWTimer3._onOverflow) {
        HWTimer3._onOverflow();
    }
}
ISR(TIMER3_CAPT_vect)
{
    if (HWTimer3._onCaptureInput) {
        HWTimer3._onCaptureInput();
    }
}
#endif

#if (HW_TIMER_16BIT_COUNT > 2)
HWTimer4Class::HWTimer4Class(): HWTimer16Bit() {
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

HWTimer4Class HWTimer4;

ISR(TIMER4_COMPA_vect)
{
    if (HWTimer4._onCompareMatchA) {
        HWTimer4._onCompareMatchA();
    }
}
ISR(TIMER4_COMPB_vect)
{
    if (HWTimer4._onCompareMatchB) {
        HWTimer4._onCompareMatchB();
    }
}
ISR(TIMER4_COMPC_vect)
{
    if (HWTimer4._onCompareMatchC) {
        HWTimer4._onCompareMatchC();
    }
}
ISR(TIMER4_OVF_vect)
{
    if (HWTimer4._onOverflow) {
        HWTimer4._onOverflow();
    }
}
ISR(TIMER4_CAPT_vect)
{
    if (HWTimer4._onCaptureInput) {
        HWTimer4._onCaptureInput();
    }
}

HWTimer5Class::HWTimer5Class(): HWTimer16Bit() {
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

HWTimer5Class HWTimer5;

ISR(TIMER5_COMPA_vect)
{
    if (HWTimer5._onCompareMatchA) {
        HWTimer5._onCompareMatchA();
    }
}
ISR(TIMER5_COMPB_vect)
{
    if (HWTimer5._onCompareMatchB) {
        HWTimer5._onCompareMatchB();
    }
}
ISR(TIMER5_COMPC_vect)
{
    if (HWTimer5._onCompareMatchC) {
        HWTimer5._onCompareMatchC();
    }
}
ISR(TIMER5_OVF_vect)
{
    if (HWTimer5._onOverflow) {
        HWTimer5._onOverflow();
    }
}
ISR(TIMER5_CAPT_vect)
{
    if (HWTimer5._onCaptureInput) {
        HWTimer5._onCaptureInput();
    }
}
#endif
