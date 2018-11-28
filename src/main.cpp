/**
 * MASTER:
 *
 * Get servo calibration:
 * TWI.write(GET_CALIBRATION_MIN)
 * TWI.write(SERVO_0)
 * TWI.transmit(DRIVER_ADDRESS)
 * val = TWI.read()
 *
 * Set set calibration:
 * TWI.write(SET_CALIBRATION)
 * TWI.transmit(DRIVER_ADDRESS)
 *
 * TWI.write(SET_CALIBRATION_MIN)
 * TWI.write(SERVO_0)
 * TWI.write(value)
 * TWI.transmit(DRIVER_ADDRESS)
 *
 * TWI.write(COMMIT_CALIBRATION)
 * TWI.transmit(DRIVER_ADDRESS)
 *
 * Set servo angle:
 * TWI.write(SET_SERVO_ANGLE)
 * TWI.write(SERVO_0)
 * TWI.write(value)
 * TWI.transmit(DRIVER_ADDRESS)
 *
 * SLAVE:
 * IDLE -> ENABLE_SERVO_COMMAND -> IDLE
 * IDLE -> ENTER_CALIBRATION -> CALIBRATION
 * CALIBRATION -> SAVE_CALIBRATION -> SAVING_CALIBRATION -> IDLE
 * CALIBRATION -> EXIT_CALIBRATION -> IDLE
 */

#include <EEPROM.h>
#include <ES.h>
#include <FSM.h>
#include <LCD.h>
#include <SPI.h>
#include <ServoMotor.h>
#include <PCD8544.h>
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

void toggle_port()
{
    PORTA = ~PORTA;
}

static ServoMotor servos[4];

/**
 * NOP              0 0 0 0 0 0 0 0
 * GET_MIN          0 0 1 0 0 0 i1 i0 Where i index of servo
 * SET_MIN          0 0 1 1 0 0 i1 i0 Where i index of servo
 * GET_MAX          0 1 0 0 0 0 i1 i0 Where i index of servo
 * SET_MAX          0 1 0 1 0 0 i1 i0 Where i index of servo
 * GET_ANGLE        0 1 1 0 0 0 i1 i0 Where i index of servo
 * SET_ANGLE        0 1 1 1 0 0 i1 i0 Where i index of servo
 * GET_MICROSECONDS 1 0 0 0 0 0 i1 i0 Where i index of servo
 * SET_MICROSECONDS 1 0 0 1 0 0 i1 i0 Where i index of servo
 */

//TODO list of 16 commands
#define COMMAND_GET_ENABLED      0x01 // 0b0001XXXX Where X is channel bit-mask
#define COMMAND_SET_ENABLED      0x02 // 0b001EXXXX Where X is channel bit-mask, E boolean value enabled or not

#define COMMAND_GET_MIN          0x01 // 0b01000WII Where I is channel index, W == 0
#define COMMAND_SET_MIN          0x02 // 0b01000WII Where I is channel index, W == 1
#define COMMAND_GET_MAX          0x03 // 0b01000WII Where I is channel index, W == 0
#define COMMAND_SET_MAX          0x04 // 0b01000WII Where I is channel index, W == 1

#define COMMAND_GET_ANGLE        0x05 // 0b01000WII Where I is channel index, W == 0
#define COMMAND_SET_ANGLE        0x06 // 0b01000WII Where I is channel index, W == 1
#define COMMAND_GET_MICROSECONDS 0x07 // 0b01000WII Where I is channel index, W == 0
#define COMMAND_SET_MICROSECONDS 0x08 // 0b01000WII Where I is channel index, W == 1

#define COMMAND_CALIBRATION_ENTER 0x09
#define COMMAND_CALIBRATION_SAVE  0x0A
#define COMMAND_CALIBRATION_EXIT  0x0B

static volatile uint8_t command, index;

void twiOnReceive()
{
    command = TWI.readU08();

    switch (command) {
        case COMMAND_GET_MIN:
            index = TWI.readU08();
            break;
        case COMMAND_SET_MIN:
            servos[TWI.readU08()].setMIN(TWI.readU16());
            break;
        case COMMAND_GET_MAX:
            index = TWI.readU08();
            break;
        case COMMAND_SET_MAX:
            servos[TWI.readU08()].setMAX(TWI.readU16());
            break;
        case COMMAND_GET_ANGLE:
            index = TWI.readU08();
            break;
        case COMMAND_SET_ANGLE:
            servos[TWI.readU08()].setAngle(TWI.readU16());
            break;
        case COMMAND_GET_MICROSECONDS:
            index = TWI.readU08();
            break;
        case COMMAND_SET_MICROSECONDS:
            servos[TWI.readU08()].setMicroseconds(TWI.readU16());
            break;
        case COMMAND_CALIBRATION_ENTER:
            //TODO stop servos
            break;
        case COMMAND_CALIBRATION_SAVE:
            //TODO schedule save calibration
            //TODO start servos
            break;
        case COMMAND_CALIBRATION_EXIT:
            //TODO start servos
            break;
        default:
            asm volatile("nop");
    }
}

void twiOnRequest() {
    switch (command) {
        case COMMAND_GET_MIN:
            TWI.writeU16(index < 4 ? servos[index].getMIN() : 0);
            break;
        case COMMAND_GET_MAX:
            TWI.writeU16(index < 4 ? servos[index].getMAX() : 0);
            break;
        case COMMAND_GET_ANGLE:
            TWI.writeU16(index < 4 ? servos[index].getAngle() : 0);
            break;
        case COMMAND_GET_MICROSECONDS:
            TWI.writeU16(index < 4 ? servos[index].getMicroseconds() : 0);
            break;
        default:
            asm volatile("nop");
    }
}

