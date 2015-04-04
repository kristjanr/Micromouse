/*
 * adc.c
 *
 * Created: 18.12.2014 17:50:18
 *  Author: Rain
 */


#include "gyro.h"

// Local function prototypes
void gyro_write_byte(uint8_t slave, uint8_t address, uint8_t data);
uint8_t gyro_read_byte(uint8_t address);
uint16_t gyro_read_word(uint8_t address);

int16_t accel[3];
int16_t accel_zero[3];
int16_t gyro[3];
int16_t gyro_zero[3];

void gyro_init(void)
{
    /* Initialize TWI master. */
    TWI_MasterInit(&twiMaster,
                   &QYRO_TWI,
                   TWI_MASTER_INTLVL_HI_gc,
                   TWI_BAUDSETTING);
    /* Enable LO interrupt level. */
    PMIC.CTRL |= PMIC_LOLVLEN_bm;
    sei();

    //gyro_write_byte(ACCEL_ADDRESS,CTRL_REG1,0b01110111);
    gyro_write_byte(GYRO_ADDRESS,CTRL_REG1,0b11111100); // ODR = 760Hz, Cut-off = 100, y axis enabled
    gyro_write_byte(GYRO_ADDRESS,CTRL_REG4,0b00110000); // 2000 dps
    gyro_write_byte(GYRO_ADDRESS,CTRL_REG5,0b00010000);
}

void gyro_write_byte(uint8_t slave, uint8_t address, uint8_t data)
{
    // Initialize and fill data buffer
    uint8_t data_buf[2];
    data_buf[0] = address;
    data_buf[1] = data;
    // Transmit bytes
    TWI_MasterWrite(&twiMaster,slave,data_buf,2);
    // Wait for transmission completion
    while (twiMaster.status != TWIM_STATUS_READY);
}

uint8_t gyro_read_byte(uint8_t address)
{
    uint8_t data_buf[1];
    data_buf[0] = address;
    // Send address to read from and read 1 byte of data
    return TWI_MasterWriteRead(&twiMaster,ACCEL_ADDRESS,data_buf,1,1);
}

uint16_t gyro_read_word(uint8_t address)
{
    uint8_t data_buf[1];
    data_buf[0] = address|128;
    // Send address to read from and read 2 bytes of data
    return TWI_MasterWriteRead(&twiMaster,ACCEL_ADDRESS,data_buf,1,2);
}

void gyro_read_accel()
{
    uint8_t data_buf[1];
    data_buf[0] = STATUS_REG|128;
    // Read acceleration words and status register
    TWI_MasterWriteRead(&twiMaster,ACCEL_ADDRESS,data_buf,1,7);
    while (twiMaster.status != TWIM_STATUS_READY); // wait for transmission completion
    if(twiMaster.readData[0] & 8) // If we have new data to update
    {
        accel[0] = ((twiMaster.readData[2]<<8)|twiMaster.readData[1]) - accel_zero[0];
        accel[1] = ((twiMaster.readData[4]<<8)|twiMaster.readData[3]) - accel_zero[1];
        accel[2] = ((twiMaster.readData[6]<<8)|twiMaster.readData[5]) - accel_zero[2];
    }
}

void gyro_zero_accel()
{
    uint8_t data_buf[1];
    data_buf[0] = STATUS_REG|128;
    // Read acceleration words and status register
    TWI_MasterWriteRead(&twiMaster,ACCEL_ADDRESS,data_buf,1,7);
    while (twiMaster.status != TWIM_STATUS_READY); // wait for transmission completion
    if(twiMaster.readData[0] & 8) // If we have new data, save new "zero" values
    {
        accel_zero[0] = ((twiMaster.readData[2]<<8)|twiMaster.readData[1]);
        accel_zero[1] = ((twiMaster.readData[4]<<8)|twiMaster.readData[3]);
        accel_zero[2] = ((twiMaster.readData[6]<<8)|twiMaster.readData[5]);
    }
}

void gyro_read_gyro()
{
    uint8_t data_buf[1];
    data_buf[0] = STATUS_REG|128;

    if((twiMaster.readData[0] & 0b00000100) && (twiMaster.status == TWIM_STATUS_READY)) // New data on z axis
    {
        gyro[0] = ((twiMaster.readData[2]<<8)|twiMaster.readData[1]) - gyro_zero[0];
        gyro[1] = ((twiMaster.readData[4]<<8)|twiMaster.readData[3]) - gyro_zero[1];
        gyro[2] = ((twiMaster.readData[6]<<8)|twiMaster.readData[5]) - gyro_zero[2];
        TWI_MasterWriteRead(&twiMaster,GYRO_ADDRESS,data_buf,1,7);
    }
    else
        TWI_MasterWriteRead(&twiMaster,GYRO_ADDRESS,data_buf,1,7);
}

void gyro_zero_gyro()
{
    uint8_t data_buf[1];
    data_buf[0] = STATUS_REG|128;
    do
    {
        TWI_MasterWriteRead(&twiMaster,GYRO_ADDRESS,data_buf,1,7);
        while (twiMaster.status != TWIM_STATUS_READY); // wait for transmission completion
    }
    while (!(twiMaster.readData[0] & 0b00000100)); // Try to read new z axis data until we get an update and a new value
    // Update new value
    gyro_zero[0] = ((twiMaster.readData[2]<<8)|twiMaster.readData[1]);
    gyro_zero[1] = ((twiMaster.readData[4]<<8)|twiMaster.readData[3]);
    gyro_zero[2] = ((twiMaster.readData[6]<<8)|twiMaster.readData[5]);
}

/*! TWIC Master Interrupt vector. */
ISR(TWIE_TWIM_vect)
{
    TWI_MasterInterruptHandler(&twiMaster);
}
