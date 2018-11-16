#define SERVOMOTOR_USE_TIMER1 1
#define SERVOMOTOR_USE_TIMER3 1
#define SERVOMOTOR_USE_TIMER4 1
#define SERVOMOTOR_USE_TIMER5 1

#include <EEPROM.h>
#include <ES.h>
#include <FSM.h>
#include <ServoMotor.h>
#include <Timer.h>
#include <TWI.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdint.h>
#include <util/delay.h>

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

int main() {
    // Initialize TWI
    TWI.setAddress(0x01);
    TWI.setOnReceiveHandler(twiOnReceive);
    TWI.setOnRequestHandler(twiOnRequest);

    // Initialize finite state machine
    FSM.initialize();

    //TODO Read calibration values from EEPROM
    //TODO Read last values from EEPROM
    //TODO Initialize servos

    uint8_t value;
    EEPROM.read(0, &value);

    DDRA  |= _BV(PA0);
    PORTA &= ~_BV(PA0);

    PORTB = value;

    *Timer0.TCNTn = 5;

    /*Timer1.setClockSource(TIMER_CLOCK_DIVIDE_BY_8);
    Timer1.setInterruptHandler(TIMER1_ISR_OUTPUT_COMPARE_A, [](){
        ServoMotor.update(SERVOMOTOR_TIMER1, &TCNT1, &OCR1A);
    });*/

    sei();

    ES.attach(1, toggle_port);

    while(true)
    {
        ES.dispatch();
        //PORTA |= _BV(PA0);
        //_delay_ms(1000);
        //PORTA &= ~_BV(PA0);
        //_delay_ms(1000);
    }
}
