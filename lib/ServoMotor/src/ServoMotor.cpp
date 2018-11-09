#include "ServoMotor.h"

#include <util/atomic.h>
#include <stdint.h>

#define SERVOMOTOR_TOTAL 10 //TODO

//TODO check dependency of timers prescaller
#define TICKS_PER_US() (F_CPU / 1000000L)
#define US_TO_TICKS(_val_) ((uint16_t) (_val_ * TICKS_PER_US()))
#define TICKS_TO_US(_val_) ((uint16_t) (_val_ / TICKS_PER_US()))

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

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t ServoMotorClass::attach(volatile uint8_t *port, uint8_t pin) {
    return this->attach(port, pin, SERVOMOTOR_PULSE_MIN, SERVOMOTOR_PULSE_MAX);
}

uint8_t ServoMotorClass::attach(volatile uint8_t *port, uint8_t pin, uint16_t min, uint16_t max) {
    if (count < SERVOMOTOR_TOTAL) {
        servos[count].port = port;

        servos[count].pin.number   = pin;
        servos[count].pin.attached = 1;

        servos[count].min = min;
        servos[count].max = max;

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

void ServoMotorClass::update(volatile uint16_t *TCNTn, volatile uint16_t *OCRnA) {
    //TODO ISR handler
}

ServoMotorClass ServoMotor;
