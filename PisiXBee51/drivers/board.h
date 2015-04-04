/*
 * board.h
 *
 * Created: 18.12.2014 17:50:18
 *  Author: Rain
 */

#ifndef BOARD_H_
#define BOARD_H_

#ifndef F_CPU
#define F_CPU 32000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include "drivers/port_driver.h"
#include "drivers/clksys_driver.h"
#include "drivers/tc_driver.h"

// Button definitions
#define SW_PORT		PORTD
#define SW1			(1<<4)
#define SW2			(1<<5)

// RGB LED definitions
#define LED_PORT	PORTB
#define LED_GREEN	2
#define LED_BLUE	3
#define LED_RED		1

typedef enum RGB_colour_enum
{
    OFF		= 0,
    RED		= 1,
    GREEN	= 2,
    YELLOW	= 3,
    BLUE	= 4,
    MAGENTA	= 5,
    PINK	= 5,
    TEAL	= 6,
    WHITE	= 7,
} RGB_colour;

// Motor pin definitions
#define MOTOR_PORT	PORTD
#define AIN1		3
#define BIN1		0
#define AIN2		2
#define BIN2		1
#define MOTOR_PINMASK	0x0F

// XBee pin definitions
#define COMPORT		PORTE
#define TXD			3
#define RXD			2

// IMU pin definitions
#define SDA			0
#define SCL			1

// Systick timer
#define SYSTICK_TIMER			TCE0
#define SYSTICK_TIMER_INTERRUPT	TCE0_OVF_vect
extern volatile uint32_t systick;

// Initialize board LEDs, switches and systick timer
inline void board_init()
{
    PORT_SetPinsAsOutput(&PORTB,0x0E);	// Set PORTB LED pins as output
    PORT_SetPinsAsInput(&PORTD,0x30);	// Set PORTD Button pins as input

    // Configure Systick timer 1kHz freq
    TC_SetPeriod(&SYSTICK_TIMER,32000);
    TC0_ConfigWGM(&SYSTICK_TIMER,TC_WGMODE_NORMAL_gc);
    TC0_SetOverflowIntLevel(&SYSTICK_TIMER,TC_OVFINTLVL_MED_gc);
    TC0_ConfigClockSource(&SYSTICK_TIMER,TC_CLKSEL_DIV1_gc);

    // Enable PMIC interrupt levels and interrupts globally
    PMIC.CTRL |= PMIC_LOLVLEX_bm|PMIC_MEDLVLEX_bm|PMIC_HILVLEX_bm;
    sei();
}

// Set value for RGB LED
inline void rgb_set(RGB_colour colour)
{
    PORT_SetOutputValue(&LED_PORT,((colour & 7) << 1)|(1 & PORTB.OUT));
}

inline int sw1_read()
{
    return (((~PORT_GetPortValue(&SW_PORT) & SW1) >> 4));
}

inline int sw2_read()
{
    return (((~PORT_GetPortValue(&SW_PORT) & SW2) >> 5));
}

inline void clock_init()
{
    /*  Enable internal 32 MHz ring oscillator and wait until it's
    	 *  stable. Divide clock by two with the prescaler C and set the
    	 *  32 MHz ring oscillator as the main clock source.
    	 */
    CLKSYS_Enable( OSC_RC32MEN_bm );
    CLKSYS_Prescalers_Config( CLK_PSADIV_1_gc, CLK_PSBCDIV_1_1_gc );
    do {}
    while ( CLKSYS_IsReady( OSC_RC32MRDY_bm ) == 0 );
    CLKSYS_Main_ClockSource_Select( CLK_SCLKSEL_RC32M_gc );
}

#endif /* BOARD_H_ */