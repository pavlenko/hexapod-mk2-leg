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

// TODO enable interrupts by compare (a/b) and overflow
class Timer0Class
{
public:
    /**
     * Overflow interrupt handler
     */
    void (*onOverflow) ();

    Timer0Class();
    /**
     * Set timer clock source bits
     *
     * @param clockSource
     */
    void setClockSource(ClockSource clockSource);

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
};

extern Timer0Class Timer0;

#endif //TIMER_H
