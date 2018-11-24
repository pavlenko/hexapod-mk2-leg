#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <stdint.h>

// ATMega16/32: SS=PB4, SCK=PB7, MOSI=PB5, MISO=PB6
// ATMega8: SS=PB2, SCK=PB5, MOSI=PB3, MISO=PB4
// ATMega64(0)/128(0/1)/256(0/1): SS=PB0, SCK=PB1, MOSI=PB2, MISO=PB3

typedef struct {
    volatile uint8_t *PORT;
    int8_t SS: 4;
    int8_t SCK: 4;
    int8_t MOSI: 4;
    int8_t MISO: 4;
} SPIConnection;

enum SPIDataOrder {
    SPI_DATA_ORDER_MSB,
    SPI_DATA_ORDER_LSB,
};

enum SPIMode {
    SPI_MODE_SLAVE,
    SPI_MODE_MASTER,
};

enum SPIClockPolarity {
    SPI_CLOCK_POLARITY_NORMAL, // <-- SCK is low when idle
    SPI_CLOCK_POLARITY_INVERSE,// <-- SCK is high when idle
};

enum SPIClockPhase {
    SPI_CLOCK_PHASE_FALLING,
    SPI_CLOCK_PHASE_RAISING,
};

enum SPIClockSource {
    SPI_CLOCK_DIVIDE_BY_4,
    SPI_CLOCK_DIVIDE_BY_16,
    SPI_CLOCK_DIVIDE_BY_64,
    SPI_CLOCK_DIVIDE_BY_128,
};

class SPIClass {
private:
    SPIConnection connection;
public:
    void initialize(SPIConnection connection) {
        this->connection = connection;
    }
    void setDataOrder(SPIDataOrder dataOrder) {
        SPCR = (uint8_t) ((SPCR & _BV(DORD)) | (dataOrder << DORD));
    }
    void setMode(SPIMode mode) {
        SPCR = (uint8_t) ((SPCR & _BV(MSTR)) | (mode << MSTR));
    }
    void setClockPolarity(SPIClockPolarity polarity) {
        SPCR = (uint8_t) ((SPCR & _BV(CPOL)) | (polarity << CPOL));
    }
    void setClockPhase(SPIClockPhase phase) {
        SPCR = (uint8_t) ((SPCR & _BV(CPHA)) | (phase << CPHA));
    }
    void setClockSource(SPIClockSource clockSource, bool x2) {
        SPCR = (uint8_t) ((SPCR & 0x11111100) | clockSource);
        SPSR = (uint8_t) ((SPSR & ~_BV(SPI2X)) | x2);
    }
    uint8_t transfer(uint8_t data) {
        SPDR = data;

        /*
         * The following NOP introduces a small delay that can prevent the wait loop form iterating when running at the
         * maximum speed.
         * This gives about 10% more speed, even if it seems counter-intuitive.
         * At lower speeds it is unnoticed.
         */
        asm volatile("nop");

        while (!(SPSR & _BV(SPIF)));

        return SPDR;
    }
};

SPIClass SPI;

#endif //SPI_H
