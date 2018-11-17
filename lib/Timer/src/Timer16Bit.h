#ifndef TIMER16BIT_H
#define TIMER16BIT_H

#include "Timer.h"

#include <stdint.h>

class Timer16BitClass: public TimerClass {
protected:
    uint8_t _channelCount;
    // User function pointers
    void (*_onOverflow)();
    void (*_onCompareMatchA)();
    void (*_onCompareMatchB)();
    void (*_onCompareMatchC)();
    void (*_onCaptureInput)();
public:
    // Value registers
    volatile uint16_t *TCNTn;
    volatile uint16_t *OCRnA;
    volatile uint16_t *OCRnB;
    volatile uint16_t *OCRnC;
    Timer16BitClass();
    void setInterruptHandler(TimerInterrupt_t interrupt, void (*handler)());
    void setCountMode(Timer16BitCountMode_t mode);
    inline void setOutputModeA(TimerOutputMode_t mode);
    inline void setOutputModeB(TimerOutputMode_t mode);
    inline void setOutputModeC(TimerOutputMode_t mode);
};

#endif //TIMER16BIT_H
