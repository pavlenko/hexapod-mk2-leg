/*
 * spi.c
 *
 * Created: 04.04.2014 15:10:06
 *  Author: MASTER
 */
#include "spi.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
//Единственный вариант сделать неблокирующий алгоритм, построить передачу на конечном автомате

volatile uint8_t spi_buffer[SPI_BUFFER_MAX];
volatile uint8_t spi_buffer_head  = 0;
volatile uint8_t spi_buffer_tail  = 0;
volatile uint8_t spi_buffer_count = 0;

volatile spi_handler_t spi_handlers[SPI_HANDLERS_MAX];

void initialize_spi(void)
{
	volatile uint8_t tmp;
	
	SPI_DDR = _BV(SPI_MOSI)|_BV(SPI_SCK);
	SPCR    = _BV(SPE)|_BV(MSTR)|_BV(SPR0);
	
	tmp = SPSR;
	tmp = SPDR;
}

void spi_attach(uint8_t code, void (*handler_ptr) (void))
{
	if (code < SPI_HANDLERS_MAX) {
		spi_handlers[code] = handler_ptr;
	}
}

void spi_detach(uint8_t code)
{
	if (code < SPI_HANDLERS_MAX) {
		spi_handlers[code] = 0;
	}
}

void spi_putc(uint8_t data)
{
	if (spi_buffer_count < SPI_BUFFER_MAX) {
		spi_buffer[spi_buffer_tail] = data;
		spi_buffer_tail++;
		
		if (spi_buffer_tail == SPI_BUFFER_MAX) {
			spi_buffer_tail = 0;
		}
		
		spi_buffer_count++;
		SPCR = _BV(SPIE);
	}
}

uint8_t spi_getc(void)
{
	uint8_t data;
	
	if (spi_buffer_count > 0) {
		data = spi_buffer[spi_buffer_head];
		
		spi_buffer_count--;
		spi_buffer_head++;
		
		if (spi_buffer_head == SPI_BUFFER_MAX) {
			spi_buffer_head = 0;
		}
		
		return data;
	}
	
	return 0;
}

uint8_t spi_current(void)
{
	if (spi_buffer_count > 0) {
		return spi_buffer[spi_buffer_head];
	}
	
	return 0;
}

ISR(SPI_STC_vect)
{
	if (spi_handlers[SPI_ON_BEFORE_ISR]) {
		spi_handlers[SPI_ON_BEFORE_ISR]();
	}
	
	SPDR = spi_getc();
	
	if (spi_buffer_count == 0) {
		if (spi_handlers[SPI_ON_PACKET_COMPLETE]) {
			spi_handlers[SPI_ON_PACKET_COMPLETE]();
		}

		SPCR &= ~_BV(SPIE);
	}
}