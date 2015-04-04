/*
 * main.c
 *
 * Created: 18.12.2014 17:50:18
 *  Author: Rain
 */

#include <avr/io.h>
#include "drivers/board.h"
#include "drivers/adc.h"
#include "drivers/motor.h"
#include "drivers/com.h"
#include "drivers/gyro.h"
#define Turn_time 350 //for 90 degrees turn
int front_left_sens = 0;
int left_sens = 1;
int left_diag_sens = 4;

int front_right_sens= 3;
int right_sens = 5;
int right_diag_sens = 2;
			
void delay_ms(uint16_t count) {
	while(count--) {
		_delay_ms(1);

	}
}

void turn_right_and_back() {
	int turn_time = 350;
	int16_t turn_speed = 1000;
	motor_left_set(turn_speed);
	delay_ms(turn_time);
	motor_left_set(0);
	delay_ms(turn_time);
	motor_left_set(-turn_speed);
	delay_ms(turn_time);
	motor_left_set(0);

}

void turn_left_and_back() {
	int16_t turn_speed = 1000;
	motor_right_set(turn_speed);
	delay_ms(Turn_time);
	motor_right_set(0);
	delay_ms(Turn_time);
	motor_right_set(-turn_speed);
	delay_ms(Turn_time);
	motor_right_set(0);
}

void turn_around(int right) {
	int16_t turn_speed = 1000;
	if (right) {
		motor_set(turn_speed, -turn_speed);
		} else {
		motor_set(-turn_speed, turn_speed);
	}
	delay_ms(350);
	motor_set(0, 0);
	
}

void straight(int move_time) {
	int16_t move_speed = 700;
	int count = move_time;
	while (count) {
		count -= 10;
		motor_set(move_speed, move_speed);
		delay_ms(10);
		int error = adc_read(left_diag_sens) - adc_read(right_diag_sens);
		int right = move_speed + error;
		int left = move_speed - error;
		motor_set(left,right);
	}
	motor_set(0, 0);
}

void send_debug_msg(char * buff) {
	int difference = adc_read(left_diag_sens) - adc_read(right_diag_sens);
	sprintf(buff,"%4d,%4d,%4d,%4d,%4d,%4d,%4d\n\r",
	adc_read(front_left_sens),adc_read(left_sens),adc_read(left_diag_sens),
	adc_read(front_right_sens), adc_read(right_sens), adc_read(right_diag_sens),
	difference);
	radio_puts(buff);
}

int main(void)
{
	char buff[30];
	clock_init();		// Seadista s?steemi kell 32MHz peale
	board_init();		// Seadista LED ja nupud
	adc_init();			// Seadista ADC kanal 0
	radio_init(57600);	// Seadista raadiomooduli UART
	motor_init();		// Seadista mootorikontroller
	
	while(1)
	{
		if(sw1_read())
		{
			rgb_set(RED);
			delay_ms(1000);
			turn_right_and_back();
			delay_ms(1000);
			turn_left_and_back();
			delay_ms(1000);
			turn_around(1);
			delay_ms(1000);
			turn_around(0);

		}
		else if(sw2_read())
		{
			rgb_set(BLUE);
			delay_ms(1000);
			straight(5000);
			delay_ms(1000);
		}
		else
		{
			rgb_set(GREEN);
			send_debug_msg(buff);

            _delay_ms(500);
		}

	}
}
