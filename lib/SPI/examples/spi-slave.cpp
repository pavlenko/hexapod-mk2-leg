#include <SPI.h>

int main() {
    //TODO init as slave
    SPI.initialize({.PORT = &PORTB, .SS = PB4, .SCK = PB7, .MOSI = PB5, .MISO = PB6});
}