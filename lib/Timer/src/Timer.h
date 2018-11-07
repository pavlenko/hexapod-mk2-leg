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

enum TimerClockSource {
    TIMER_CLOCK_NONE,
    TIMER_CLOCK_NO_DIVIDE,
    TIMER_CLOCK_DIVIDE_BY_8,
    TIMER_CLOCK_DIVIDE_BY_64,
    TIMER_CLOCK_DIVIDE_BY_256,
    TIMER_CLOCK_DIVIDE_BY_1024,
    TIMER_CLOCK_EXTERNAL_BY_FALLING_EDGE,
    TIMER_CLOCK_EXTERNAL_BY_RISING_EDGE
};

enum TimerWaveformGenerationMode {
    TIMER_WGM_NORMAL,
    TIMER_WGM_PWM_PHASE_CORRECT_8,
    TIMER_WGM_PWM_PHASE_CORRECT_9,
    TIMER_WGM_PWM_PHASE_CORRECT_10,
    TIMER_WGM_CTC_BY_OCR,
    TIMER_WGM_FAST_PWM_8,
    TIMER_WGM_FAST_PWM_9,
    TIMER_WGM_FAST_PWM_10,
    TIMER_WGM_PWM_PHASE_FREQ_CORRECT_BY_ICR,
    TIMER_WGM_PWM_PHASE_FREQ_CORRECT_BY_OCR,
    TIMER_WGM_PWM_PHASE_CORRECT_BY_ICR,
    TIMER_WGM_PWM_PHASE_CORRECT_BY_OCR,
    TIMER_WGM_CTC_BY_ICR,
    TIMER_WGM_RESERVED,
    TIMER_WGM_FAST_PWM_BY_ICR,
    TIMER_WGM_FAST_PWM_BY_OCR,
};

enum TimerOutputCompareMode {
    TIMER_OUTPUT_COMPARE_DISABLED,
    TIMER_OUTPUT_COMPARE_TOGGLE,
    TIMER_OUTPUT_COMPARE_CLEAR_UP,
    TIMER_OUTPUT_COMPARE_SET_UP
};

typedef void (*TimerInterruptHandler_t) ();

class Timer8BitClass {
protected:
    volatile uint8_t *TCCR_A;
    volatile uint8_t *TCCR_B;
    volatile uint8_t *TCNT;

    volatile TimerInterruptHandler_t handlers[1];//TODO size???
public:
    /**
     * @param clockSource
     */
    void setClockSource(TimerClockSource clockSource);

    /**
     * @return
     */
    uint8_t getCounterValue();

    /**
     * @param value
     */
    void setCounterValue(uint8_t value);

    /**
     * @param code
     * @param handlerPtr
     */
    void setInterruptHandler(uint8_t code, TimerInterruptHandler_t handlerPtr);

    /**
     * @param code
     */
    void triggerInterrupt(uint8_t code);
};

class Timer16BitClass {
protected:
    volatile uint8_t  *TCCR_A;
    volatile uint8_t  *TCCR_B;
    volatile uint16_t *TCNT;
    volatile uint16_t *OCR_A;
    volatile uint16_t *OCR_B;
    volatile uint16_t *ICR;

    volatile TimerInterruptHandler_t handlers[1];//TODO size???
public:
    /**
     * @param clockSource
     */
    void setClockSource(TimerClockSource clockSource);

    /**
     * @param mode
     */
    void setWaveformGenerationMode(TimerWaveformGenerationMode mode);

    /**
     * @param mode
     */
    void setOutputCompareModeA(TimerOutputCompareMode mode);

    /**
     * @param mode
     */
    void setOutputCompareModeB(TimerOutputCompareMode mode);

    /**
     * @return
     */
    uint16_t getCounterValue();

    /**
     * @param value
     */
    void setCounterValue(uint16_t value);

    /**
     * @return
     */
    uint16_t getCompareValueA();

    /**
     * @param value
     */
    void setCompareValueA(uint16_t value);

    /**
     * @return
     */
    uint16_t getCompareValueB();

    /**
     * @param value
     */
    void setCompareValueB(uint16_t value);

    /**
     * @return
     */
    uint16_t getCaptureValue();

    /**
     * @param value
     */
    void setCaptureValue(uint16_t value);

    /**
     * @param code
     * @param handlerPtr
     */
    void setInterruptHandler(uint8_t code, TimerInterruptHandler_t handlerPtr);

    /**
     * @param code
     */
    void triggerInterrupt(uint8_t code);
};

class Timer1Class : Timer16BitClass
{
public:
    Timer1Class();
};

extern Timer1Class Timer1();

#endif //TIMER_H
