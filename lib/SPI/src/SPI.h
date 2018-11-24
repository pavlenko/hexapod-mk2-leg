#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <stdint.h>
#include <stddef.h>

// ATMega16/32: SS=PB4, SCK=PB7, MOSI=PB5, MISO=PB6
// ATMega8: SS=PB2, SCK=PB5, MOSI=PB3, MISO=PB4
// ATMega64(0)/128(0/1)/256(0/1): SS=PB0, SCK=PB1, MOSI=PB2, MISO=PB3

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

enum SPIClockMode {
    SPI_CLOCK_SAMPLE_RISING_NORMAL,
    SPI_CLOCK_SAMPLE_FALLING_NORMAL,
    SPI_CLOCK_SAMPLE_RISING_INVERTED,
    SPI_CLOCK_SAMPLE_FALLING_INVERTED,
};

//TODO for slave pass {port, ddr, mosi pin, miso pin, sck pin, ss pin}
//TODO for master pass {port, ddr, mosi pin, miso pin, sck pin}
//TODO master multiple slave support: for each slave pass SS pin {port, ddr, pinnum}

class SPI2;

class SPIConnection {
    friend SPI2;
private:
    volatile uint8_t *PORT;
    volatile uint8_t *DDR;
};

class SPIMasterConnection: public SPIConnection {
public:
    SPIMasterConnection();
};

class SPISlaveConnection: public SPIConnection {
public:
    SPISlaveConnection();
};

typedef struct {
    volatile uint8_t *port;
    volatile uint8_t *ddr;
} SPIMasterConnection_t;

typedef struct {
    volatile uint8_t *port;
    volatile uint8_t *ddr;
} SPISlaveConnection_t;

typedef struct {
    volatile uint8_t *port;
    volatile uint8_t *ddr;
    uint8_t pin;
} SPIChipSelect_t;

class SPIChipSelect {
    friend SPI2;
private:
    volatile uint8_t *port;
    volatile uint8_t *ddr;
    uint8_t pin;
};

class SPI2 {
private:
    uint8_t connection;
    SPIChipSelect *slave;
public:
    /**
     * Initialize SPI as slave
     *
     * @param connection
     */
    void initialize(SPISlaveConnection connection) {
        *(connection.DDR) |= _BV(connection.MISO);

        // Set SPI as slave
        SPCR &= ~_BV(MSTR);
    }

    /**
     * Enable SPI and select slave device
     *
     * @param slave
     */
    void start(SPIChipSelect &slave) {
        this->slave = &slave;

        if (this->slave) {
            // Set pin as output
            *(this->slave->ddr) |= _BV(this->slave->pin);

            // Set pin low to select slave
            *(this->slave->port) &= ~_BV(this->slave->pin);
        }

        // Enable SPI
        SPCR |= _BV(SPE);
    }

    /**
     * Transmit one byte
     *
     * @param data
     * @return
     */
    uint8_t transmit(uint8_t data) {
        // Store data to transmit buffer
        SPDR = data;

        /**
         * The following NOP introduces a small delay that can prevent the wait loop form iterating when running at the
         * maximum speed.
         * This gives about 10% more speed, even if it seems counter-intuitive.
         * At lower speeds it is unnoticed.
         */
        asm volatile("nop");

        // Wait while transmission completed
        while (!(SPSR & _BV(SPIF)));

        // Return received data
        return SPDR;
    }

    /**
     * Disable SPI and disconnect from slave device
     */
    void end() {
        // Disable SPI
        SPCR &= ~_BV(SPE);

        if (this->slave) {
            // Set pin high to release slave
            *(this->slave->port) |= _BV(this->slave->pin);
        }
    }
};


typedef struct {
    volatile uint8_t *DDR;
    int8_t SS: 4;
    int8_t SCK: 4;
    int8_t MOSI: 4;
    int8_t MISO: 4;
} SPIConnection_t;

typedef struct {
    SPIClockSource clockSource;
    SPIClockMode clockMode;
    SPIDataOrder dataOrder;
} SPISettings_t;

class SPIClass {
private:
    SPIConnection_t connection;
public:
    void initialize(SPIConnection_t connection, SPISettings_t settings);
    void initialize(SPIConnection_t connection);
    //TODO other transfer methods
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
