#include "ServoMotor.h"

// Dependency on Timer lib
#include <Timer16Bit.h>

#include <util/atomic.h>
#include <stdint.h>

//TODO check dependency of timers prescaller
#define TICKS_PER_US() (F_CPU / 1000000L)
#define US_TO_TICKS(_val_) ((uint16_t) (_val_ * TICKS_PER_US()))
#define TICKS_TO_US(_val_) ((uint16_t) (_val_ / TICKS_PER_US()))
#define REFRESH_US 20000U

typedef struct {
    uint8_t number: 3;
    uint8_t attached: 1;
} pin_t;

typedef struct {
    volatile uint8_t *port;
    pin_t pin;
    uint16_t ticks;
    uint16_t min;
    uint16_t max;
} servo_t;

static volatile uint8_t count = 0;
static volatile servo_t servos[SERVOMOTOR_TOTAL];
static volatile int8_t channels[SERVOMOTOR_TIMER_COUNT];

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//TODO add internal enable/disable timer interrupt/handler
//TODO -- maybe use map of timer obj address -> channel index, and pass timer instance to handler
static inline void onTimerCompareA(ServomotorTimer timerN, Timer16BitClass *timer) {
    uint8_t index;

    if (channels[timerN] < 0) {
        *timer->TCNTn = 0;
    } else {
        index = (timerN * SERVOMOTOR_PER_TIMER) + channels[timerN];

        if (index < count && servos[index].pin.attached) {
            *(servos[index].port) &= ~_BV(servos[index].pin.number);
        }
    }

    channels[timerN]++;

    index = (timerN * SERVOMOTOR_PER_TIMER) + channels[timerN];

    if (index < count && channels[timerN] < SERVOMOTOR_PER_TIMER) {
        *timer->OCRnA = *timer->TCNTn + servos[index].ticks;

        if (servos[index].pin.attached) {
            *(servos[index].port) |= _BV(servos[index].pin.number);
        }
    } else {
        if (*timer->TCNTn + 4 < US_TO_TICKS(REFRESH_US)) {
            *timer->OCRnA = (uint16_t) US_TO_TICKS(REFRESH_US);
        } else {
            *timer->OCRnA = *timer->TCNTn + 4;
        }

        channels[timerN] = -1;
    }
}

#ifdef TCNT1
#include <Timer1.h>
static inline void onTimer1CompareA() { onTimerCompareA(SERVOMOTOR_TIMER1, &Timer1); }
#endif

#ifdef TCNT3
#include <Timer3.h>
static inline void onTimer3CompareA() { onTimerCompareA(SERVOMOTOR_TIMER3, &Timer3); }
#endif

#ifdef TCNT4
#include <Timer4.h>
static inline void onTimer4CompareA() { onTimerCompareA(SERVOMOTOR_TIMER4, &Timer4); }
#endif

#ifdef TCNT5
#include <Timer5.h>
static inline void onTimer5CompareA() { onTimerCompareA(SERVOMOTOR_TIMER5, &Timer5); }
#endif

ServoMotorClass::ServoMotorClass() {
#ifdef TCNT1
    Timer1.setCountMode(TIMER_16BIT_COUNT_NORMAL);
    Timer1.setClockSource(TIMER_CLOCK_DIVIDE_BY_8);
    Timer1.setInterruptHandler(TIMER_INTERRUPT_COMPARE_MATCH_A, onTimer1CompareA);
#endif
#ifdef TCNT3
    Timer3.setCountMode(TIMER_16BIT_COUNT_NORMAL);
    Timer3.setClockSource(TIMER_CLOCK_DIVIDE_BY_8);
    Timer3.setInterruptHandler(TIMER_INTERRUPT_COMPARE_MATCH_A, onTimer3CompareA);
#endif
#ifdef TCNT4
    Timer4.setCountMode(TIMER_16BIT_COUNT_NORMAL);
    Timer4.setClockSource(TIMER_CLOCK_DIVIDE_BY_8);
    Timer4.setInterruptHandler(TIMER_INTERRUPT_COMPARE_MATCH_A, onTimer4CompareA);
#endif
#ifdef TCNT5
    Timer5.setCountMode(TIMER_16BIT_COUNT_NORMAL);
    Timer5.setClockSource(TIMER_CLOCK_DIVIDE_BY_8);
    Timer5.setInterruptHandler(TIMER_INTERRUPT_COMPARE_MATCH_A, onTimer5CompareA);
#endif
}

uint8_t ServoMotorClass::attach(volatile uint8_t *port, uint8_t pin) {
    return this->attach(port, pin, SERVOMOTOR_PULSE_MIN, SERVOMOTOR_PULSE_MAX);
}

uint8_t ServoMotorClass::attach(volatile uint8_t *port, uint8_t pin, uint16_t min, uint16_t max) {
    if (count < SERVOMOTOR_TOTAL) {
        servos[count].min = min;
        servos[count].max = max;

        servos[count].port = port;

        servos[count].pin.number   = pin;
        servos[count].pin.attached = 1;

        return count++;
    }

    return SERVOMOTOR_INVALID;
}

void ServoMotorClass::detach(uint8_t index) {
    if (index < count) {
        servos[index].pin.attached = 0;
    }
}

uint16_t ServoMotorClass::getMIN(uint8_t index) {
    if (index < count) {
        return servos[index].min;
    }

    return 0;
}

void ServoMotorClass::setMIN(uint8_t index, uint16_t value) {
    if (index < count) {
        servos[index].min = value;
    }
}

uint16_t ServoMotorClass::getMAX(uint8_t index) {
    if (index < count) {
        return servos[index].max;
    }

    return 0;
}

void ServoMotorClass::setMAX(uint8_t index, uint16_t value) {
    if (index < count) {
        servos[index].max = value;
    }
}

uint16_t ServoMotorClass::getAngle(uint8_t index) {
    if (index < count) {
        return (uint16_t) map(this->getMicroseconds(index), servos[index].min, servos[index].max, 0, 180);
    }

    return 0;
}

void ServoMotorClass::setAngle(uint8_t index, uint16_t value) {
    if (index < count) {
        this->setMicroseconds(index, (uint16_t) map(value, 0, 180, servos[index].min, servos[index].max));
    }
}

uint16_t ServoMotorClass::getMicroseconds(uint8_t index) {
    if (index < count) {
        return TICKS_TO_US(servos[index].ticks);
    } else {
        return 0;
    }
}

void ServoMotorClass::setMicroseconds(uint8_t index, uint16_t value) {
    if (index < count) {
        uint16_t ticks = US_TO_TICKS(value);

        ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
            servos[index].ticks = ticks;
        }
    }
}

ServoMotorClass ServoMotor;
