#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

#define SERVOMOTOR_PULSE_MIN 544
#define SERVOMOTOR_PULSE_MID 1472
#define SERVOMOTOR_PULSE_MAX 2400

class Servo {
private:
    volatile uint8_t *port;
    uint8_t pin;
    uint16_t min;
    uint16_t max;
public:
    /**
     * Bind servo to port pin
     *
     * @param port
     * @param pin
     */
    Servo(volatile uint8_t *port, uint8_t pin);

    /**
     * Bind servo to port pin with calibration
     *
     * @param port
     * @param pin
     * @param min
     * @param max
     */
    Servo(volatile uint8_t *port, uint8_t pin, uint16_t min, uint16_t max);

    /**
     * Enable servo
     */
    void attach();

    /**
     * Disable servo
     */
    void detach();

    /**
     * Get min calibration value
     *
     * @return
     */
    uint16_t getMIN();

    /**
     * Set min calibration value
     *
     * @param value
     */
    void setMIN(uint16_t value);

    /**
     * Get max calibration value
     *
     * @return
     */
    uint16_t getMAX();

    /**
     * Set max calibration value
     *
     * @param value
     */
    void setMAX(uint16_t value);

    /**
     * Get angle value
     *
     * @return
     */
    uint8_t getAngle();

    /**
     * Set angle value
     *
     * @param value
     */
    void setAngle(uint8_t value);

    /**
     * Get microseconds value
     *
     * @return
     */
    uint16_t getMicroseconds();

    /**
     * Set microseconds value
     *
     * @param value
     */
    void setMicroseconds(uint16_t value);
};

#endif //SERVO_H
