#ifndef TIMER_H
#define TIMER_H

//#define __AVR_ATmega2560__ 1 // For test only
//#define __AVR_ATmega128A__ 1

#include <avr/io.h>
#include <avr/sfr_defs.h>

// ATMEGA 48,88,168: TCCR0A, TCCR0B, TCNT0, OCR0A, OCR0B, TIMSK0, TIFR0
// ATMEGA 8L: TCCR0, TCNT0, TIMSK, TIFR <-- not supported
// ATMEGA 16a: TCCR0, TCNT0, OCR0, TIMSK, TIFR
// ATMEGA 32L: TCCR0, TCNT0, OCR0, TIMSK, TIFR
// ATMEGA 64L: TCCR0, TCNT0, OCR0, TIMSK, TIFR
// ATMEGA 128L: TCCR0, TCNT0, OCR0, TIMSK, TIFR
// ATMEGA 2560: TCCR0A/B, TCNT0, OCR0A/B, TIMSK0, TIFR0

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

#ifdef TCNT1

enum Timer1Interrupt {
    TIMER1_ISR_OVERFLOW = TOIE1
    ,TIMER1_ISR_OUTPUT_COMPARE_A = OCIE1A
    ,TIMER1_ISR_OUTPUT_COMPARE_B = OCIE1B
#ifdef OCIE1C
    ,TIMER1_ISR_OUTPUT_COMPARE_C = OCIE1C
#endif
#ifdef ICIE1
    ,TIMER1_ISR_INPUT_CAPTURE = ICIE1
#else
    ,TIMER1_ISR_INPUT_CAPTURE = TICIE1
#endif
};

class Timer1Class {
private:
    volatile TimerInterruptHandler_t handlers[8];
public:
    /**
     * @param clockSource
     */
    void setClockSource(TimerClockSource clockSource) {
        TCCR1B = (uint8_t) ((TCCR1B & 0b11111000) | clockSource);
    }

    /**
     * @param code
     * @param handlerPtr
     */
    void setInterruptHandler(Timer1Interrupt code, TimerInterruptHandler_t handlerPtr) {
        this->handlers[code] = handlerPtr;

        if (handlerPtr) {
#ifdef TIMSK3
            TIMSK3 |= _BV(code);
#else
            TIMSK |= _BV(code);
#endif
        } else {
#ifdef TIMSK3
            TIMSK3 &= ~_BV(code);
#else
            TIMSK &= ~_BV(code);
#endif
        }
    }

    /**
     * @param code
     */
    void triggerInterrupt(Timer1Interrupt code) {
        if (this->handlers[code]) {
            this->handlers[code]();
        }
    }
};

extern Timer1Class Timer1;

#endif //TCNT1

#ifdef TCNT2

enum Timer2Interrupt {
    TIMER2_ISR_OVERFLOW = TOIE2
#ifdef OCIE2A
    ,TIMER2_ISR_OUTPUT_COMPARE_A = OCIE2A
    ,TIMER2_ISR_OUTPUT_COMPARE_B = OCIE2B
#else
    ,TIMER2_ISR_OUTPUT_COMPARE_A = OCIE2
#endif
};

class Timer2Class {
private:
    volatile TimerInterruptHandler_t handlers[8];
public:
    /**
     * @param clockSource
     */
    void setClockSource(TimerClockSource clockSource) {
#ifdef TCCR2B
        TCCR2B = (uint8_t) ((TCCR2B & 0b11111000) | clockSource);
#else
        TCCR2 = (uint8_t) ((TCCR2 & 0b11111000) | clockSource);
#endif
    }

    /**
     * @param code
     * @param handlerPtr
     */
    void setInterruptHandler(Timer2Interrupt code, TimerInterruptHandler_t handlerPtr) {
        this->handlers[code] = handlerPtr;

        if (handlerPtr) {
#ifdef TIMSK2
            TIMSK2 |= _BV(code);
#else
            TIMSK |= _BV(code);
#endif
        } else {
#ifdef TIMSK2
            TIMSK2 &= ~_BV(code);
#else
            TIMSK &= ~_BV(code);
#endif
        }
    }

    /**
     * @param code
     */
    void triggerInterrupt(Timer2Interrupt code) {
        if (this->handlers[code]) {
            this->handlers[code]();
        }
    }
};

#endif //TCNT2

#ifdef TCNT3

enum Timer3Interrupt {
    TIMER3_ISR_OVERFLOW = TOIE3,
    TIMER3_ISR_OUTPUT_COMPARE_A = OCIE3A,
    TIMER3_ISR_OUTPUT_COMPARE_B = OCIE3B,
    TIMER3_ISR_OUTPUT_COMPARE_C = OCIE3C,
#ifdef ICIE3
    TIMER3_ISR_INPUT_CAPTURE = ICIE3
#else
    TIMER3_ISR_INPUT_CAPTURE = TICIE3
#endif
};

class Timer3Class {
private:
    volatile TimerInterruptHandler_t handlers[8];
public:
    /**
     * @param clockSource
     */
    void setClockSource(TimerClockSource clockSource) {
        TCCR3B = (uint8_t) ((TCCR3B & 0b11111000) | clockSource);
    }

    uint16_t getCounterValue() {
        return TCNT3;
    }

    void setCounterValue(uint16_t value) {
        TCNT3 = value;
    }

    uint16_t getCompareValueA() {
        return OCR3A;
    }

    void setCompareValueA(uint16_t value) {
        OCR3A = value;
    }

