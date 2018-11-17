#include <Servo.h>

#include <avr/io.h>

int main() {
    // Initialize servo for port and pin
    Servo servo1 = Servo(&PORTA, PB0);

    // Initialize servo for port and pin with calibration
    Servo servo2 = Servo(&PORTA, PB0, 600, 2300);

    // You can get calibration values
    uint16_t min = servo1.getMin();
    uint16_t max = servo1.getMax();

    // You can set calibration values after initialization
    servo1.setMin(SERVOMOTOR_PULSE_MIN);
    servo1.setMax(SERVOMOTOR_PULSE_MAX);

    // For change angle you can set it as degree value
    servo1.setAngle(60);

    // Or you can set it as pulse width
    servo1.setMicroseconds(1000);

    // You can read latest angle value as degree
    uint8_t degree = servo1.getAngle();

    // Or read as microseconds
    uint16_t ms = servo1.getMicroseconds();
}