#ifndef TIMER1_H
#define TIMER1_H

#include <avr/io.h>

#ifndef TCNT1
#error "Current CPU does not support timer 1"
#endif

#define TIMER1_CLOCK_MASK 0b11111000

typedef void (*Timer1InterruptHandler_t) ();

enum Timer1ClockSource {
    TIMER1_CLOCK_NONE,
    TIMER1_CLOCK_NO_DIVIDE,
    TIMER1_CLOCK_DIVIDE_BY_8,
    TIMER1_CLOCK_DIVIDE_BY_64,
    TIMER1_CLOCK_DIVIDE_BY_256,
    TIMER1_CLOCK_DIVIDE_BY_1024,
    TIMER1_CLOCK_EXTERNAL_BY_FALLING_EDGE,
    TIMER1_CLOCK_EXTERNAL_BY_RISING_EDGE
};

enum Timer1Interrupt {
    TIMER1_OVERFLOW_INTERRUPT
    ,TIMER1_COMPARE_A_INTERRUPT
    ,TIMER1_COMPARE_B_INTERRUPT
    ,TIMER1_CAPTURE_INTERRUPT
    ,TIMER1_INTERRUPT_COUNT
};

class Timer1Class
{
private:
    volatile Timer1InterruptHandler_t handlers[TIMER1_INTERRUPT_COUNT];
public:
    Timer1Class();

    /**
     * Set timer clock source bits
     *
     * @param clockSource
     */
    void setClockSource(Timer1ClockSource clockSource);

    /**
     * Set or clear interrupt handler for specific interrupt
     *
     * @param interrupt
     * @param handler_ptr
     */
    void setInterruptHandler(Timer1Interrupt interrupt, void (*handler_ptr) ());

    /**
     * Get timer value
     *
     * @return
     */
    uint16_t getValue();

    /**
     * Set timer value
     *
     * @param value
     */
    void setValue(uint16_t value);

    /**
     * Trigger specific interrupt handler
     *
     * @param interrupt
     */
    void triggerInterrupt(Timer1Interrupt interrupt);
};

extern Timer1Class Timer1;

#endif //TIMER1_H
