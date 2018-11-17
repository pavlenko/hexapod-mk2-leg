#include "Timer8Bit.h"

Timer8BitClass::Timer8BitClass() = default;

void Timer8BitClass::setInterruptHandler(TimerInterrupt_t interrupt, void (*handler)()) {
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

void Timer8BitClass::setCountMode(Timer8BitCountMode_t mode) {
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

void Timer8BitClass::setOutputModeA(TimerOutputMode_t mode) {
#if defined (TIMSK)
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b11001111) | (mode << 4));
#else
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b00111111) | (mode << 6));
#endif
}

void Timer8BitClass::setOutputModeB(TimerOutputMode_t mode) {
#if defined(TIMSK0)
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b11001111) | (mode << 4));
#endif
}