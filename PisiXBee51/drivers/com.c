/*
 * com.c
 *
 * Created: 18.12.2014 17:50:18
 *  Author: Rain
 */

#include "com.h"
#include <stdlib.h>
#include <util/delay.h>

// USART data struct used in task
USART_data_t USART_data;
uint8_t noPrint = 0;

void radio_init(uint32_t baud) {
    // (TX) as output
    COMPORT.DIRSET = (1 << TXD);
    // (RX) as input
    COMPORT.DIRCLR = (1 << RXD);

    // Use USARTE0 and initialize buffers
    USART_InterruptDriver_Initialize(&USART_data, &RADIO_USART, USART_DREINTLVL_LO_gc);

    // USARTE0, 8 Data bits, No Parity, 1 Stop bit
    USART_Format_Set(USART_data.usart, USART_CHSIZE_8BIT_gc, USART_PMODE_DISABLED_gc, false);

    // Enable RXC interrupt
    USART_RxdInterruptLevel_Set(USART_data.usart, USART_RXCINTLVL_HI_gc);

    // Set Baudrate
    USART_Baudrate_Set(&RADIO_USART, (((F_CPU) / (16 * baud))), 0);

    // Enable both RX and TX.
    USART_Rx_Enable(USART_data.usart);
    USART_Tx_Enable(USART_data.usart);

    // Enable PMIC interrupt level low
    PMIC.CTRL |= PMIC_LOLVLEX_bm;

    // Enable global interrupts
    sei();

}

// Send character to UART TX buffer
void radio_putc(char character) {
    while (1) {
        while (!USART_TXBuffer_FreeSpace(&USART_data));
        if (USART_TXBuffer_PutByte(&USART_data, character)) {
            return;
        }
    }
}

// Send string to UART TX buffer
void radio_puts(char *stringPtr) {
    while (*stringPtr) {
        radio_putc(*stringPtr++);
    }
}

// Wait for and get character from UART RX buffer
int radio_getc() {
    while (1) {
        if (USART_RXBufferData_Available(&USART_data)) {
            return USART_RXBuffer_GetByte(&USART_data);
        }
    }
}

// Get character from UART RX buffer
int radio_getc_nolock() {
    if (USART_RXBufferData_Available(&USART_data)) {
        return USART_RXBuffer_GetByte(&USART_data);
    }
    return 0;
}

// Check if we have data availible in RX buffer
int radio_available() {
    return USART_RXBufferData_Available(&USART_data);
}

// Get string from UART RX buffer
int radio_gets(char *stringPtr) {
    if (USART_RXBufferData_Available(&USART_data) == 0) return 0;
    while (1) {
        if (USART_RXBufferData_Available(&USART_data)) {
            *stringPtr = USART_RXBuffer_GetByte(&USART_data);
            if (*stringPtr == 0) return 1;
            stringPtr++;
        }
    }
}



//  Receive complete interrupt service routine.
//  Calls the common receive complete handler with pointer to the correct USART
//  as argument.
ISR( RADIO_USART_RXC_vect ) // Note that this vector name is a define mapped to the correct interrupt vector
        {
                USART_RXComplete( &USART_data );
        }


//  Data register empty  interrupt service routine.
//  Calls the common data register empty complete handler with pointer to the
//  correct USART as argument.
ISR( RADIO_USART_DRE_vect ) // Note that this vector name is a define mapped to the correct interrupt vector
        {
                USART_DataRegEmpty( &USART_data );
        }