    uint16_t getCompareValueB() {
        return OCR3B;
    }

    void setCompareValueB(uint16_t value) {
        OCR3B = value;
    }

    uint16_t getCompareValueC() {
        return OCR3C;
    }

    void setCompareValueC(uint16_t value) {
        OCR3C = value;
    }

    uint16_t getCaptureValue() {
        return ICR3;
    }

    void setCaptureValue(uint16_t value) {
        ICR3 = value;
    }

    /**
     * @param code
     * @param handlerPtr
     */
    void setInterruptHandler(Timer3Interrupt code, TimerInterruptHandler_t handlerPtr) {
        this->handlers[code] = handlerPtr;

        if (handlerPtr) {
#ifdef TIMSK3
            TIMSK3 |= _BV(code);
#else
            ETIMSK |= _BV(code);
#endif
        } else {
#ifdef TIMSK3
            TIMSK3 &= ~_BV(code);
#else
            ETIMSK &= ~_BV(code);
#endif
        }
    }

    /**
     * @param code
     */
    void triggerInterrupt(Timer3Interrupt code) {
        if (this->handlers[code]) {
            this->handlers[code]();
        }
    }
};

extern Timer3Class Timer3;

#endif //TCNT3

#ifdef TCNT4

enum Timer4Interrupt {
    TIMER4_ISR_OVERFLOW = TOIE4,
    TIMER4_ISR_OUTPUT_COMPARE_A = OCIE4A,
    TIMER4_ISR_OUTPUT_COMPARE_B = OCIE4B,
    TIMER4_ISR_OUTPUT_COMPARE_C = OCIE4C,
    TIMER4_ISR_INPUT_CAPTURE = ICIE4
};

class Timer4Class {
private:
    volatile TimerInterruptHandler_t handlers[8];
public:
    /**
     * @param clockSource
     */
    void setClockSource(TimerClockSource clockSource) {
        TCCR4B = (uint8_t) ((TCCR4B & 0b11111000) | clockSource);
    }

    uint16_t getCounterValue() {
        return TCNT4;
    }

    void setCounterValue(uint16_t value) {
        TCNT4 = value;
    }

    uint16_t getCompareValueA() {
        return OCR4A;
    }

    void setCompareValueA(uint16_t value) {
        OCR4A = value;
    }

    uint16_t getCompareValueB() {
        return OCR4B;
    }

    void setCompareValueB(uint16_t value) {
        OCR4B = value;
    }

    uint16_t getCompareValueC() {
        return OCR4C;
    }

    void setCompareValueC(uint16_t value) {
        OCR4C = value;
    }

    uint16_t getCaptureValue() {
        return ICR4;
    }

    void setCaptureValue(uint16_t value) {
        ICR4 = value;
    }

    /**
     * @param code
     * @param handlerPtr
     */
    void setInterruptHandler(Timer4Interrupt code, TimerInterruptHandler_t handlerPtr) {
        this->handlers[code] = handlerPtr;

        if (handlerPtr) {
            TIMSK4 |= _BV(code);
        } else {
            TIMSK4 &= ~_BV(code);
        }
    }

    /**
     * @param code
     */
    void triggerInterrupt(Timer4Interrupt code) {
        if (this->handlers[code]) {
            this->handlers[code]();
        }
    }
};

extern Timer4Class Timer4;

#endif //TCNT4

#ifdef TCNT5

enum Timer5Interrupt {
    TIMER5_ISR_OVERFLOW = TOIE5,
    TIMER5_ISR_OUTPUT_COMPARE_A = OCIE5A,
    TIMER5_ISR_OUTPUT_COMPARE_B = OCIE5B,
    TIMER5_ISR_OUTPUT_COMPARE_C = OCIE5C,
    TIMER5_ISR_INPUT_CAPTURE = ICIE5
};

class Timer5Class {
private:
    volatile TimerInterruptHandler_t handlers[8];
public:
    /**
     * @param clockSource
     */
    void setClockSource(TimerClockSource clockSource) {
        TCCR5B = (uint8_t) ((TCCR5B & 0b11111000) | clockSource);
    }

    uint16_t getCounterValue() {
        return TCNT5;
    }

    void setCounterValue(uint16_t value) {
        TCNT5 = value;
    }

    uint16_t getCompareValueA() {
        return OCR5A;
    }

    void setCompareValueA(uint16_t value) {
        OCR5A = value;
    }

    uint16_t getCompareValueB() {
        return OCR5B;
    }

    void setCompareValueB(uint16_t value) {
        OCR5B = value;
    }

    uint16_t getCompareValueC() {
        return OCR5C;
    }

    void setCompareValueC(uint16_t value) {
        OCR5C = value;
    }

    uint16_t getCaptureValue() {
        return ICR5;
    }

    void setCaptureValue(uint16_t value) {
        ICR5 = value;
    }

    /**
     * @param code
     * @param handlerPtr
     */
    void setInterruptHandler(Timer5Interrupt code, TimerInterruptHandler_t handlerPtr) {
        this->handlers[code] = handlerPtr;

        if (handlerPtr) {
            TIMSK5 |= _BV(code);
        } else {
            TIMSK5 &= ~_BV(code);
        }
    }

    /**
     * @param code
     */
    void triggerInterrupt(Timer5Interrupt code) {
        if (this->handlers[code]) {
            this->handlers[code]();
        }
    }
};

extern Timer5Class Timer5;

#endif //TCNT5

#endif //TIMER_H
