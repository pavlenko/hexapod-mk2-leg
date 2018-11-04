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

enum Timer1WaveformGenerationMode {
    TIMER1_WGM_NORMAL,
    TIMER1_WGM_PWM_PHASE_CORRECT_8,
    TIMER1_WGM_PWM_PHASE_CORRECT_9,
    TIMER1_WGM_PWM_PHASE_CORRECT_10,
    TIMER1_WGM_CTC_BY_OCR,
    TIMER1_WGM_FAST_PWM_8,
    TIMER1_WGM_FAST_PWM_9,
    TIMER1_WGM_FAST_PWM_10,
    TIMER1_WGM_PWM_PHASE_FREQ_CORRECT_BY_ICR,
    TIMER1_WGM_PWM_PHASE_FREQ_CORRECT_BY_OCR,
    TIMER1_WGM_PWM_PHASE_CORRECT_BY_ICR,
    TIMER1_WGM_PWM_PHASE_CORRECT_BY_OCR,
    TIMER1_WGM_CTC_BY_ICR,
    TIMER1_WGM_RESERVED,
    TIMER1_WGM_FAST_PWM_BY_ICR,
    TIMER1_WGM_FAST_PWM_BY_OCR,
};

enum Timer1OutputCompareMode {
    TIMER1_COMPARE_OUTPUT_DISABLED,
    TIMER1_COMPARE_OUTPUT_TOGGLE,
    TIMER1_COMPARE_OUTPUT_CLEAR,
    TIMER1_COMPARE_OUTPUT_SET
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
     * Set waveform generation mode (updates both TCCRnA and TCCRnB registers)
     *
     * @param mode
     */
    void setWaveformGenerationMode(Timer1WaveformGenerationMode mode);

    /**
     * Set output compare mode for channel A
     *
     * @param mode
     */
    void setCompareOutputModeA(Timer1OutputCompareMode mode);

    /**
     * Set output compare mode for channel B
     *
     * @param mode
     */
    void setCompareOutputModeB(Timer1OutputCompareMode mode);

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
     * Trigger specific interrupt handler
     *
     * @param interrupt
     */
    void triggerInterrupt(Timer1Interrupt interrupt);
};

extern Timer1Class Timer1;

#endif //TIMER1_H
