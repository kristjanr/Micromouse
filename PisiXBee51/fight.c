/*
* main.c
*
* Created: 04.04.2015 12:00:00
*  Author: Kristjan Roosild
*/

#include <avr/eeprom.h>
#include <avr/io.h>
#include <stdlib.h>
#include "drivers/board.h"
#include "drivers/adc.h"
#include "drivers/motor.h"
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
#define SPEED 800
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
void turn(int);
void motors();
void straight(int speed);
void mapping_run();
void stop();
void turn_around();
void calibrate_front();
void gradual_stop();
void turn_if_needed();
void step();
void speed_run();
void set_wall(int, int, int);
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

int main(void)
{
    clock_init();
    board_init();
    adc_init();
    motor_init();
    build_labyrinth();
    rgb_set(PINK);
    while (!sw2_read() && !sw1_read())
    {
    }
    if (sw1_read())
    {
        rgb_set(WHITE);
        _delay_ms(500);
        eeprom_read_block(Walls, (uint8_t *)1, ARRAYSIZE);
        rgb_set(YELLOW);
        GOAL_COLUMN = 6;
        GOAL_ROW = 5;
        flood();
        next_square();
        turn_if_needed();
        rgb_set(OFF);
        speed_run();
    }
    else if (sw2_read())
    {
        rgb_set(OFF);
        _delay_ms(500);
        step();
        mapping_run();
        GOAL_COLUMN = 0;
        GOAL_ROW = 0;
        step();
        mapping_run();
        put_walls_to_unvisited();
        rgb_set(WHITE);
        eeprom_update_block(Walls, (uint8_t *)1, ARRAYSIZE);
        rgb_set(OFF);
    }
    return 0;
}

void speed_run()
{
    int count = 0;
    int squares = 1;
    while (!(CurrentRow == GOAL_ROW && CurrentColumn == GOAL_COLUMN))
    {
        _delay_ms(5);
        count++;
        straight(1000);
        if (count % 127 == 0)
        {
            squares += 1;
            set_loc();
            next_square();
            turn_if_needed();
        }
    }
    stop();
}

void mapping_run()
{
    int count = 0;
    int squares = 1;
    int speed = 0;
    while (!sw1_read() && !(CurrentRow == GOAL_ROW && CurrentColumn == GOAL_COLUMN))
    {
        _delay_ms(5);
        count++;
        if (speed < 800)
        {
            speed += 50;
        }
        straight(speed);
        if (count % 160 == 0)
        {
            stop();
            speed = 0;
            if (get_front_left() > 37 && get_front_right() > 32)
            {
                while(!wall())
                {
                    straight(400);
                    _delay_ms(5);
                    rgb_set(RED);
                }
                calibrate_front();
            }
            if (wall())
            {
                rgb_set(RED);
                count = 0;
            }
            else
            {
                rgb_set(WHITE);
            }
            squares += 1;
            set_loc();
            step();
        }
    }
    stop();
}

void straight(int speed)
{
    uint16_t ld = get_left_diag();
    uint16_t rd = get_right_diag();
    bool no_walls = rd < 25 && ld < 25;
    bool left_wall = ld > rd;
    bool right_wall = rd > ld;
    int diag_diff = 0;
    if (no_walls)
    {
        rgb_set(GREEN);
        diag_diff = 0;
        motors(SPEED, SPEED);
    }
    else if (left_wall)
    {
        rgb_set(BLUE);
        diag_diff = (ld - 42)*4;
        motors(speed + diag_diff, speed - diag_diff);
    }
    else if (right_wall)
    {
        rgb_set(YELLOW);
        diag_diff = (38 - rd)*4;
        motors(speed + diag_diff, speed - diag_diff);
    }
}

void step()
{
    read_set_walls();
    flood();
    next_square();
    turn_if_needed();
}

void turn_if_needed()
{
    int next_direction = get_next_direction();
    if (next_direction == LEFT)
    {
        stop();
        _delay_ms(50);
        turn(LEFT);
        _delay_ms(50);
    }
    else if (next_direction == RIGHT)
    {
        stop();
        _delay_ms(50);
        turn(RIGHT);
        _delay_ms(50);
    }
    else if (next_direction == BACKWARD)
    {
        turn_around();
    }
}

void turn(int direction)
{
    if (direction == RIGHT)
    {
        motors(575, -575);
        robot_direction = n_direction() + RIGHT_DIRECTION;
    }
    else if (direction == LEFT)
    {
        motors(-575, 575);
        robot_direction = n_direction() + LEFT_DIRECTION;
    }
    _delay_ms(300);
    stop();
}

void turn_around()
{
    turn(RIGHT);
    calibrate_front();
    turn(RIGHT);
}

void read_set_walls()
{
    // read front wall
    if (get_front_left() > 40 && get_front_right() > 35) add_front_wall_info();
    // read right wall
    if (get_right() > 60) add_right_wall_info();
    // read left wall
    if (get_left() > 65) add_left_wall_info();
    Walls[CurrentRow][CurrentColumn] |= VISITED;
}

int wall()
{
    if (get_front_left() > 90 || get_front_right() > 90)
        return 1;
    else return 0;
}

