/*
 motor.c
 *
 * Created: 18.12.2014 17:50:18
 *  Author: Rain
 */

#include "motor.h"
#include <util/delay.h>

void motor_init(void)
{
    // Set motor pins as output and set them low
    PORT_SetPinsAsOutput(&MOTOR_PORT,MOTOR_PINMASK);
    PORT_ClearPins(&MOTOR_PORT,MOTOR_PINMASK);

    // Configure PORTD Timer 0 as Single Slope PWM source 32kHz
    TC_SetPeriod(&MOTOR_TIMER,1000);
    TC0_ConfigWGM(&MOTOR_TIMER,TC_WGMODE_SS_gc);
    TC0_DisableCCChannels(&MOTOR_TIMER,(TC0_CCAEN_bm|TC0_CCBEN_bm|TC0_CCCEN_bm|TC0_CCDEN_bm));
    TC0_ConfigClockSource(&MOTOR_TIMER,TC_CLKSEL_DIV1_gc);

    TC_SetCompareA (&MOTOR_TIMER,0);
    TC_SetCompareB (&MOTOR_TIMER,0);
    TC_SetCompareC (&MOTOR_TIMER,0);
    TC_SetCompareD (&MOTOR_TIMER,0);
	
	PORTR.DIRSET = 3;
	PORTR.OUTCLR = 3;
	_delay_ms(1);
	PORTR.OUTSET = 3;
}

/*! \brief Number of lines in the quadrature encoder. */

void quadrature_init()
{
    /* Setup PORTC with pin 2 and 4 as input for QDPH0, don't invert IO pins.
      *
      * Setup event channel 0, pin 2 on PORTC as input, don't enable index.
      * Setup event channel 2, pin 4 on PORTC as input, don't enable index.
      * if index used then state 00 is set as the index reset state.
      *
      * Setup TCC0 with Event channel 0 and lineCount.
      * Setup TCC1 with Event channel 2 and lineCount.
      */
    QDEC_Total_Setup(&QDEC_PORT,                /*PORT_t * qPort*/
                     QDEC1_A,                   /*uint8_t qPin*/
                     false,                     /*bool invIO*/
                     0,                         /*uint8_t qEvMux*/
                     EVSYS_CHMUX_PORTC_PIN2_gc, /*EVSYS_CHMUX_t qPinInput*/
                     false,                     /*bool useIndex*/
                     EVSYS_QDIRM_00_gc,         /*EVSYS_QDIRM_t qIndexState*/
                     &TCC0,                     /*TC0_t * qTimer*/
                     TC_EVSEL_CH0_gc,           /*TC_EVSEL_t qEventChannel*/
                     1);						/*uint8_t lineCount*/
    QDEC_Total_Setup(&QDEC_PORT,                /*PORT_t * qPort*/
                     QDEC2_A,                   /*uint8_t qPin*/
                     false,                     /*bool invIO*/
                     2,                         /*uint8_t qEvMux*/
                     EVSYS_CHMUX_PORTC_PIN4_gc, /*EVSYS_CHMUX_t qPinInput*/
                     false,                     /*bool useIndex*/
                     EVSYS_QDIRM_00_gc,         /*EVSYS_QDIRM_t qIndexState*/
                     &TCC1,                     /*TC0_t * qTimer*/
                     TC_EVSEL_CH2_gc,           /*TC_EVSEL_t qEventChannel*/
                     1);						/*uint8_t lineCount*/

    PORTC.PIN4CTRL |= PORT_INVEN_bm;
}

void motor_right_set(int16_t right)
{
    if(right > 0)
    {
        // PWM to AIN1, AIN2 high
        TC0_DisableCCChannels(&MOTOR_TIMER,TC0_CCCEN_bm);
        TC0_EnableCCChannels(&MOTOR_TIMER,TC0_CCDEN_bm);

        PORT_SetOutputBit(&MOTOR_PORT,AIN2);
        TC_SetCompareD (&MOTOR_TIMER,1001-right);
    }
    else if (right < 0)
    {
        // PWM to AIN2, AIN1 high
        TC0_DisableCCChannels(&MOTOR_TIMER,TC0_CCDEN_bm);
        TC0_EnableCCChannels(&MOTOR_TIMER,TC0_CCCEN_bm);

        PORT_SetOutputBit(&MOTOR_PORT,AIN1);
        TC_SetCompareC (&MOTOR_TIMER,1001+right);
    }
    else
    {
        // Pins LOW (coast)
        TC0_DisableCCChannels(&MOTOR_TIMER,TC0_CCCEN_bm|TC0_CCDEN_bm);
        PORT_SetPins(&MOTOR_PORT,(1<<AIN1)|(1<<AIN2));
    }
}

