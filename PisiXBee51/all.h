#ifndef ALL_H_
#define ALL_H_

#include <avr/io.h>
#include <stdlib.h>
#include "drivers/board.h"
#include "drivers/adc.h"
#include "drivers/motor.h"
#include "drivers/com.h"
#include "labyrinth.h"

#define LEFT_DIRECTION -90
#define RIGHT_DIRECTION 90
#define N 0
#define E 90
#define S 180
#define W -90
#define FRONT_LEFT_S 0
#define LEFT_S 1
#define LEFT_DIAG_S 4
#define FRONT_RIGHT_S 3
#define RIGHT_S  5
#define RIGHT_DIAG_S 2
#define TURN_SPEED 200
#define MAX_SPEED 200
#define SPEED 100
#define LEFT 2
#define RIGHT 1
#define FORWARD 0
#define BACKWARD -1
#define ARRAYSIZE (ARRAY_LENGTH * ARRAY_LENGTH)

void set_loc();

void read_set_walls();

void add_front_wall_info();

void add_left_wall_info();

void add_right_wall_info();

void delay_ms();

void motors();

void straight(int speed);

void mapping_run();

void stop();

void calibrate_front();

void gradual_stop();

void turn_if_needed();

void step();

void speed_run();

void set_wall(int, int, int);

void turn_R();

void turn_L();

void straight_sense_only();

void debug_sensors();

void move_one_square(int max_speed);

void move_one_square_turn_around_when_wall(int max_speed);

uint16_t get_left();

uint16_t get_left_diag();

uint16_t get_front_left();

uint16_t get_right();

uint16_t get_right_diag();

uint16_t get_front_right();

int wall();

int turns = 0;

int get_next_direction();

int n_direction();

int robot_direction = E;
int CurrentColumn = 0;
int CurrentRow = 0;

#endif /* ALL_H_ */