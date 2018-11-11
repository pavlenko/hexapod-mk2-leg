#define SERVOMOTOR_USE_TIMER1 1
#define SERVOMOTOR_USE_TIMER3 1
#define SERVOMOTOR_USE_TIMER4 1
#define SERVOMOTOR_USE_TIMER5 1

#include <ES.h>
#include <ServoMotor.h>
#include <HWTimer.h>
//#include <Timer.h>
#include <TWI.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdint.h>
#include <util/delay.h>

volatile uint8_t timer;

void onTimer5CompareA() {
    //TODO handle servo update
}

void toggle_port()
{
    PORTA = ~PORTA;
}

void twiOnRequest() {
    //TODO handle commands from TWI module
}

int main()
{
    DDRA  |= _BV(PA0);
    PORTA &= ~_BV(PA0);

    TWI.setOnRequestHandler(twiOnRequest);

    *HWTimer0.TCNTn = 5;

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
