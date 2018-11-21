#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <avr/io.h>
#include <stdint.h>

#define SERVOMOTOR_PULSE_MIN 544
#define SERVOMOTOR_PULSE_MID 1472
#define SERVOMOTOR_PULSE_MAX 2400

#define SERVOMOTOR_INVALID 255

#ifndef SERVOMOTOR_PER_TIMER
#define SERVOMOTOR_PER_TIMER 10
#endif

#ifndef SERVOMOTOR_USE_TIMER1
#define SERVOMOTOR_USE_TIMER1 1
#endif

#ifndef SERVOMOTOR_USE_TIMER3
#define SERVOMOTOR_USE_TIMER3 1
#endif

#ifndef SERVOMOTOR_USE_TIMER4
#define SERVOMOTOR_USE_TIMER4 1
#endif

#ifndef SERVOMOTOR_USE_TIMER5
#define SERVOMOTOR_USE_TIMER5 1
#endif

enum ServomotorTimer {
#if defined(TCNT1) && SERVOMOTOR_USE_TIMER1
    SERVOMOTOR_TIMER1,
#endif
#if defined(TCNT3) && SERVOMOTOR_USE_TIMER1
    SERVOMOTOR_TIMER3,
#endif
#if defined(TCNT4) && SERVOMOTOR_USE_TIMER1
    SERVOMOTOR_TIMER4,
#endif
#if defined(TCNT5) && SERVOMOTOR_USE_TIMER1
    SERVOMOTOR_TIMER5,
#endif
    SERVOMOTOR_TIMER_COUNT
};

#define SERVOMOTOR_TOTAL (SERVOMOTOR_PER_TIMER * SERVOMOTOR_TIMER_COUNT)

class ServoMotorClass
{
public:
    ServoMotorClass();

    /**
     * Attach servomotor to PIN of specific PORT
     *
     * @param port
     * @param pin
     * @return
     */
    uint8_t attach(volatile uint8_t *port, uint8_t pin);

    /**
     * Attach servomotor to PIN of specific PORT and define limits
     *
     * @param port
     * @param pin
     * @param min
     * @param max
     * @return
     */
    uint8_t attach(volatile uint8_t *port, uint8_t pin, uint16_t min, uint16_t max);

    /**
     * Detach servomotor
     *
     * @param index
     */
    void detach(uint8_t index);

    /**
     * Get calibration minimal ticks
     *
     * @param index
     * @return
     */
    uint16_t getMIN(uint8_t index);

    /**
     * Set calibration minimal ticks
     *
     * @param index
     * @param value
     */
    void setMIN(uint8_t index, uint16_t value);

    /**
     * Get calibration maximum ticks
     *
     * @param index
     * @return
     */
    uint16_t getMAX(uint8_t index);

    /**
     * Set calibration maximum ticks
     *
     * @param index
     * @param value
     */
    void setMAX(uint8_t index, uint16_t value);

    /**
     * Get angle as degree
     *
     * @param index
     * @return
     */
    uint16_t getAngle(uint8_t index);

    /**
     * Set angle as degree
     *
     * @param index
     * @param angle
     */
    void setAngle(uint8_t index, uint16_t value);

    /**
     * Get angle as microseconds
     *
     * @param index
     * @return
     */
    uint16_t getMicroseconds(uint8_t index);

    /**
     * Set angle as microseconds
     *
     * @param index
     * @param value
     */
    void setMicroseconds(uint8_t index, uint16_t value);
};

extern ServoMotorClass ServoMotor;

#endif //SERVOMOTOR_H
