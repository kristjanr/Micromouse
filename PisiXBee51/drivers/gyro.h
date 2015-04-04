/*
 * adc.h
 *
 * Created: 18.12.2014 17:50:18
 *  Author: Rain
 */
#ifndef QYRO_H_
#define QYRO_H_


#include <avr/io.h>
#include "board.h"
#include "drivers/twi_master_driver.h"

/*! Defining an example slave address. */
#define ACCEL_ADDRESS   0b00011001
#define GYRO_ADDRESS    0b01101011

/*! Defining number of bytes in buffer. */
#define NUM_BYTES        8

/*! CPU speed 32MHz, BAUDRATE 400kHz and Baudrate Register Settings */
#define BAUDRATE	400000
#define TWI_BAUDSETTING TWI_BAUD(F_CPU, BAUDRATE)

// TWI module to use for IMU
#define QYRO_TWI	TWIE

// LSM330 register addresses
#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define CTRL_REG6 0x25
#define REFERENCE_A 0x26
#define REFERENCE 0x25
#define OUT_TEMP 0x26
#define STATUS_REG 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define FIFO_CTRL_REG 0x2E
#define FIFO_SRC_REG 0x2F
#define INT1_CFG 0x30
#define INT1_SRC 0x31
#define INT1_TSH_XH 0x32
#define INT1_TSH_XL 0x33
#define INT1_TSH_YH 0x34
#define INT1_TSH_YL 0x35
#define INT1_TSH_ZH 0x36
#define INT1_TSH_ZL 0x37
#define INT1_DURATION 0x38

/* Global variables */
TWI_Master_t twiMaster;    /*!< TWI master module. */
extern int16_t accel[3];
extern int16_t gyro[3];

// Initialize IMU sensor
void gyro_init(void);

// Read accelerometer data from IMU to global variables
void gyro_read_accel();
// Zero acelerometer data
void gyro_zero_accel();

// Read qyroscope data from IMU to global variables
void gyro_read_gyro();
//Zero gyro data
void gyro_zero_gyro();


#endif /* QYRO_H_ */