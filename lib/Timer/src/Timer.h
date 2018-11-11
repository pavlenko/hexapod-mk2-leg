#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include <stdint.h>

#if defined (TCCR5A)
// Devices with Timer 5, have 4 16 bit timers (i.e Timer 1, 3, 4, 5)
#define TIMER_16BIT_COUNT 4
#elif defined (TCCR3A)
// Some devices have 2 16 bit timers (i.e. Timer 1, 3)
#define TIMER_16BIT_COUNT 2
#else
// The remainder only have 1 16 bit timer (i.e. Timer 1)
#define TIMER_16BIT_COUNT 1
#endif

#if defined (TCCR2) || defined (TCCR2A)
// A majority of the devices have Timer 2 (i.e Timer 0 and Asynchronous Timer 2)
#define TIMER_8BIT_COUNT 2
#else
// The remainder only have 1 8 bit timer (i.e. Timer 0)
#define TIMER_8BIT_COUNT 1
#endif

#if defined (TIMER0_COMP_vect)
ISR(TIMER0_COMP_vect);
#else
ISR(TIMER0_COMPA_vect);
ISR(TIMER0_COMPB_vect);
#endif
ISR(TIMER0_OVF_vect);

#if (TIMER_8BIT_COUNT == 2)
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

#if (TIMER_16BIT_COUNT > 1)
ISR(TIMER3_COMPA_vect);
ISR(TIMER3_COMPB_vect);
ISR(TIMER3_COMPC_vect);
ISR(TIMER3_OVF_vect);
ISR(TIMER3_CAPT_vect);
#endif

#if (TIMER_16BIT_COUNT > 2)
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
    TIMER_CLOCK_NONE,
    TIMER_CLOCK_NO_DIVIDE,
    TIMER_CLOCK_DIVIDE_BY_8,
    TIMER_CLOCK_DIVIDE_BY_64,
    TIMER_CLOCK_DIVIDE_BY_256,
    TIMER_CLOCK_DIVIDE_BY_1024,
    TIMER_CLOCK_EXTERNAL_BY_FALLING_EDGE,
    TIMER_CLOCK_EXTERNAL_BY_RISING_EDGE
} TimerClockSource_t;

#if defined (__AVR_ATmega128__) || defined (__AVR_ATmega64__)
typedef enum {
    TIMER0_CLOCK_NONE,
    TIMER0_CLOCK_NO_DIVIDE,
    TIMER0_CLOCK_DIVIDE_BY_8,
    TIMER0_CLOCK_DIVIDE_BY_32,
    TIMER0_CLOCK_DIVIDE_BY_64,
    TIMER0_CLOCK_DIVIDE_BY_128,
    TIMER0_CLOCK_DIVIDE_BY_256,
    TIMER0_CLOCK_DIVIDE_BY_1024
} Timer0ClockSource_t;
#else
typedef enum {
    TIMER2_CLOCK_NONE,
    TIMER2_CLOCK_NO_DIVIDE,
    TIMER2_CLOCK_DIVIDE_BY_8,
    TIMER2_CLOCK_DIVIDE_BY_32,
    TIMER2_CLOCK_DIVIDE_BY_64,
    TIMER2_CLOCK_DIVIDE_BY_128,
    TIMER2_CLOCK_DIVIDE_BY_256,
    TIMER2_CLOCK_DIVIDE_BY_1024
} Timer2ClockSource_t;
#endif

typedef enum {
    TIMER_INTERRUPT_OVERFLOW,
    TIMER_INTERRUPT_COMPARE_MATCH_A,
    TIMER_INTERRUPT_COMPARE_MATCH_B,
    TIMER_INTERRUPT_COMPARE_MATCH_C,
    TIMER_INTERRUPT_CAPTURE_INPUT
} TimerInterrupt_t;

typedef enum {
    TIMER_8BIT_COUNT_NORMAL,
    TIMER_8BIT_COUNT_PWM_PHASE_CORRECT,
    TIMER_8BIT_COUNT_CTC_BY_OCR,
    TIMER_8BIT_COUNT_FAST_PWM
#if defined(TIMSK)
    ,TIMER_8BIT_COUNT_RESERVED_1
    ,TIMER_8BIT_COUNT_PWM_PHASE_CORRECT_BY_OCR
    ,TIMER_8BIT_COUNT_RESERVED_2
    ,TIMER_8BIT_COUNT_FAST_PWM_BY_OCR
#endif
} Timer8BitCountMode_t;

