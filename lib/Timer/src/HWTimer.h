#ifndef HW_TIMER_H
#define HW_TIMER_H

//#define __AVR_ATmega8__ 1
//#define __AVR_ATmega32__ 1
//#define __AVR_ATmega128__ 1
#define __AVR_ATmega2560__ 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include <stdint.h>

#define HW_TIMER_INTERRUPT_OVERFLOW        0
#define HW_TIMER_INTERRUPT_COMPARE_MATCH_A 1
#define HW_TIMER_INTERRUPT_COMPARE_MATCH_B 2
#define HW_TIMER_INTERRUPT_COMPARE_MATCH_C 3
#define HW_TIMER_INTERRUPT_CAPTURE_INPUT   4

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

#if defined (TIMER0_COMP_vect)
ISR(TIMER0_COMP_vect);
#else
ISR(TIMER0_COMPA_vect);
ISR(TIMER0_COMPB_vect);
#endif
ISR(TIMER0_OVF_vect);

#if (HW_TIMER_8BIT_COUNT == 2)
#if defined (TIMER2_COMP_vect)
ISR(TIMER2_COMP_vect);
#else
ISR(TIMER2_COMPA_vect);
ISR(TIMER2_COMPB_vect);
#endif
ISR(TIMER2_OVF_vect);
#endif

ISR(TIMER1_COMPA_vect);
ISR(TIMER1_COMPB_vect);
#if defined (TIMER1_COMPC_vect)
ISR(TIMER1_COMPC_vect);
#endif
ISR(TIMER1_OVF_vect);
ISR(TIMER1_CAPT_vect);

#if (HW_TIMER_16BIT_COUNT > 1)
ISR(TIMER3_COMPA_vect);
ISR(TIMER3_COMPB_vect);
ISR(TIMER3_COMPC_vect);
ISR(TIMER3_OVF_vect);
ISR(TIMER3_CAPT_vect);
#endif

#if (HW_TIMER_16BIT_COUNT > 2)
ISR(TIMER4_COMPA_vect);
ISR(TIMER4_COMPB_vect);
ISR(TIMER4_COMPC_vect);
ISR(TIMER4_OVF_vect);
ISR(TIMER4_CAPT_vect);

ISR(TIMER5_COMPA_vect);
ISR(TIMER5_COMPB_vect);
ISR(TIMER5_COMPC_vect);
ISR(TIMER5_OVF_vect);
ISR(TIMER5_CAPT_vect);
#endif

typedef struct {
    volatile uint8_t *value;
    int8_t TOIEn: 4;
    int8_t OCIEnA: 4;
    int8_t OCIEnB: 4;
    int8_t OCIEnC: 4;
    int8_t ICIEn: 4;
} TIMSK_t;

class HWTimerClass {
protected:
    // Control registers
    TIMSK_t _TIMSKn;
    volatile uint8_t *_TCCRnA;
    volatile uint8_t *_TCCRnB;
public:
    HWTimerClass();
    void setInterruptEnabled(uint8_t interrupt, bool enabled);
};

class HWTimer8Bit: public HWTimerClass {
protected:
    uint8_t _channelCount;
    // Value registers
    volatile uint8_t *_TCNTn;
    volatile uint8_t *_OCRnA;
    volatile uint8_t *_OCRnB;
    // User function pointers
    void (*_onOverflow)();
    void (*_onCompareMatchA)();
    void (*_onCompareMatchB)();
public:
    HWTimer8Bit();
};

class HWTimer0Class: public HWTimer8Bit {
#if defined (TIMER0_COMP_vect)
    friend void TIMER0_COMP_vect();
#else
    friend void TIMER0_COMPA_vect();
    friend void TIMER0_COMPB_vect();
#endif
    friend void TIMER0_OVF_vect();
public:
    HWTimer0Class();
};

extern HWTimer0Class HWTimer0;

#if HW_TIMER_8BIT_COUNT == 2
class HWTimer2Class: public HWTimer8Bit {
#if defined (TIMER2_COMP_vect)
    friend void TIMER2_COMP_vect();
#else
    friend void TIMER2_COMPA_vect();
    friend void TIMER2_COMPB_vect();
#endif
    friend void TIMER2_OVF_vect();
public:
    HWTimer2Class();
};

extern HWTimer2Class HWTimer2;
#endif

class HWTimer16Bit: public HWTimerClass {
protected:
    uint8_t _channelCount;
    // Value registers
    volatile uint16_t *_TCNTn;
    volatile uint16_t *_OCRnA;
    volatile uint16_t *_OCRnB;
    volatile uint16_t *_OCRnC;
    // User function pointers
    void (*_onOverflow)();
    void (*_onCompareMatchA)();
    void (*_onCompareMatchB)();
    void (*_onCompareMatchC)();
    void (*_onCaptureInput)();
public:
    HWTimer16Bit();
};

class HWTimer1Class: public HWTimer16Bit {
    friend void TIMER1_COMPA_vect();
    friend void TIMER1_COMPB_vect();
#if defined (TIMER1_COMPC_vect)
    friend void TIMER1_COMPC_vect();
#endif
    friend void TIMER1_OVF_vect();
    friend void TIMER1_CAPT_vect();
public:
    HWTimer1Class();
};

extern HWTimer1Class HWTimer1;

#if (HW_TIMER_16BIT_COUNT > 1)
class HWTimer3Class: public HWTimer16Bit {
    friend void TIMER3_COMPA_vect();
    friend void TIMER3_COMPB_vect();
    friend void TIMER3_COMPC_vect();
    friend void TIMER3_OVF_vect();
    friend void TIMER3_CAPT_vect();
public:
    HWTimer3Class();
};

extern HWTimer3Class HWTimer3;
#endif

#if (HW_TIMER_16BIT_COUNT > 2)
class HWTimer4Class: public HWTimer16Bit {
    friend void TIMER4_COMPA_vect();
    friend void TIMER4_COMPB_vect();
    friend void TIMER4_COMPC_vect();
    friend void TIMER4_OVF_vect();
    friend void TIMER4_CAPT_vect();
public:
    HWTimer4Class();
};

extern HWTimer4Class HWTimer4;

class HWTimer5Class: public HWTimer16Bit {
    friend void TIMER5_COMPA_vect();
    friend void TIMER5_COMPB_vect();
    friend void TIMER5_COMPC_vect();
    friend void TIMER5_OVF_vect();
    friend void TIMER5_CAPT_vect();
public:
    HWTimer5Class();
};

extern HWTimer5Class HWTimer5;
#endif

#endif //HW_TIMER_H
