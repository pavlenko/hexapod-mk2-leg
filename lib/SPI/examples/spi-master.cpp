#include <SPI.h>

int main() {
    // Initialize SPI connection
    SPI.initialize({&DDRB, PB4, PB7, PB5, PB6});

    // Configure SPI
    SPI.setBusMode(SPI_BUS_MASTER);
    SPI.setClockSource(SPI_CLOCK_DIVIDE_BY_4, false);
    SPI.setDataOrder(SPI_DATA_ORDER_MSB_FIRST);
    SPI.setDataMode(SPI_MODE_SAMPLE_RISING_NORMAL);

    // Connect to slave by defined pin configuration
    SPI.start({&PORTB, &DDRB, PB4});

    // Send byte
    SPI.transfer(0xFF);

    // Receive byte by send 0
    uint8_t response = SPI.transfer(0x00);

    // Disconnect
    SPI.end();
}