static uint8_t LCDBuffer[84 * (48 / 8)];

//Byte array of bitmap of 88 x 37 px:
const static uint8_t logo[] PROGMEM = {
        0x00, 0x04, 0x08, 0x10, 0x00, 0x00, 0x10, 0x20, 0x80, 0x00, 0x00, 0x0F, 0xF8, 0x1F, 0xE0, 0x00,
        0x3F, 0xC0, 0xFF, 0xFF, 0x00, 0x0F, 0xFC, 0x0F, 0xE0, 0x00, 0x3F, 0xC1, 0xFF, 0xFF, 0x00, 0x1F,
        0xFC, 0x0F, 0xF0, 0x00, 0x3F, 0x81, 0xFF, 0xFF, 0x00, 0x1F, 0xFE, 0x0F, 0xF0, 0x00, 0x7F, 0x83,
        0xFF, 0xFF, 0x00, 0x1F, 0xFE, 0x07, 0xF8, 0x00, 0x7F, 0x03, 0xF8, 0x3F, 0x00, 0x3F, 0xFF, 0x07,
        0xF8, 0x00, 0xFF, 0x03, 0xF8, 0x1F, 0x00, 0x3F, 0xFF, 0x03, 0xF8, 0x00, 0xFE, 0x07, 0xF0, 0x0F,
        0x00, 0x7F, 0x3F, 0x83, 0xFC, 0x01, 0xFE, 0x07, 0xF0, 0x0F, 0x00, 0x7F, 0x3F, 0x81, 0xFC, 0x01,
        0xFE, 0x0F, 0xF0, 0x0F, 0x00, 0x7E, 0x3F, 0x81, 0xFE, 0x03, 0xFC, 0x0F, 0xE0, 0x0F, 0x00, 0xFE,
        0x1F, 0xC0, 0xFE, 0x03, 0xFC, 0x1F, 0xE0, 0x1F, 0x00, 0xFE, 0x1F, 0xC0, 0xFE, 0x03, 0xF8, 0x1F,
        0xC0, 0x3F, 0x01, 0xFC, 0x1F, 0xE0, 0x7F, 0x07, 0xF8, 0x3F, 0xC1, 0xFF, 0x01, 0xFC, 0x0F, 0xE0,
        0x7F, 0x07, 0xF0, 0x3F, 0xFF, 0xFF, 0x01, 0xFC, 0x0F, 0xF0, 0x7F, 0x8F, 0xF0, 0x3F, 0xFF, 0xFF,
        0x03, 0xF8, 0x07, 0xF0, 0x3F, 0x8F, 0xE0, 0x7F, 0xFF, 0xFC, 0x03, 0xF8, 0x07, 0xF8, 0x3F, 0x8F,
        0xE0, 0x7F, 0xFF, 0xC0, 0x07, 0xF8, 0x07, 0xF8, 0x1F, 0x9F, 0xC0, 0xFF, 0xFF, 0xC0, 0x07, 0xF3,
        0xF3, 0xFC, 0x1F, 0xFF, 0xC0, 0xFF, 0x1F, 0xC0, 0x0F, 0xFF, 0xFF, 0xFC, 0x0F, 0xFF, 0x81, 0xFE,
        0x1F, 0xE0, 0x0F, 0xFF, 0xFF, 0xFC, 0x0F, 0xFF, 0x81, 0xFE, 0x0F, 0xE0, 0x0F, 0xFF, 0xFF, 0xFE,
        0x07, 0xFF, 0x83, 0xFC, 0x0F, 0xE0, 0x1F, 0xFF, 0xFF, 0xFE, 0x07, 0xFF, 0x03, 0xFC, 0x0F, 0xF0,
        0x1F, 0xC0, 0x00, 0xFF, 0x07, 0xFF, 0x03, 0xF8, 0x0F, 0xF0, 0x3F, 0xC0, 0x00, 0xFF, 0x03, 0xFE,
        0x07, 0xF8, 0x07, 0xF0, 0x3F, 0xC0, 0x00, 0x7F, 0x83, 0xFE, 0x07, 0xF8, 0x07, 0xF8, 0x3F, 0x80,
        0x00, 0x7F, 0x81, 0xFC, 0x0F, 0xF0, 0x07, 0xF8, 0x7F, 0x80, 0x00, 0x3F, 0xC1, 0xFC, 0x0F, 0xF0,
        0x07, 0xF8, 0x40, 0x00, 0x00, 0x20, 0x40, 0xF8, 0x10, 0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
        0x00, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x05, 0x2B, 0x17, 0x32, 0xEC, 0x59, 0xF7, 0xD4, 0xF4, 0x79, 0x94, 0x54, 0xBD, 0x8E,
        0xDA, 0xE3, 0x43, 0x06,

};

LCDBitmap_t _logo = {
    .data   = logo,
    .width  = 88,
    .height = 37,
};

int main() {
    SPI.initialize({&DDRB, PIN4, PIN7, PIN5, PIN6});

    PCD8544.initialize({&PORTB, &DDRB, PIN2}, {&PORTB, &DDRB, PIN3}, {&PORTB, &DDRB, PIN4});

    LCD lcd = LCD(LCDBuffer, 84, 48, [](uint8_t byte){ PCD8544.write(PCD8544_DC_DATA, byte); });
    //lcd.clear();
    //lcd.line(0, 0, 50, 20);
    //lcd.circle(15, 15, 15);
    //lcd.rectangle(55, 0, 80, 20);
    //lcd.string("HELLO", 0, 32);
    //lcd.string("HELLO", 32, 34);
    //lcd.bitmap(logo, 0, 0, 88, 37);
    lcd.bitmap(0, 0, _logo);
    lcd.flush();

    //PCD8544.write("HELLO");

    while (true) {}

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
