/*
 * pcf8813.h
 *
 * Created: 04.04.2014 12:14:22
 *  Author: MASTER
 */ 


#ifndef PCF8813_H_
#define PCF8813_H_

#include <stdint.h>

void initialize_pcf8813(void);
void pcf8813_putc(uint8_t data);

#endif /* PCF8813_H_ */