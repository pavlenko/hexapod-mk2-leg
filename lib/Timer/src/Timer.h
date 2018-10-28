#ifndef TIMER_H
#define TIMER_H

//#define __AVR_ATmega2560__ 1 // For test only

#include <avr/io.h>
#include <avr/sfr_defs.h>

// ATMEGA 48,88,168: TCCR0A, TCCR0B, TCNT0, OCR0A, OCR0B, TIMSK0, TIFR0
// ATMEGA 8L: TCCR0, TCNT0, TIMSK, TIFR <-- not supported
// ATMEGA 16a: TCCR0, TCNT0, OCR0, TIMSK, TIFR
// ATMEGA 32L: TCCR0, TCNT0, OCR0, TIMSK, TIFR
// ATMEGA 64L: TCCR0, TCNT0, OCR0, TIMSK, TIFR
// ATMEGA 128L: TCCR0, TCNT0, OCR0, TIMSK, TIFR
// ATMEGA 2560: TCCR0A/B, TCNT0, OCR0A/B, TIMSK0, TIFR0

// Unusable:
#if SIGNATURE_0 == 0x1E && SIGNATURE_1 == 0x92 // ATMEGA 4x
#elif SIGNATURE_0 == 0x1E && SIGNATURE_1 == 0x93// ATMEGA 8x
#elif SIGNATURE_0 == 0x1E && SIGNATURE_1 == 0x94// ATMEGA 16x
#elif SIGNATURE_0 == 0x1E && SIGNATURE_1 == 0x95// ATMEGA 32x
#elif SIGNATURE_0 == 0x1E && SIGNATURE_1 == 0x96// ATMEGA 64x
#elif SIGNATURE_0 == 0x1E && SIGNATURE_1 == 0x97// ATMEGA 128x
#elif SIGNATURE_0 == 0x1E && SIGNATURE_1 == 0x98// ATMEGA 256x
#endif

#define TIMER_CLOCK_MASK 0b11111000

typedef void (*TimerInterruptHandler_t) ();

enum ClockSource {
    TIMER_CLOCK_NONE,
    TIMER_CLOCK_NO_DIVIDE,
    TIMER_CLOCK_DIVIDE_BY_8,
    TIMER_CLOCK_DIVIDE_BY_64,
    TIMER_CLOCK_DIVIDE_BY_256,
    TIMER_CLOCK_DIVIDE_BY_1024,
    TIMER_CLOCK_EXTERNAL_BY_FALLING_EDGE,
    TIMER_CLOCK_EXTERNAL_BY_RISING_EDGE
};

#if defined(TCNT0)

enum Timer0Interrupt {
    TIMER0_OVERFLOW_INTERRUPT
#if defined(OCR0A)
    ,TIMER0_COMPARE_A_INTERRUPT
    ,TIMER0_COMPARE_B_INTERRUPT
#elif defined(OCR0)
    ,TIMER0_COMPARE_INTERRUPT
#endif
    ,TIMER0_INTERRUPT_COUNT
};

class Timer0Class
{
private:
    volatile TimerInterruptHandler_t handlers[TIMER0_INTERRUPT_COUNT];
public:
    Timer0Class();

    /**
     * Set timer clock source bits
     *
     * @param clockSource
     */
    void setClockSource(ClockSource clockSource);

    /**
     * Set or clear interrupt handler for specific interrupt
     *
     * @param interrupt
     * @param handler_ptr
     */
    void setInterruptHandler(Timer0Interrupt interrupt, void (*handler_ptr) ());

    /**
     * Get timer value
     *
     * @return
     */
    uint8_t getValue();

    /**
     * Set timer value
     *
     * @param value
     */
    void setValue(uint8_t value);

    /**
     * Trigger specific interrupt handler
     *
     * @param interrupt
     */
    void triggerInterrupt(Timer0Interrupt interrupt);
};

extern Timer0Class Timer0;

#endif

#if defined(TCNT1)

enum Timer1Interrupt {
    TIMER1_OVERFLOW_INTERRUPT
#if defined(TIMSK1)
    ,TIMER1_COMPARE_A_INTERRUPT
    ,TIMER1_COMPARE_B_INTERRUPT
#if defined(OCIE1C)
    ,TIMER1_COMPARE_C_INTERRUPT
#else
    ,TIMER1_RESERVED_BIT_3
#endif
    ,TIMER1_RESERVED_BIT_4
    ,TIMER1_INPUT_CAPTURE_INTERRUPT
#elif defined(TIMSK)
    ,TIMER1_COMPARE_B_INTERRUPT
    ,TIMER1_COMPARE_A_INTERRUPT
    ,TIMER1_INPUT_CAPTURE_INTERRUPT
#endif
    ,TIMER1_INTERRUPT_COUNT
};

class Timer1Class
{
private:
    volatile TimerInterruptHandler_t handlers[TIMER1_INTERRUPT_COUNT];
public:
    Timer1Class();

    /**
     * Set timer clock source bits
     *
     * @param clockSource
     */
    void setClockSource(ClockSource clockSource);