void calibrate_front()
{
    int fl = get_front_left();
    int fr = get_front_right();
    if (fl < 50 && fr < 50) return;
    int count = 0;
    while (count < 75)
    {
        count++;
        int fd = fl - fr;
        int speed = abs(fd) * 10;
        if (fd > 5)
        {
            motors(-speed, speed);
        }
        else if (fd < -5)
        {
            motors(speed, -speed);
        }
        delay_ms(5);
        fl = get_front_left();
        fr = get_front_right();
    }
    stop();
}

void motors(int16_t l_speed, int16_t r_speed)
{
    // do not allow values bigger than 1000 and smaller than -1000
    if (l_speed < -1000)
    {
        l_speed = -1000;
    }
    else if (l_speed > 1000)
    {
        l_speed = 1000;
    }
    if (r_speed < -1000)
    {
        r_speed = -1000;
    }
    else if (r_speed > 1000)
    {
        r_speed = 1000;
    }
    // correct the slight curving to right
    if (l_speed != 0 && r_speed != 0)
    {
        if (l_speed > 0) l_speed = l_speed - 10;
        else l_speed = l_speed + 10;
    }

    motor_set(l_speed, r_speed);
}

void stop()
{
    motor_set(0, 0);
}

int get_next_direction()
{
    if (NextRow < CurrentRow && NextColumn == CurrentColumn) // go north
    {
        if (n_direction() == N) return FORWARD;
        if (n_direction() == E) return LEFT;
        if (n_direction() == S) return BACKWARD;
        if (n_direction() == W) return RIGHT;
    }
    if (NextRow == CurrentRow && NextColumn > CurrentColumn) // go east
    {
        if (n_direction() == N) return RIGHT;
        if (n_direction() == E) return FORWARD;
        if (n_direction() == S) return LEFT;
        if (n_direction() == W) return BACKWARD;
    }
    if (NextRow > CurrentRow && NextColumn == CurrentColumn) // go south
    {
        if (n_direction() == N) return BACKWARD;
        if (n_direction() == E) return RIGHT;
        if (n_direction() == S) return FORWARD;
        if (n_direction() == W) return LEFT;
    }
    if (NextRow == CurrentRow && NextColumn < CurrentColumn) // go west
    {
        if (n_direction() == N) return LEFT;
        if (n_direction() == E) return BACKWARD;
        if (n_direction() == S) return RIGHT;
        if (n_direction() == W) return FORWARD;
    }
    return 0;
}

void set_loc()
{
    if (n_direction() == N)
    {
        CurrentRow -= 1;
    }
    else if (n_direction() == S)
    {
        CurrentRow += 1;
    }
    else if (n_direction() == E)
    {
        CurrentColumn += 1;
    }
    else if (n_direction() == W)
    {
        CurrentColumn -= 1;
    }
}

void add_front_wall_info()
{
    if (n_direction() == N)
    {
        set_wall(NWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == E)
    {
        set_wall(EWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == S)
    {
        set_wall(SWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == W)
    {
        set_wall(WWall, CurrentRow, CurrentColumn);
    }
}

void add_right_wall_info()
{
    if (n_direction() == N)
    {
        set_wall(EWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == E)
    {
        set_wall(SWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == S)
    {
        set_wall(WWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == W)
    {
        set_wall(NWall, CurrentRow, CurrentColumn);
    }
}

void add_left_wall_info()
{
    if (n_direction() == N)
    {
        set_wall(WWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == E)
    {
        set_wall(NWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == S)
    {
        set_wall(EWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == W)
    {
        set_wall(SWall, CurrentRow, CurrentColumn);
    }
}

void set_wall(int wall, int row, int column)
{
    Walls[row][column] |= wall;
    if (wall == NWall && row - 1 >= 0)
    {
        Walls[row -1][column] |= SWall;
    }
    else if (wall == SWall && row + 1 < ARRAY_LENGTH)
    {
        Walls[row + 1][column] |= NWall;
    }
    else if (wall == EWall && column + 1 < ARRAY_LENGTH)
    {
        Walls[row][column + 1] |= WWall;
    }
    else if (wall == WWall && column - 1 >= 0)
    {
        Walls[row][column - 1] |= EWall;
    }
}

int n_direction()
{
    if (robot_direction == 360)
    {
        return N;
    }
    else if (robot_direction == -180)
    {
        return S;
    }
    else if (robot_direction == 270)
    {
        return W;
    }
    else
    {
        return robot_direction;
    }
}

void delay_ms(uint16_t count)
{
    while (count--)
    {
        _delay_ms(1);
    }
}

uint16_t get_front_left()
{
    return adc_read(FRONT_LEFT_S);
}

uint16_t get_front_right()
{
    return adc_read(FRONT_RIGHT_S);
}

uint16_t get_left_diag()
{
    return adc_read(LEFT_DIAG_S);
}

uint16_t get_right_diag()
{
    return adc_read(RIGHT_DIAG_S);
}

uint16_t get_left()
{
    return adc_read(LEFT_S);
}

uint16_t get_right()
{
    return adc_read(RIGHT_S);
}

