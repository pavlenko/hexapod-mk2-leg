#include <ES.h>
#include <Timer.h>

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

int main(void)
{
    DDRA  |= _BV(PA0);
    PORTA &= ~_BV(PA0);

    TCCR1A = 0x00;
    TCCR1B = _BV(WGM12)|_BV(CS12)|_BV(CS10);

    TCNT1L = 0x00;
    TCNT1H = 0x00;

    OCR1AL = 0x40;
    OCR1AH = 0x9C;

    TIFR  |= _BV(OCF1A);
    TIMSK |= _BV(OCIE1A);

    sei();

    Timer0.setClockSource(TIMER_CLOCK_DIVIDE_BY_8);

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

ISR(TIMER1_COMPA_vect)
{
    timer++;

    if (timer == 10) {
        timer = 0;
        //PORTA = ~PORTA;
        ES.trigger(1);
    }
}
