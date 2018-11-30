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

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdint.h>
#include <util/delay.h>

#define SERVO_COUNT 4

uint16_t EEPROM_SERVO0_MIN EEMEM = 0;
uint16_t EEPROM_SERVO0_MAX EEMEM = 0;

uint16_t EEPROM_SERVO1_MIN EEMEM = 0;
uint16_t EEPROM_SERVO1_MAX EEMEM = 0;

uint16_t EEPROM_SERVO2_MIN EEMEM = 0;
uint16_t EEPROM_SERVO2_MAX EEMEM = 0;

uint16_t EEPROM_SERVO3_MIN EEMEM = 0;
uint16_t EEPROM_SERVO3_MAX EEMEM = 0;

FSMState STATE_IDLE = FSMState();

static ServoMotor servos[SERVO_COUNT];

#define COMMAND_NOP                 0b00000000 // 0b00000000 No operation
#define COMMAND_CONTROL             0b00010000 // 0b0001000E Where E is boolean enable/disable control angle get/set
#define COMMAND_CALIBRATION_MIN_GET 0b00100000 // 0b00100RXX Where X is channel index, R == 0
#define COMMAND_CALIBRATION_MIN_SET 0b00100100 // 0b00100WXX Where X is channel index, W == 1
#define COMMAND_CALIBRATION_MAX_GET 0b00101000 // 0b00101RXX Where X is channel index, R == 0
#define COMMAND_CALIBRATION_MAX_SET 0b00101100 // 0b00101WXX Where X is channel index, W == 1
#define COMMAND_CALIBRATION_SAVE    0b00100000 // 0b00100000 Store calibration values into EEPROM
#define COMMAND_ANGLE_GET           0b00110000 // 0b00110RXX Where X is channel index, R == 0
#define COMMAND_ANGLE_SET           0b00110100 // 0b00110WXX Where X is channel index, W == 1
#define COMMAND_MICROSECONDS_GET    0b00111000 // 0b00111RXX Where X is channel index, R == 0
#define COMMAND_MICROSECONDS_SET    0b00111100 // 0b00111WXX Where X is channel index, W == 1

typedef struct {
    uint8_t command;
    union {
        uint32_t argumentU32;
        uint16_t argumentU16A;
        uint16_t argumentU16B;
        uint8_t argumentU08A;
        uint8_t argumentU08B;
        uint8_t argumentU08C;
        uint8_t argumentU08D;
    };
} Command_t;

static Command_t commandsBufferData[SERVO_COUNT];
static volatile uint8_t commandsBufferIndex  = 0;
static volatile uint8_t commandsBufferLength = 0;

static volatile uint8_t command = COMMAND_NOP;
static volatile bool control = false;

void twiOnReceive()
{
    while (TWI_ERROR_READ != TWI.getError()) {
        //TODO read commands to command buffer
        //TODO command can have 2 arguments with total 32 bit size
    }

    command = TWI.readU08();

    if (command & COMMAND_CONTROL) {
        control = (bool) (command & 0x01);
    }
    if (command & COMMAND_CALIBRATION_MIN_SET) {
        servos[(command & 0x3)].setMIN(TWI.readU16());
    }
    if (command & COMMAND_CALIBRATION_MAX_SET) {
        servos[(command & 0x3)].setMAX(TWI.readU16());
    }
    if (command & COMMAND_CALIBRATION_SAVE) {
        EEPROM.start();

        EEPROM.writeU16(EEPROM_SERVO0_MIN, servos[0].getMIN());
        EEPROM.writeU16(EEPROM_SERVO0_MAX, servos[0].getMAX());

        EEPROM.writeU16(EEPROM_SERVO1_MIN, servos[1].getMIN());
        EEPROM.writeU16(EEPROM_SERVO1_MAX, servos[1].getMAX());

        EEPROM.writeU16(EEPROM_SERVO2_MIN, servos[2].getMIN());
        EEPROM.writeU16(EEPROM_SERVO2_MAX, servos[2].getMAX());

        EEPROM.writeU16(EEPROM_SERVO3_MIN, servos[3].getMIN());
        EEPROM.writeU16(EEPROM_SERVO3_MAX, servos[3].getMAX());

        EEPROM.flush();
    }
    if (control && command & COMMAND_ANGLE_SET) {
        servos[(command & 0x3)].setAngle(TWI.readU16());
    }
    if (control && command & COMMAND_MICROSECONDS_SET) {
        servos[(command & 0x3)].setMicroseconds(TWI.readU16());
    }
}

void twiOnRequest() {
    if (command & COMMAND_CALIBRATION_MIN_GET) {
        TWI.writeU16(servos[(command & 0x3)].getMIN());
    }
    if (command & COMMAND_CALIBRATION_MAX_GET) {
        TWI.writeU16(servos[(command & 0x3)].getMAX());
    }
    if (command & COMMAND_ANGLE_GET) {
        TWI.writeU16(servos[(command & 0x3)].getAngle());
    }
    if (command & COMMAND_MICROSECONDS_GET) {
        TWI.writeU16(servos[(command & 0x3)].getMicroseconds());
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
        0x00, 0x00, 0x0C, 0xFB, 0x26, 0x4A, 0xBB, 0x4C, 0x91, 0x93, 0x96, 0x15, 0x43, 0x9B, 0xD2, 0xBE,
        0x37, 0x01, 0xD8, 0xDE,
};

LCDBitmap_t _logo = {
    .data   = logo,
    .width  = 84,
    .height = 37,
    .horizontal = true,
    .msb = false
};
// Represents char "E"
const static uint8_t test_bitmapH_8x5_data[] PROGMEM = {
        0xFF,
        0x80,
        0xFF,
        0x80,
        0xFF,
};

LCDBitmap_t test_bitmapH_8x5 = {.data = test_bitmapH_8x5_data, .width = 8, .height = 5, .horizontal = true, .msb = true};

// Represents char "M"
const static uint8_t test_bitmapV_5x8_data[] PROGMEM = {
        0xFF, 0x80, 0xFF, 0x080, 0xFF,
};

LCDBitmap_t test_bitmapV_5x8 = {.data = test_bitmapV_5x8_data, .width = 5, .height = 8, .horizontal = false, .msb = true};

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
    servos[0].attach(&PORTB, PB0, EEPROM.readU16(EEPROM_SERVO0_MIN), EEPROM.readU16(EEPROM_SERVO0_MAX));
    servos[1].attach(&PORTB, PB1, EEPROM.readU16(EEPROM_SERVO1_MIN), EEPROM.readU16(EEPROM_SERVO1_MAX));
    servos[2].attach(&PORTB, PB2, EEPROM.readU16(EEPROM_SERVO2_MIN), EEPROM.readU16(EEPROM_SERVO2_MAX));
    servos[3].attach(&PORTB, PB3, EEPROM.readU16(EEPROM_SERVO3_MIN), EEPROM.readU16(EEPROM_SERVO3_MAX));

    // Initialize finite state machine
    FSM.initialize(STATE_IDLE);

    // Global enable interrupts
    sei();

    while (true) {
        ES.dispatch();
    }
}
