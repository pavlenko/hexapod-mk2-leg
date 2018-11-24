/*
 * pcf8813.c
 *
 * Created: 04.04.2014 12:14:05
 *  Author: MASTER
 */
#include "pcf8813.h"

#include "spi.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sfr_defs.h>

#define PCF8813_DC_PORT PORTB
#define PCF8813_DC_PIN  PB1

#define PCF8813_RAMSIZE 816

#define PCF8813_VPR_DEFAULT 64

#define PCF8813_H 0
#define PCF8813_V 1
#define PCF8813_PD 2
#define PCF8813_MY 3
#define PCF8813_MX 4
#define PCF8813_PRS 0
#define PCF8813_E 0
#define PCF8813_D 2
#define PCF8813_BRS 0
#define PCF8813_DO 2

#define PCF8813_COMMAND() (PCF8813_DC_PORT &= ~_BV(PCF8813_DC_PIN))
#define PCF8813_DATA() (PCF8813_DC_PORT |= _BV(PCF8813_DC_PIN))

#define PCF8813_SET_FUNCTION 0x20
#define PCF8813_SET_PROGRAM_RANGE 0x10
#define PCF8813_SET_VPR 0x80
#define PCF8813_DISPLAY_CONTROL 0x08
#define PCF8813_DISPLAY_CONFIG 0x16
#define PCF8813_SET_X 0x80
#define PCF8813_SET_Y 0x40
#define PCF8813_SET_DATA_LENGTH 0x70

volatile uint8_t pcf8813_datalen = 0;

void _pcf8813_spi_on_before_isr(void)
{
	if (pcf8813_datalen == 0 && spi_current() == PCF8813_SET_DATA_LENGTH) {
		spi_getc();
		pcf8813_datalen = spi_getc();
	}
	
	if (pcf8813_datalen > 0) {
		PCF8813_DATA();
		pcf8813_datalen--;
	} else {
		PCF8813_COMMAND();
	}
}

void initialize_pcf8813(void)
{
	// Ensure command mode
	PCF8813_COMMAND();
	
	// Add control of D/C line
	spi_attach(SPI_ON_BEFORE_ISR, _pcf8813_spi_on_before_isr);
	
	// Function set: select extended instruction set (H = 1)
	spi_putc(PCF8813_SET_FUNCTION | (1 << PCF8813_H));
	
	// Set lower/higher program range: select higher (PRS = 1)
	spi_putc(PCF8813_SET_PROGRAM_RANGE | (1 << PCF8813_PRS));
	
	// Set VPR: select middle value (VPR = 64)
	spi_putc(PCF8813_SET_VPR | PCF8813_VPR_DEFAULT);
	
	// Function set: select normal instruction set (H = 0)
	spi_putc(PCF8813_SET_FUNCTION);
	
	// Display control: set normal mode (D = 1; E = 0)
	spi_putc(PCF8813_DISPLAY_CONTROL | (1 << PCF8813_D));
	
	// Display configuration: set data order LSB is on top (DO = 0)
	spi_putc(PCF8813_DISPLAY_CONFIG);
	spi_putc(0);
	
	//TODO check needed below
	//spi_putc(PCF8813_SET_X);//x=0
	//spi_putc(PCF8813_SET_Y);//y=0
}

void pcf8813_putc(uint8_t data)
{
	PCF8813_DATA();
	for (uint8_t i = 0; i < 5; i++) {
		//spi_send((uint8_t) pgm_read_byte(pcf8813_font[data - 32][i]) << 1);
	}
}
