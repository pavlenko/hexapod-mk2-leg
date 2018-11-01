#include <ES.h>
#include <Timer0.h>
#include <TWI.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdint.h>
#include <util/delay.h>

volatile uint8_t timer;

void toggle_port()
{
    PORTA = ~PORTA;
}

void timer0_ovf()
{
    Timer0.setValue(5);
    timer++;

    if (timer == 200) {
        timer = 0;
        ES.trigger(1);
    }
}

int main()
{
    DDRA  |= _BV(PA0);
    PORTA &= ~_BV(PA0);

    TWI.setOnRequestHandler(timer0_ovf);

    Timer0.setClockSource(TIMER0_CLOCK_DIVIDE_BY_8);
    Timer0.setValue(5);
    Timer0.setInterruptHandler(TIMER0_OVERFLOW_INTERRUPT, timer0_ovf);

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
