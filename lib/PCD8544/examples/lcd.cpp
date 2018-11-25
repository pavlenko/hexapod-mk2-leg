#include <PCD8544.h>
#include <SPI.h>
#include <util/delay.h>

int main() {
    SPI.initialize({&DDRB, PIN4, PIN7, PIN5, PIN6});

    PCD8544.initialize({&PORTB, &DDRB, PIN2}, {&PORTB, &DDRB, PIN3}, {&PORTB, &DDRB, PIN4});

    while (true) {
        PCD8544.write("HELLO");
        _delay_ms(1000);
        PCD8544.flush();
        _delay_ms(1000);
    }
}