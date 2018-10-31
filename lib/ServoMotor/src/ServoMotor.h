#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <stdint.h>

#define SERVOMOTOR_PULSE_MIN 544
#define SERVOMOTOR_PULSE_MID 1472
#define SERVOMOTOR_PULSE_MAX 2400

#define SERVOMOTOR_INVALID 255

class ServoMotor
{
private:
    uint8_t index;
    uint16_t min;
    uint16_t max;
public:
    ServoMotor();

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
     */
    void detach();

    /**
     * Get calibration minimal ticks
     *
     * @return
     */
    uint16_t getMIN();

    /**
     * Set calibration minimal ticks
     *
     * @return
     */
    void setMIN(uint16_t value);

    /**
     * Get calibration maximum ticks
     *
     * @return
     */
    uint16_t getMAX();

    /**
     * Set calibration maximum ticks
     *
     * @return
     */
    void setMAX(uint16_t value);

    /**
     * Get angle as degree
     *
     * @return
     */
    uint16_t getAngle();

    /**
     * Set angle as degree
     *
     * @param angle
     */
    void setAngle(uint16_t value);

    /**
     * Get angle as microseconds
     *
     * @return
     */
    uint16_t getMicroseconds();

    /**
     * Set angle as microseconds
     *
     * @param value
     */
    void setMicroseconds(uint16_t value);
};

#endif //SERVOMOTOR_H
