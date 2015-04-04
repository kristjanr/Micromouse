/*
 * adc.h
 *
 * Created: 18.12.2014 17:50:18
 *  Author: Rain
 */
#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include "board.h"
#include "drivers/adc_driver.h"

// Initialize ADC module
void adc_init(void);

// Read ADC MUX channel (Locking function)
// Parameters:
//	channel - ADC MUX channel number to read
// Returns:
//	channel ADC read result
uint16_t adc_read(uint8_t channel);

#endif /* ADC_H_ */