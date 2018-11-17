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