void motor_left_set(int16_t left)
{
    if(left > 0)
    {
        // PWM to BIN1, BIN2 low
        TC0_DisableCCChannels(&MOTOR_TIMER,TC0_CCBEN_bm);
        TC0_EnableCCChannels(&MOTOR_TIMER,TC0_CCAEN_bm);

        PORT_SetOutputBit(&MOTOR_PORT,BIN2);
        TC_SetCompareA (&MOTOR_TIMER,1001-left);
    }
    else if (left < 0)
    {
        // PWM to BIN2, BIN1 low
        TC0_DisableCCChannels(&MOTOR_TIMER,TC0_CCAEN_bm);
        TC0_EnableCCChannels(&MOTOR_TIMER,TC0_CCBEN_bm);

        PORT_SetOutputBit(&MOTOR_PORT,BIN1);
        TC_SetCompareB (&MOTOR_TIMER,1001+left);
    }
    else
    {
        // Pins LOW (coast)
        TC0_DisableCCChannels(&MOTOR_TIMER,TC0_CCAEN_bm|TC0_CCBEN_bm);
        PORT_SetPins(&MOTOR_PORT,(1<<BIN1)|(1<<BIN2));
    }
}

void motor_set(int16_t left, int16_t right)
{
    if(right > 0)
    {
        // PWM to AIN1, AIN2 high
        TC0_DisableCCChannels(&MOTOR_TIMER,TC0_CCCEN_bm);
        TC0_EnableCCChannels(&MOTOR_TIMER,TC0_CCDEN_bm);

        PORT_SetOutputBit(&MOTOR_PORT,AIN2);
        TC_SetCompareD (&MOTOR_TIMER,1001-right);
    }
    else if (right < 0)
    {
        // PWM to AIN2, AIN1 high
        TC0_DisableCCChannels(&MOTOR_TIMER,TC0_CCDEN_bm);
        TC0_EnableCCChannels(&MOTOR_TIMER,TC0_CCCEN_bm);

        PORT_SetOutputBit(&MOTOR_PORT,AIN1);
        TC_SetCompareC (&MOTOR_TIMER,1001+right);
    }
    else
    {
        // Pins LOW (coast)
        TC0_DisableCCChannels(&MOTOR_TIMER,TC0_CCCEN_bm|TC0_CCDEN_bm);
        PORT_SetPins(&MOTOR_PORT,(1<<AIN1)|(1<<AIN2));
    }

    if(left > 0)
    {
        // PWM to BIN1, BIN2 low
        TC0_DisableCCChannels(&MOTOR_TIMER,TC0_CCBEN_bm);
        TC0_EnableCCChannels(&MOTOR_TIMER,TC0_CCAEN_bm);

        PORT_SetOutputBit(&MOTOR_PORT,BIN2);
        TC_SetCompareA (&MOTOR_TIMER,1001-left);
    }
    else if (left < 0)
    {
        // PWM to BIN2, BIN1 low
        TC0_DisableCCChannels(&MOTOR_TIMER,TC0_CCAEN_bm);
        TC0_EnableCCChannels(&MOTOR_TIMER,TC0_CCBEN_bm);

        PORT_SetOutputBit(&MOTOR_PORT,BIN1);
        TC_SetCompareB (&MOTOR_TIMER,1001+left);
    }
    else
    {
        // Pins LOW (coast)
        TC0_DisableCCChannels(&MOTOR_TIMER,TC0_CCAEN_bm|TC0_CCBEN_bm);
        PORT_SetPins(&MOTOR_PORT,(1<<BIN1)|(1<<BIN2));
    }
}