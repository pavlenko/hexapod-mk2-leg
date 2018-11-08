#include <ES.h>
#include <ServoMotor.h>
#include <Timer.h>
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

    Timer5.setClockSource(TIMER_CLOCK_DIVIDE_BY_8);
    Timer5.setInterruptHandler(TIMER5_ISR_OUTPUT_COMPARE_A, onTimer5CompareA);

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
