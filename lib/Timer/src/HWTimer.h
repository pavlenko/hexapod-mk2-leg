#ifndef HW_TIMER_H
#define HW_TIMER_H

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

#if defined (TCCR5A)
// Devices with Timer 5, have 4 16 bit timers (i.e Timer 1, 3, 4, 5)
#define HW_TIMER_16BIT_COUNT 4
#elif defined (TCCR3A)
// Some devices have 2 16 bit timers (i.e. Timer 1, 3)
#define HW_TIMER_16BIT_COUNT 2
#else
// The remainder only have 1 16 bit timer (i.e. Timer 1)
#define HW_TIMER_16BIT_COUNT 1
#endif

#if defined (TCCR2) || defined (TCCR2A)
// A majority of the devices have Timer 2 (i.e Timer 0 and Asynchronous Timer 2)
#define HW_TIMER_8BIT_COUNT 2
#else
// The remainder only have 1 8 bit timer (i.e. Timer 0)
#define HW_TIMER_8BIT_COUNT 1
#endif

class HWTimer8Bit {
protected:
    uint8_t _timerNumber;
    uint8_t _channelCount;
    // Control registers
    volatile uint8_t *_TIMSKn;
    volatile uint8_t *_TCCRnA;
    volatile uint8_t *_TCCRnB;
    // Value registers
    volatile uint8_t *_TCNTn;
    volatile uint8_t *_OCRnA;
    volatile uint8_t *_OCRnB;
};

class HWTimer0Class : public HWTimer8Bit {
public:
    HWTimer0Class();
};

extern HWTimer0Class HWTimer0;

#if HW_TIMER_8BIT_COUNT == 2
class HWTimer2Class : public HWTimer8Bit {
public:
    HWTimer2Class();
};

extern HWTimer2Class HWTimer2;
#endif

class HWTimer16Bit {
private:
    uint8_t _timerNumber;
    uint8_t _channelCount;
    // Control registers
    volatile uint8_t *_TIMSKn;
    volatile uint8_t *_TCCRnA;
    volatile uint8_t *_TCCRnB;
    // Value registers
    volatile uint16_t *_TCNTn;
    volatile uint8_t *_TCNTnH;
    volatile uint8_t *_TCNTnL;
    volatile uint16_t *_OCRnA;
    volatile uint8_t *_OCRnAH;
    volatile uint8_t *_OCRnAL;
    volatile uint16_t *_OCRnB;
    volatile uint8_t *_OCRnBH;
    volatile uint8_t *_OCRnBL;
    volatile uint16_t *_OCRnC;
    volatile uint8_t *_OCRnCH;
    volatile uint8_t *_OCRnCL;
};

#endif //HW_TIMER_H
