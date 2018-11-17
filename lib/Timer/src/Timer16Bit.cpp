#include "Timer16Bit.h"

Timer16BitClass::Timer16BitClass() = default;

void Timer16BitClass::setInterruptHandler(TimerInterrupt_t interrupt, void (*handler)()) {
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

void Timer16BitClass::setCountMode(Timer16BitCountMode_t mode) {
    // WGMn3 and WGMn2 are in positions 4 and 3, respectively, in TCCRnB
    // WGMn1 and WGNn0 are in positions 1 and 0, respectively, in TCCRnA
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b11111100) | (mode & 0b00000011));
    *_TCCRnB = (uint8_t) ((*_TCCRnB & 0b11100111) | ((mode & 0b00001100) << 1));
}

void Timer16BitClass::setOutputModeA(TimerOutputMode_t mode) {
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b00111111) | (mode << 6));
}

void Timer16BitClass::setOutputModeB(TimerOutputMode_t mode) {
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b11001111) | (mode << 4));
}

void Timer16BitClass::setOutputModeC(TimerOutputMode_t mode) {
    *_TCCRnA = (uint8_t) ((*_TCCRnA & 0b11110011) | (mode << 2));
}
