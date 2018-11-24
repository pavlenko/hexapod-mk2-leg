/*
 * spi.h
 *
 * Created: 04.04.2014 15:09:49
 *  Author: MASTER
 */ 
#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <stdint.h>

typedef void (*spi_handler_t) (void);

#define SPI_DDR  DDRB
#define SPI_PORT PORTB
#define SPI_SS   PB0
#define SPI_SCK  PB1
#define SPI_MOSI PB2
#define SPI_MISO PB3

#define SPI_BUFFER_MAX 64

#define SPI_ON_BEFORE_ISR 0
#define SPI_ON_PACKET_COMPLETE 1
#define SPI_HANDLERS_MAX 2

void initialize_spi(void);

void spi_attach(uint8_t code, void (*handler_ptr) (void));

void spi_detach(uint8_t code);

void spi_putc(uint8_t data);

uint8_t spi_getc(void);

uint8_t spi_current(void);

#endif /* SPI_H_ */