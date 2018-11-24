#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <stdint.h>
#include <stddef.h>

enum SPIDataOrder {
    SPI_DATA_ORDER_MSB,
    SPI_DATA_ORDER_LSB,
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

typedef struct {
    volatile uint8_t *DDR;
    uint8_t SS: 4;
    uint8_t SCK: 4;
    uint8_t MOSI: 4;
    uint8_t MISO: 4;
} SPIConnection_t;

typedef struct {
    SPIClockSource clockSource;
    SPIClockMode clockMode;
    SPIDataOrder dataOrder;
} SPISettings_t;

typedef struct {
    volatile uint8_t *PORT;
    volatile uint8_t *DDR;
    uint8_t SS;
} SPISlaveSelect_t;

class SPIClass {
private:
    SPISlaveSelect_t slaveSelect;
public:
    /**
     * Initialize SPI as master
     *
     * @param connection
     * @param settings
     */
    void initialize(SPIConnection_t connection, SPISettings_t settings);

    /**
     * Initialize SPI as slave
     *
     * @param connection
     */
    void initialize(SPIConnection_t connection);

    /**
     * Enable SPI and start transmission to slave
     *
     * @param slave
     */
    void start(SPISlaveSelect_t slave);

    /**
     * Transfer data between master and slave
     *
     * @param data
     * @return
     */
    uint8_t transfer(uint8_t data);

    /**
     * Disable SPI and complete transmission
     */
    void end();
};

SPIClass SPI;

#endif //SPI_H
