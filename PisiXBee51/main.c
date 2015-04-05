/*
* main.c
*
* Created: 18.12.2014 17:50:18
*  Author: Rain
*/

#include <avr/io.h>
#include <stdlib.h>
#include "drivers/board.h"
#include "drivers/adc.h"
#include "drivers/motor.h"
#include "drivers/com.h"
#include "drivers/gyro.h"
#define FRONT_LEFT_S 0
#define LEFT_S 1
#define LEFT_DIAG_S 4

#define FRONT_RIGHT_S 3
#define RIGHT_S  5
#define RIGHT_DIAG_S 2
bool random_turn = true;
void delay_ms();
void send_debug_msg();
uint16_t get_left();
uint16_t get_left_diag();
uint16_t get_front_left();
uint16_t get_right();
uint16_t get_right_diag();
uint16_t get_front_right();
void forward();
void turn(int);
void motors();
void straight();
void go();
void stop();
void turn_around();
int turn_seq = 0;
bool directions[] = {false, false, true, true, true, true, false, true, true, true, true, true, false, false, true}; 
#define RIGHT 1
#define LEFT 2
#define RANDOM 0


int main(void) {
	clock_init();        // Seadista systeemi kell 32MHz peale
	board_init();        // Seadista LED ja nupud
	adc_init();            // Seadista ADC kanal 0
	radio_init(57600);    // Seadista raadiomooduli UART
	motor_init();        // Seadista mootorikontroller
	char buff[30];
	rgb_set(BLUE);
	while(1) {
		if(sw2_read()) {
			rgb_set(RED);
			_delay_ms(1000);
			go();
		}
		_delay_ms(400);
		send_debug_msg(buff);
	}
}

bool wall()
{
	return get_front_left() > 180 && get_front_right() > 180;
}

void go() {
	char buff[30];
	int count = 0;
	while(!sw1_read()) {
		count ++;
		if (count % 250 == 0) {
			send_debug_msg(buff);
		}
		int side_diff = abs(get_left() - get_right());
		if (wall()) {
			turn_around();
		} else if (side_diff > 35 && get_left() < 40) {
			turn(2);
		} else if (side_diff > 35 && get_right() < 40) {
			turn(1);
		} else if (side_diff <= 35 && get_right() < 40 && get_left() < 40 && !wall()) {
			turn(0);
		} else {
			straight();
			_delay_ms(5);
		}
	}
	rgb_set(BLUE);
	_delay_ms(1000);
	stop();
	_delay_ms(300);
	rgb_set(GREEN);
}

void straight() {
	int diag_diff = get_left_diag() - get_right_diag();
	motors(400 + diag_diff, 400 - diag_diff);
}

void motors(int16_t l_speed, int16_t r_speed) {
	// correct the slight curving to right
	int16_t left = l_speed - 12;
	motor_set(left, r_speed);
}

void turn(int direction) {
	if (!directions[turn_seq]) {
		motors(500, 500);
		_delay_ms(700);		
		turn_seq ++;
		return;
	}
	if (direction != RANDOM) {
		turn_seq ++;
		_delay_ms(500);
		if (direction == RIGHT) {
			motors(500, 0);
		} else if (direction == LEFT){
			motors(0, 500);
		}
		_delay_ms(625);
		motors(500, 500);
		_delay_ms(300);
	} else {
		turn(RIGHT);
	}
}

void turn_around(){
	motor_set(500, -500);
	_delay_ms(675);
}

void stop(){motor_set(0, 0);}

void send_debug_msg(char * buff) {
	sprintf(
	buff, "%4d,%4d,%4d,%4d,%4d,%4d\n\r",
	get_front_left(), get_left_diag(), get_left(),
	get_front_right(), get_right_diag(), get_right()
	);
	
	radio_puts(buff);
}

void delay_ms(uint16_t count) {
	while (count--) {
		_delay_ms(1);
	}
}

uint16_t get_front_left() {
	return adc_read(FRONT_LEFT_S) - 4;
}

uint16_t get_left_diag() { return adc_read(LEFT_DIAG_S); }

uint16_t get_left() {
	return adc_read(LEFT_S);
}

uint16_t get_front_right() {
	return adc_read(FRONT_RIGHT_S);
}

uint16_t get_right_diag() {
	return adc_read(RIGHT_DIAG_S) + 3;
}
uint16_t get_right() {
	return adc_read(RIGHT_S) - 2;
}
