#include <ServoMotor.h>

#include <stdint.h>

int main() {
    // Attach servo to specific port and pin
    uint8_t index1 = ServoMotor.attach(&PORTA, PINA1);

    // Attach servo to specific port and pin with pass calibration limits
    uint8_t index2 = ServoMotor.attach(&PORTA, PINA1, 600, 2000);
}

