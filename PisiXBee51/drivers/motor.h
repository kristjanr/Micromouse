/*
 * motor.h
 *
 * Created: 18.12.2014 17:50:18
 *  Author: Rain
 */
#ifndef MOTOR_H_
#define MOTOR_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "board.h"
#include "drivers/tc_driver.h"
#include "drivers/qdec_driver.h"

// Motor driver PWM timer
#define MOTOR_TIMER		TCD0

// Quadrature decoder inputs
#define QDEC_PORT		PORTC
#define QDEC1_A			2
#define QDEC1_B			3
#define QDEC2_A			4
#define QDEC2_B			5

#define RIGHTENC	TCC0.CNT
#define LEFTENC		TCC1.CNT
// Initialize motor controller
void motor_init(void);
// Initialize quadrature decoder
void quadrature_init();

// Set a new speed setpoint for left motor
void motor_left_set(int16_t left);
// Set a new speed setpoint for right motor
void motor_right_set(int16_t right);
// Set a new speed setpoint for both motors
void motor_set(int16_t left, int16_t right);


#endif /* MOTOR_H_ */