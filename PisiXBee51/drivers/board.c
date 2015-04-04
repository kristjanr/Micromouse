/*
 * board.c
 *
 * Created: 18.12.2014 17:50:18
 *  Author: Rain
 */

#include "board.h"

volatile uint32_t systick = 0;

// Systick interrupt service
ISR(SYSTICK_TIMER_INTERRUPT)
{
    systick++;
}
