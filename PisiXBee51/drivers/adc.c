/*
 * adc.c
 *
 * Created: 18.12.2014 17:50:18
 *  Author: Rain
 */


#include "adc.h"

static int8_t offset;

void adc_init(void)
{
    /* Move stored calibration values to ADC A */
    ADC_CalibrationValues_Load(&ADCA);

    /* Set up ADC A to have signed conversion mode and 12 bit resolution. */
    ADC_ConvMode_and_Resolution_Config(&ADCA, true, ADC_RESOLUTION_12BIT_gc);

    // The ADC has different voltage reference options, controlled by the REFSEL bits in the
    // REFCTRL register. Here the internal reference is selected
    // IntVCC = Vcc / 1.6 = 2,0625 V
    ADC_Reference_Config(&ADCA, ADC_REFSEL_INTVCC_gc);

    // The clock into the ADC decide the maximum sample rate and the conversion time, and
    // this is controlled by the PRESCALER bits in the PRESCALER register. Here, the
    // Peripheral Clock is divided by 64 ( gives 500 KSPS with 32Mhz clock )
    ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV64_gc);

    // The used Virtual Channel (CH0) must be set in the correct mode
    // In this task we will use single ended input, so this mode is selected

    /* Setup channel 0 to have single ended input. */
    ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH0,
                                     ADC_CH_INPUTMODE_SINGLEENDED_gc,
                                     ADC_CH_GAIN_1X_gc);

    // Setting up the which pins to convert.
    // Note that the negative pin is internally connected to ground
    ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_MUXPOS_PIN8_gc, ADC_CH_MUXNEG_PIN0_gc);


    // Before the ADC can be used it must be enabled
    ADC_Enable(&ADCA);


    // Wait until the ADC is ready
    ADC_Wait_8MHz(&ADCA);

    /* Get offset value for ADCA.  */
    offset = ADC_Offset_Get_Signed(&ADCA, &(ADCA.CH0), true);
}

uint16_t adc_read(uint8_t channel)
{
    ADC_Ch_InputMux_Config(&ADCA.CH0, (channel << 3), ADC_CH_MUXNEG_PIN0_gc);

    // Start a single conversion
    ADC_Ch_Conversion_Start(&ADCA.CH0);

    while(!ADC_Ch_Conversion_Complete(&ADCA.CH0));

    return ADC_ResultCh_GetWord_Signed(&ADCA.CH0, offset);
}

