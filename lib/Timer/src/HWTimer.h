#ifndef HW_TIMER_H
#define HW_TIMER_H

//#define __AVR_ATmega8__ 1
//#define __AVR_ATmega32__ 1
//#define __AVR_ATmega128__ 1
//#define __AVR_ATmega2560__ 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include <stdint.h>

//#define HW_TIMER_INTERRUPT_OVERFLOW        0
//#define HW_TIMER_INTERRUPT_COMPARE_MATCH_A 1
//#define HW_TIMER_INTERRUPT_COMPARE_MATCH_B 2
//#define HW_TIMER_INTERRUPT_COMPARE_MATCH_C 3
//#define HW_TIMER_INTERRUPT_CAPTURE_INPUT   4

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

typedef enum {
    HW_TIMER_CLOCK_NONE,
    HW_TIMER_CLOCK_NO_DIVIDE,
    HW_TIMER_CLOCK_DIVIDE_BY_8,
    HW_TIMER_CLOCK_DIVIDE_BY_64,
    HW_TIMER_CLOCK_DIVIDE_BY_256,
    HW_TIMER_CLOCK_DIVIDE_BY_1024,
    HW_TIMER_CLOCK_EXTERNAL_BY_FALLING_EDGE,
    HW_TIMER_CLOCK_EXTERNAL_BY_RISING_EDGE
} HWTimerClockSource_t;

#if defined (__AVR_ATmega128__) || defined (__AVR_ATmega64__)
typedef enum {
    HW_TIMER0_CLOCK_NONE,
    HW_TIMER0_CLOCK_NO_DIVIDE,
    HW_TIMER0_CLOCK_DIVIDE_BY_8,
    HW_TIMER0_CLOCK_DIVIDE_BY_32,
    HW_TIMER0_CLOCK_DIVIDE_BY_64,
    HW_TIMER0_CLOCK_DIVIDE_BY_128,
    HW_TIMER0_CLOCK_DIVIDE_BY_256,
    HW_TIMER0_CLOCK_DIVIDE_BY_1024
} HWTimer0ClockSource_t;
#else
typedef enum {
    HW_TIMER2_CLOCK_NONE,
    HW_TIMER2_CLOCK_NO_DIVIDE,
    HW_TIMER2_CLOCK_DIVIDE_BY_8,
    HW_TIMER2_CLOCK_DIVIDE_BY_32,
    HW_TIMER2_CLOCK_DIVIDE_BY_64,
    HW_TIMER2_CLOCK_DIVIDE_BY_128,
    HW_TIMER2_CLOCK_DIVIDE_BY_256,
    HW_TIMER2_CLOCK_DIVIDE_BY_1024
} HWTimer2ClockSource_t;
#endif

typedef enum {
    HW_TIMER_INTERRUPT_OVERFLOW,
    HW_TIMER_INTERRUPT_COMPARE_MATCH_A,
    HW_TIMER_INTERRUPT_COMPARE_MATCH_B,
    HW_TIMER_INTERRUPT_COMPARE_MATCH_C,
    HW_TIMER_INTERRUPT_CAPTURE_INPUT
} HWTimerInterrupt_t;

typedef enum {
    HW_TIMER_8BIT_COUNT_NORMAL,
    HW_TIMER_8BIT_COUNT_PWM_PHASE_CORRECT,
    HW_TIMER_8BIT_COUNT_CTC,
    HW_TIMER_8BIT_COUNT_FAST_PWM,
    HW_TIMER_8BIT_COUNT_RESERVED_1,
    HW_TIMER_8BIT_COUNT_PWM_PHASE_CORRECT_BY_OCR,
    HW_TIMER_8BIT_COUNT_RESERVED_2,
    HW_TIMER_8BIT_COUNT_FAST_PWM_BY_OCR
} HWTimer8BitCountMode_t;

typedef enum {
    HW_TIMER_16BIT_COUNT_NORMAL,
    HW_TIMER_16BIT_COUNT_PWM_PHASE_CORRECT_8,
    HW_TIMER_16BIT_COUNT_PWM_PHASE_CORRECT_9,
    HW_TIMER_16BIT_COUNT_PWM_PHASE_CORRECT_10,
    HW_TIMER_16BIT_COUNT_CTC_BY_OCR,
    HW_TIMER_16BIT_COUNT_FAST_PWM_8,
    HW_TIMER_16BIT_COUNT_FAST_PWM_9,
    HW_TIMER_16BIT_COUNT_FAST_PWM_10,
    HW_TIMER_16BIT_COUNT_PWM_PHASE_FREQ_CORRECT_BY_ICR,
    HW_TIMER_16BIT_COUNT_PWM_PHASE_FREQ_CORRECT_BY_OCR,
    HW_TIMER_16BIT_COUNT_PWM_PHASE_CORRECT_BY_ICR,
    HW_TIMER_16BIT_COUNT_PWM_PHASE_CORRECT_BY_OCR,
    HW_TIMER_16BIT_COUNT_CTC_BY_ICR,
    HW_TIMER_16BIT_COUNT_RESERVED,
    HW_TIMER_16BIT_COUNT_FAST_PWM_BY_ICR,
    HW_TIMER_16BIT_COUNT_FAST_PWM_BY_OCR,
} HWTimer16BitCountMode_t;

typedef enum {
    HW_TIMER_OUTPUT_DISABLED,
    HW_TIMER_OUTPUT_TOGGLE,
    HW_TIMER_OUTPUT_CLEAR_UP,
    HW_TIMER_OUTPUT_SET_UP
} HWTimerOutputMode_t;

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
    void setClockSource(HWTimerClockSource_t clockSource);
    void setInterruptEnabled(HWTimerInterrupt_t interrupt, bool enabled);
};

class HWTimer8Bit: public HWTimerClass {
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
    HWTimer8Bit();
    void setInterruptHandler(HWTimerInterrupt_t interrupt, void (*handler)());
    void setCountMode(uint8_t mode);
    inline void setOutputModeA(HWTimerOutputMode_t mode);
    inline void setOutputModeB(HWTimerOutputMode_t mode);
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
#if defined (__AVR_ATmega128__) || defined (__AVR_ATmega64__)
    void setClockSource(HWTimer0ClockSource_t clockSource);
#endif
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
#if defined (__AVR_ATmega128__) || defined (__AVR_ATmega64__)
#else
    void setClockSource(HWTimer2ClockSource_t clockSource);
#endif
};

extern HWTimer2Class HWTimer2;
#endif

class HWTimer16Bit: public HWTimerClass {
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
    HWTimer16Bit();
    void setInterruptHandler(HWTimerInterrupt_t interrupt, void (*handler)());
    void setCountMode(uint8_t mode);
    inline void setOutputModeA(HWTimerOutputMode_t mode);
    inline void setOutputModeB(HWTimerOutputMode_t mode);
    inline void setOutputModeC(HWTimerOutputMode_t mode);
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
