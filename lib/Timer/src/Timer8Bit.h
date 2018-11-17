#ifndef TIMER8BIT_H
#define TIMER8BIT_H

#include "Timer.h"

#include <stdint.h>

class Timer8BitClass: public TimerClass {
protected:
    uint8_t _channelCount;
    // User function pointers
    void (*_onOverflow)();
    void (*_onCompareMatchA)();
    void (*_onCompareMatchB)();
public:
    // Value registers
    volatile uint8_t *TCNTn;
    volatile uint8_t *OCRnA;
    volatile uint8_t *OCRnB;
    Timer8BitClass();
    void setInterruptHandler(TimerInterrupt_t interrupt, void (*handler)());
    void setCountMode(Timer8BitCountMode_t mode);
    inline void setOutputModeA(TimerOutputMode_t mode);
    inline void setOutputModeB(TimerOutputMode_t mode);
};

#endif //TIMER8BIT_H
