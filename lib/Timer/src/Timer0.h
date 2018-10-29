#ifndef TIMER0_H
#define TIMER0_H

#include <avr/io.h>
#include <avr/sfr_defs.h>

#ifndef TCNT0
#error "Current CPU does not support timer 0"
#endif

//TODO not all processor support CTC in timer 0
#ifndef TIMER0_ENABLE_CTC
#define TIMER0_ENABLE_CTC 1
#endif

#define TIMER0_CLOCK_MASK 0b11111000

typedef void (*Timer0InterruptHandler_t) ();

enum Timer0ClockSource {
    TIMER0_CLOCK_NONE,
    TIMER0_CLOCK_NO_DIVIDE,
    TIMER0_CLOCK_DIVIDE_BY_8,
    TIMER0_CLOCK_DIVIDE_BY_64,
    TIMER0_CLOCK_DIVIDE_BY_256,
    TIMER0_CLOCK_DIVIDE_BY_1024,
    TIMER0_CLOCK_EXTERNAL_BY_FALLING_EDGE,
    TIMER0_CLOCK_EXTERNAL_BY_RISING_EDGE
};

enum Timer0Interrupt {
    TIMER0_OVERFLOW_INTERRUPT
#if defined(OCR0A)
    ,TIMER0_COMPARE_INTERRUPT
    ,TIMER0_COMPARE_B_INTERRUPT
#elif defined(OCR0)
    ,TIMER0_COMPARE_INTERRUPT
#endif
    ,TIMER0_INTERRUPT_COUNT
};

class Timer0Class
{
private:
    volatile Timer0InterruptHandler_t handlers[TIMER0_INTERRUPT_COUNT];
public:
    Timer0Class();

    /**
     * Set timer clock source bits
     *
     * @param clockSource
     */
    void setClockSource(Timer0ClockSource clockSource);

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

#endif //TIMER0_H
