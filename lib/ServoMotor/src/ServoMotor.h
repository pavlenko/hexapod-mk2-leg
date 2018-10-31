#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

#include <stdint.h>

#define SERVOMOTOR_INVALID 255

//TODO min/max calibration
class ServoMotor
{
private:
    uint8_t index;
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
     * Detach servomotor
     */
    void detach();

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

    /**
     * Get angle as degree
     *
     * @return
     */
    uint16_t getDegree();

    /**
     * Set angle as degree
     *
     * @param angle
     */
    void setDegree(uint16_t value);
};

#endif //SERVOMOTOR_H
