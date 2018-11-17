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

#endif //TIMER_H
