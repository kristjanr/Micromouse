/*
 * com.h
 *
 * Created: 18.12.2014 17:50:18
 *  Author: Rain
 */
#ifndef COM_H_
#define COM_H_

#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "board.h"
#include "drivers/usart_driver.h"

#define RADIO_USART USARTE0
#define RADIO_USART_RXC_vect USARTE0_RXC_vect
#define RADIO_USART_DRE_vect USARTE0_DRE_vect

// Initialize radio with 32 char Tx/Rx buffer
void radio_init(uint32_t baud);

// Add character to Tx buffer
void radio_putc(char character);

// Add string to Tx buffer
void radio_puts(char* stringPtr);

// Read character from Rx buffer
// Waits forever for character
int radio_getc();

// Read character from Rx buffer
// Returns 0 if no character
int radio_getc_nolock();

// Reads string from Rx buffer
// 0 terminates
int radio_gets(char* stringPtr);


#endif /* COM_H_ */