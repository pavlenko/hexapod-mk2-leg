#define SERVOMOTOR_USE_TIMER1 1
#define SERVOMOTOR_USE_TIMER3 1
#define SERVOMOTOR_USE_TIMER4 1
#define SERVOMOTOR_USE_TIMER5 1

#include <EEPROM.h>
#include <ES.h>
#include <FSM.h>
#include <ServoMotor.h>
#include <Timer0.h>
#include <TWI.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdint.h>
#include <util/delay.h>

#define EEPROM_SERVO1_MIN 0x00
#define EEPROM_SERVO1_MAX 0x01

#define EEPROM_SERVO2_MIN 0x02
#define EEPROM_SERVO2_MAX 0x03

#define EEPROM_SERVO3_MIN 0x04
#define EEPROM_SERVO3_MAX 0x05

#define EEPROM_SERVO4_MIN 0x06
#define EEPROM_SERVO4_MAX 0x07

#define MODE_IDLE      0
#define MODE_ACTIVE    1
#define MODE_CALIBRATE 2

static volatile uint8_t mode;

#define COMMAND_NONE          0x00
#define COMMAND_SERVO_ON      0x10
//TODO servo related commands
#define COMMAND_SERVO_OFF     0x1F
#define COMMAND_CALIBRATE_ON  0x20
//TODO calibrate related command
#define COMMAND_CALIBRATE_OFF 0x2F

FSMState STATE_IDLE = FSMState();

volatile uint8_t timer;

void onTimer5CompareA() {
    //TODO handle servo update
}

void toggle_port()
{
    PORTA = ~PORTA;
}

void twiOnReceive()
{
    uint8_t command;

    TWI.read(&command);

    switch (command) {
        case COMMAND_SERVO_ON:
            mode = MODE_ACTIVE;
            break;
        case COMMAND_CALIBRATE_ON:
            mode = MODE_CALIBRATE;
            break;
        case COMMAND_SERVO_OFF:
        case COMMAND_CALIBRATE_OFF:
            mode = MODE_IDLE;
            break;
    }
}

void twiOnRequest() {
    //TODO handle commands from TWI module
}

static ServoMotor servos[4];

int main() {
    // Initialize port for address configuration (set as input & enable internal pull-up resistors)
    DDRC  = 0x00;
    PORTC = 0xFF;

    // Initialize TWI
    TWI.setAddress((uint8_t) (0xF0 | ~(PINC & 0x0F)));// <-- read low 4 bits, use connected to ground as 1
    TWI.setOnReceiveHandler(twiOnReceive);
    TWI.setOnRequestHandler(twiOnRequest);

    // Read calibration from EEPROM & initialize servo motors
    uint16_t min, max;

    EEPROM.read(EEPROM_SERVO1_MIN, &min);
    EEPROM.read(EEPROM_SERVO1_MAX, &max);

    servos[0].attach(&PORTB, PB0, min, max);

    EEPROM.read(EEPROM_SERVO2_MIN, &min);
    EEPROM.read(EEPROM_SERVO2_MAX, &max);

    servos[1].attach(&PORTB, PB1, min, max);

    EEPROM.read(EEPROM_SERVO3_MIN, &min);
    EEPROM.read(EEPROM_SERVO3_MAX, &max);

    servos[2].attach(&PORTB, PB2, min, max);

    EEPROM.read(EEPROM_SERVO4_MIN, &min);
    EEPROM.read(EEPROM_SERVO4_MAX, &max);

    servos[3].attach(&PORTB, PB3, min, max);

    // Initialize finite state machine
    FSM.initialize(STATE_IDLE);

    // Global enable interrupts
    sei();

    while (true) {
        ES.dispatch();
    }
}