    /**
     * Set or clear interrupt handler for specific interrupt
     *
     * @param interrupt
     * @param handler_ptr
     */
    void setInterruptHandler(Timer1Interrupt interrupt, void (*handler_ptr) ());

    /**
     * Trigger specific interrupt handler
     *
     * @param interrupt
     */
    void triggerInterrupt(Timer1Interrupt interrupt);
};

extern Timer1Class Timer1;

#endif //TCNT1

#if defined(TCNT2)

enum Timer2Interrupt {
    TIMER2_OVERFLOW_INTERRUPT
#if defined(TIMSK2)
    ,TIMER2_COMPARE_A_INTERRUPT
    ,TIMER2_COMPARE_B_INTERRUPT
#elif defined(TIMSK)
    ,TIMER2_COMPARE_INTERRUPT
#endif
    ,TIMER2_INTERRUPT_COUNT
};

class Timer2Class
{
private:
    volatile TimerInterruptHandler_t handlers[TIMER2_INTERRUPT_COUNT];
public:
    Timer2Class();

    /**
     * Set timer clock source bits
     *
     * @param clockSource
     */
    void setClockSource(ClockSource clockSource);

    /**
     * Set or clear interrupt handler for specific interrupt
     *
     * @param interrupt
     * @param handler_ptr
     */
    void setInterruptHandler(Timer2Interrupt interrupt, void (*handler_ptr) ());

    /**
     * Trigger specific interrupt handler
     *
     * @param interrupt
     */
    void triggerInterrupt(Timer2Interrupt interrupt);
};

extern Timer2Class Timer2;

#endif //TCNT2

#if defined(TCNT3)

enum Timer3Interrupt {
    TIMER3_OVERFLOW_INTERRUPT
    ,TIMER3_INTERRUPT_COUNT
};

class Timer3Class
{
private:
    volatile TimerInterruptHandler_t handlers[TIMER3_INTERRUPT_COUNT];
public:
    Timer3Class();

    /**
     * Set timer clock source bits
     *
     * @param clockSource
     */
    void setClockSource(ClockSource clockSource);

    /**
     * Set or clear interrupt handler for specific interrupt
     *
     * @param interrupt
     * @param handler_ptr
     */
    void setInterruptHandler(Timer3Interrupt interrupt, void (*handler_ptr) ());

    /**
     * Trigger specific interrupt handler
     *
     * @param interrupt
     */
    void triggerInterrupt(Timer3Interrupt interrupt);
};

extern Timer3Class Timer3;

#endif //TCNT3

#if defined(TCNT4)

enum Timer4Interrupt {
    TIMER4_OVERFLOW_INTERRUPT
    ,TIMER4_COMPARE_A_INTERRUPT
    ,TIMER4_COMPARE_B_INTERRUPT
    ,TIMER4_COMPARE_C_INTERRUPT
    ,TIMER4_RESERVED_BIT_4
    ,TIMER4_INPUT_CAPTURE_INTERRUPT
    ,TIMER4_INTERRUPT_COUNT
};

class Timer4Class
{
private:
    volatile TimerInterruptHandler_t handlers[TIMER4_INTERRUPT_COUNT];
public:
    Timer4Class();

    /**
     * Set timer clock source bits
     *
     * @param clockSource
     */
    void setClockSource(ClockSource clockSource);

    /**
     * Set or clear interrupt handler for specific interrupt
     *
     * @param interrupt
     * @param handler_ptr
     */
    void setInterruptHandler(Timer4Interrupt interrupt, void (*handler_ptr) ());

    /**
     * Trigger specific interrupt handler
     *
     * @param interrupt
     */
    void triggerInterrupt(Timer4Interrupt interrupt);
};

extern Timer4Class Timer4;

#endif //TCNT4

#if defined(TCNT5)

enum Timer5Interrupt {
    TIMER5_OVERFLOW_INTERRUPT
    ,TIMER5_COMPARE_A_INTERRUPT
    ,TIMER5_COMPARE_B_INTERRUPT
    ,TIMER5_COMPARE_C_INTERRUPT
    ,TIMER5_RESERVED_BIT_4
    ,TIMER5_INPUT_CAPTURE_INTERRUPT
    ,TIMER5_INTERRUPT_COUNT
};

class Timer5Class
{
private:
    volatile TimerInterruptHandler_t handlers[TIMER5_INTERRUPT_COUNT];
public:
    Timer5Class();
    /**
     * Set timer clock source bits
     *
     * @param clockSource
     */
    void setClockSource(ClockSource clockSource);

    /**
     * Set or clear interrupt handler for specific interrupt
     *
     * @param interrupt
     * @param handler_ptr
     */
    void setInterruptHandler(Timer5Interrupt interrupt, void (*handler_ptr) ());

    /**
     * Trigger specific interrupt handler
     *
     * @param interrupt
     */
    void triggerInterrupt(Timer5Interrupt interrupt);
};

extern Timer5Class Timer5;

#endif //TCNT5

#endif //TIMER_H