typedef enum {
    TIMER_16BIT_COUNT_NORMAL,
    TIMER_16BIT_COUNT_PWM_PHASE_CORRECT_8,
    TIMER_16BIT_COUNT_PWM_PHASE_CORRECT_9,
    TIMER_16BIT_COUNT_PWM_PHASE_CORRECT_10,
    TIMER_16BIT_COUNT_CTC_BY_OCR,
    TIMER_16BIT_COUNT_FAST_PWM_8,
    TIMER_16BIT_COUNT_FAST_PWM_9,
    TIMER_16BIT_COUNT_FAST_PWM_10,
    TIMER_16BIT_COUNT_PWM_PHASE_FREQ_CORRECT_BY_ICR,
    TIMER_16BIT_COUNT_PWM_PHASE_FREQ_CORRECT_BY_OCR,
    TIMER_16BIT_COUNT_PWM_PHASE_CORRECT_BY_ICR,
    TIMER_16BIT_COUNT_PWM_PHASE_CORRECT_BY_OCR,
    TIMER_16BIT_COUNT_CTC_BY_ICR,
    TIMER_16BIT_COUNT_RESERVED,
    TIMER_16BIT_COUNT_FAST_PWM_BY_ICR,
    TIMER_16BIT_COUNT_FAST_PWM_BY_OCR,
} Timer16BitCountMode_t;

typedef enum {
    TIMER_OUTPUT_DISABLED,
    TIMER_OUTPUT_TOGGLE,
    TIMER_OUTPUT_CLEAR_UP,
    TIMER_OUTPUT_SET_UP
} TimerOutputMode_t;

typedef struct {
    volatile uint8_t *value;
    int8_t TOIEn: 4;
    int8_t OCIEnA: 4;
    int8_t OCIEnB: 4;
    int8_t OCIEnC: 4;
    int8_t ICIEn: 4;
} TIMSK_t;

class TimerClass {
protected:
    // Control registers
    TIMSK_t _TIMSKn;
    volatile uint8_t *_TCCRnA;
    volatile uint8_t *_TCCRnB;
public:
    TimerClass();
    void setClockSource(TimerClockSource_t clockSource);
    void setInterruptEnabled(TimerInterrupt_t interrupt, bool enabled);
};

class Timer8Bit: public TimerClass {
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
    Timer8Bit();
    void setInterruptHandler(TimerInterrupt_t interrupt, void (*handler)());
    void setCountMode(Timer8BitCountMode_t mode);
    inline void setOutputModeA(TimerOutputMode_t mode);
    inline void setOutputModeB(TimerOutputMode_t mode);
};

class Timer0Class: public Timer8Bit {
#if defined (TIMER0_COMP_vect)
    friend void TIMER0_COMP_vect();
#else
    friend void TIMER0_COMPA_vect();
    friend void TIMER0_COMPB_vect();
#endif
    friend void TIMER0_OVF_vect();
public:
    Timer0Class();
#if defined (__AVR_ATmega128__) || defined (__AVR_ATmega64__)
    void setClockSource(Timer0ClockSource_t clockSource);
#endif
};

extern Timer0Class Timer0;

#if TIMER_8BIT_COUNT == 2
class Timer2Class: public Timer8Bit {
#if defined (TIMER2_COMP_vect)
    friend void TIMER2_COMP_vect();
#else
    friend void TIMER2_COMPA_vect();
    friend void TIMER2_COMPB_vect();
#endif
    friend void TIMER2_OVF_vect();
public:
    Timer2Class();
#if defined (__AVR_ATmega128__) || defined (__AVR_ATmega64__)
#else
    void setClockSource(Timer2ClockSource_t clockSource);
#endif
};

extern Timer2Class Timer2;
#endif

class Timer16Bit: public TimerClass {
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
    Timer16Bit();
    void setInterruptHandler(TimerInterrupt_t interrupt, void (*handler)());
    void setCountMode(Timer16BitCountMode_t mode);
    inline void setOutputModeA(TimerOutputMode_t mode);
    inline void setOutputModeB(TimerOutputMode_t mode);
    inline void setOutputModeC(TimerOutputMode_t mode);
};

class Timer1Class: public Timer16Bit {
    friend void TIMER1_COMPA_vect();
    friend void TIMER1_COMPB_vect();
#if defined (TIMER1_COMPC_vect)
    friend void TIMER1_COMPC_vect();
#endif
    friend void TIMER1_OVF_vect();
    friend void TIMER1_CAPT_vect();
public:
    Timer1Class();
};

extern Timer1Class Timer1;

#if (TIMER_16BIT_COUNT > 1)
class Timer3Class: public Timer16Bit {
    friend void TIMER3_COMPA_vect();
    friend void TIMER3_COMPB_vect();
    friend void TIMER3_COMPC_vect();
    friend void TIMER3_OVF_vect();
    friend void TIMER3_CAPT_vect();
public:
    Timer3Class();
};

extern Timer3Class Timer3;
#endif

#if (TIMER_16BIT_COUNT > 2)
class Timer4Class: public Timer16Bit {
    friend void TIMER4_COMPA_vect();
    friend void TIMER4_COMPB_vect();
    friend void TIMER4_COMPC_vect();
    friend void TIMER4_OVF_vect();
    friend void TIMER4_CAPT_vect();
public:
    Timer4Class();
};

extern Timer4Class Timer4;

class Timer5Class: public Timer16Bit {
    friend void TIMER5_COMPA_vect();
    friend void TIMER5_COMPB_vect();
    friend void TIMER5_COMPC_vect();
    friend void TIMER5_OVF_vect();
    friend void TIMER5_CAPT_vect();
public:
    Timer5Class();
};

extern Timer5Class Timer5;
#endif

#endif //TIMER_H
