#include "Timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include <stdint.h>

TimerClass::TimerClass() = default;

void TimerClass::setClockSource(TimerClockSource_t clockSource) {
    *_TCCRnB = (uint8_t) ((*_TCCRnB & 0b11111000) | clockSource);
}

void TimerClass::setInterruptEnabled(TimerInterrupt_t interrupt, bool enabled) {
    int8_t flag = -1;

    volatile uint8_t *value = _TIMSKn.value;

    switch (interrupt) {
        case TIMER_INTERRUPT_OVERFLOW:
            flag = _TIMSKn.TOIEn;
            break;
        case TIMER_INTERRUPT_COMPARE_MATCH_A:
            flag = _TIMSKn.OCIEnA;
            break;
        case TIMER_INTERRUPT_COMPARE_MATCH_B:
            flag = _TIMSKn.OCIEnB;
            break;
        case TIMER_INTERRUPT_COMPARE_MATCH_C:
#if defined (ETIMSK)
            // Compare match 3 channel flags for timer 1 and timer 3 both placed in ETIMSK register
            if (value == &TIMSK) {
                value = &ETIMSK;
            }
#endif
            flag = _TIMSKn.OCIEnC;
            break;
        case TIMER_INTERRUPT_CAPTURE_INPUT:
            flag = _TIMSKn.ICIEn;
            break;
    }

    if (flag >= 0) {
        *value = (uint8_t) ((*value & ~_BV(flag)) | (enabled << flag));
    }
}

Timer8Bit::Timer8Bit() = default;

void Timer8Bit::setInterruptHandler(TimerInterrupt_t interrupt, void (*handler)()) {
    switch (interrupt) {
        case TIMER_INTERRUPT_OVERFLOW:
            _onOverflow = handler;
            break;
        case TIMER_INTERRUPT_COMPARE_MATCH_A:
            _onCompareMatchA = handler;
            break;
        case TIMER_INTERRUPT_COMPARE_MATCH_B:
            _onCompareMatchB = handler;
            break;
        case TIMER_INTERRUPT_COMPARE_MATCH_C:
        case TIMER_INTERRUPT_CAPTURE_INPUT:
            break;
    }
}

void Timer8Bit::setCountMode(Timer8BitCountMode_t mode) {
    // For devices with global TIMSK, 8 bit timers have:
    // WGMn0 in position 6 of TCCRn register
    // WGMn1 in position 3 of TCCRn register
#if defined (TIMSK)
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b10110111) | ((mode & 0b00000001) << 6) | ((mode & 0b00000010) << 2));
#else
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b11111100) | (mode & 0b00000011));
    *_TCCRnB = (uint8_t) ((*_TCCRnB & 0b11100111) | ((mode & 0b00001100) << 1));
#endif
}

void Timer8Bit::setOutputModeA(TimerOutputMode_t mode) {
#if defined (TIMSK)
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b11001111) | (mode << 4));
#else
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b00111111) | (mode << 6));
#endif
}

void Timer8Bit::setOutputModeB(TimerOutputMode_t mode) {
#if defined(TIMSK0)
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b11001111) | (mode << 4));
#endif
}

Timer0Class::Timer0Class() : Timer8Bit() {
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

#if TIMER_8BIT_COUNT == 2
Timer2Class::Timer2Class() : Timer8Bit() {
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
#endif

Timer16Bit::Timer16Bit() = default;

void Timer16Bit::setInterruptHandler(TimerInterrupt_t interrupt, void (*handler)()) {
    switch (interrupt) {
        case TIMER_INTERRUPT_OVERFLOW:
            _onOverflow = handler;
            break;
        case TIMER_INTERRUPT_COMPARE_MATCH_A:
            _onCompareMatchA = handler;
            break;
        case TIMER_INTERRUPT_COMPARE_MATCH_B:
            _onCompareMatchB = handler;
            break;
        case TIMER_INTERRUPT_COMPARE_MATCH_C:
            _onCompareMatchC = handler;
            break;
        case TIMER_INTERRUPT_CAPTURE_INPUT:
            _onCaptureInput = handler;
            break;
    }
}

void Timer16Bit::setCountMode(Timer16BitCountMode_t mode) {
    // WGMn3 and WGMn2 are in positions 4 and 3, respectively, in TCCRnB
    // WGMn1 and WGNn0 are in positions 1 and 0, respectively, in TCCRnA
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b11111100) | (mode & 0b00000011));
    *_TCCRnB = (uint8_t) ((*_TCCRnB & 0b11100111) | ((mode & 0b00001100) << 1));
}

void Timer16Bit::setOutputModeA(TimerOutputMode_t mode) {
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b00111111) | (mode << 6));
}

void Timer16Bit::setOutputModeB(TimerOutputMode_t mode) {
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b11001111) | (mode << 4));
}

void Timer16Bit::setOutputModeC(TimerOutputMode_t mode) {
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b11110011) | (mode << 2));
}

Timer1Class::Timer1Class(): Timer16Bit() {
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

#if (TIMER_16BIT_COUNT > 1)
Timer3Class::Timer3Class(): Timer16Bit() {
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
#endif

#if (TIMER_16BIT_COUNT > 2)
Timer4Class::Timer4Class(): Timer16Bit() {
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

Timer5Class::Timer5Class(): Timer16Bit() {
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
#endif
