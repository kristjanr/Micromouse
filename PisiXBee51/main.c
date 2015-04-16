/*
* main.c
*
* Created: 04.04.2015 12:00:00
*  Author: Kristjan Roosild
*/

#include "labyrinth.h"

#include <avr/io.h>
#include <stdlib.h>
#include "drivers/board.h"
#include "drivers/adc.h"
#include "drivers/motor.h"
#include "drivers/com.h"

int n_direction();
#define LEFT_DIRECTION -90
#define RIGHT_DIRECTION 90
#define N 0
#define E 90
#define S 180
#define W -90
int robot_direction = E;
void print_direction();
int CurrentColumn = 0;
int CurrentRow = 0;
void set_loc();
void read_set_walls();
void add_front_wall_info();
void add_left_wall_info();
void add_right_wall_info();

#define FRONT_LEFT_S 0
#define LEFT_S 1
#define LEFT_DIAG_S 4

#define FRONT_RIGHT_S 3
#define RIGHT_S  5
#define RIGHT_DIAG_S 2

#define SPEED 800

void delay_ms();

uint16_t get_left();
uint16_t get_left_diag();
uint16_t get_front_left();
uint16_t get_right();
uint16_t get_right_diag();
uint16_t get_front_right();
uint16_t one_square_delay();
void forward();

#define RIGHT 1
#define LEFT 2
#define FORWARD 0
#define BACKWARD -1

void turn(int);

void motors();
void straight();
void mapping_run();
void stop();
void turn_around();
bool wall();
void calibrate_front();
void back_to_center();
void gradual_stop();

int turns = 0;
#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))
void turn_if_needed();

char buff[100];
void print_wall_labyrinth();
void print_wall_labyrinth();

void print_distance_labyrinth();
void send_debug_msg();
void step();
int get_next_direction();

int main(void)
{
    clock_init();        // Seadista systeemi kell 32MHz peale
    board_init();        // Seadista LED ja nupud
    adc_init();            // Seadista ADC kanal 0
    radio_init(57600);    // Seadista raadiomooduli UART
    motor_init();        // Seadista mootorikontroller
    build_labyrinth();
    rgb_set(PINK);
    while(1)
    {
        if(sw2_read())
        {
            rgb_set(OFF);
            _delay_ms(1000);
            mapping_run();
        }
        _delay_ms(500);
        send_debug_msg(buff);
    }
    rgb_set(PINK);

    return 0;
}

void mapping_run()
{
    int count = 0;
    int squares = 1;
    step();
    while(!sw1_read() || (CurrentRow == GOAL_ROW && CurrentColumn == GOAL_COLUMN))
    {
        _delay_ms(5);
        count ++;
        // with this debug message sending, the one square delay is 130. Without: 140
        //if (count % 10 == 0)
        //{
        //send_debug_msg(buff);
        //}
        straight();
        if(count % one_square_delay() == 0 && get_front_left() < 35 && get_front_right() < 35)
        {
            gradual_stop();
            rgb_set(WHITE);
            squares += 1;
            sprintf(buff, "sq: %d \n\r", squares);
            radio_puts(buff);
            set_loc();
            step();
        }
        if (wall())
        {
            rgb_set(RED);
            gradual_stop();
            sprintf(buff, "wall! count: %d \n\r", count);
            radio_puts(buff);
            count = 0;
            squares += 1;
            sprintf(buff, "sq: %d \n\r", squares);
            radio_puts(buff);
            set_loc();
            step();
        }
    }
    stop();
}

void step()
{
    read_set_walls();
    _delay_ms(2000);
    print_wall_labyrinth();
    _delay_ms(2000);
    flood();
    _delay_ms(2000);
    print_distance_labyrinth();
    turn_if_needed();
    _delay_ms(2000);
    print_direction();
}

int get_next_direction()
{
    if(NextRow < CurrentRow && NextColumn == CurrentColumn) // go north
    {
        if (n_direction() == N) return FORWARD;
        if (n_direction() == E) return LEFT;
        if (n_direction() == S) return BACKWARD;
        if (n_direction() == W) return RIGHT;
    }
    if(NextRow == CurrentRow && NextColumn > CurrentColumn) // go east
    {
        if (n_direction() == N) return RIGHT;
        if (n_direction() == E) return FORWARD;
        if (n_direction() == S) return LEFT;
        if (n_direction() == W) return BACKWARD;
    }
    if(NextRow > CurrentRow && NextColumn == CurrentColumn) // go south
    {
        if (n_direction() == N) return BACKWARD;
        if (n_direction() == E) return RIGHT;
        if (n_direction() == S) return FORWARD;
        if (n_direction() == W) return LEFT;
    }
    if(NextRow == CurrentRow && NextColumn < CurrentColumn) // go west
    {
        if (n_direction() == N) return LEFT;
        if (n_direction() == E) return BACKWARD;
        if (n_direction() == S) return RIGHT;
        if (n_direction() == W) return FORWARD;
    }

}

void set_loc()
{
    if(n_direction() == N)
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
    sprintf(buff, "Row: %d, Column: %d \n\r", CurrentRow, CurrentColumn);
    radio_puts(buff);
}

void read_set_walls()
{
    // read front wall
    if (get_front_left() > 80 && get_front_right() > 80) add_front_wall_info();
    // read right wall
    if (get_right() > 80) add_right_wall_info();
    // read left wall
    if (get_left() > 80) add_left_wall_info();
    Walls[CurrentRow][CurrentColumn] |= Visited;
}

void add_front_wall_info()
{
    if (n_direction() == N)
    {
        Walls[CurrentRow][CurrentColumn] |= NWall;
    }
    else if (n_direction() == E)
    {
        Walls[CurrentRow][CurrentColumn] |= EWall;
    }
    else if (n_direction() == S)
    {
        Walls[CurrentRow][CurrentColumn] |= SWall;
    }
    else if (n_direction() == W)
    {
        Walls[CurrentRow][CurrentColumn] |= WWall;
    }
}

void add_right_wall_info()
{
    if (n_direction() == N)
    {
        Walls[CurrentRow][CurrentColumn] |= EWall;
    }
    else if (n_direction() == E)
    {
        Walls[CurrentRow][CurrentColumn] |= SWall;
    }
    else if (n_direction() == S)
    {
        Walls[CurrentRow][CurrentColumn] |= WWall;
    }
    else if (n_direction() == W)
    {
        Walls[CurrentRow][CurrentColumn] |= NWall;
    }
}

void add_left_wall_info()
{
    if (n_direction() == N)
    {
        Walls[CurrentRow][CurrentColumn] |= WWall;
    }
    else if (n_direction() == E)
    {
        Walls[CurrentRow][CurrentColumn] |= NWall;
    }
    else if (n_direction() == S)
    {
        Walls[CurrentRow][CurrentColumn] |= EWall;
    }
    else if (n_direction() == W)
    {
        Walls[CurrentRow][CurrentColumn] |= SWall;
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


void turn_if_needed()
{
    int next_direction = get_next_direction();
    if (next_direction == LEFT)
    {
        turn(LEFT);
    }
    else if (next_direction == RIGHT)
    {
        turn(RIGHT);
    }
    else if (next_direction == BACKWARD)
    {
        turn_around();
    }
}

uint16_t one_square_delay()
{
    return 125;
}

bool wall()
{
    return get_front_left()  > 75 || get_front_right() > 75;
}

void calibrate_front()
{
    int count = 0;
    while(count < 100)
    {
        count ++;
        int fl = get_front_left();
        int fr = get_front_right();
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
        delay_ms(10);
    }
    stop();
}

void straight()
{
    uint16_t ld = get_left_diag();
    uint16_t rd = get_right_diag();
    bool no_walls = rd < 25 && ld < 25;
    bool left_wall = ld > rd;
    bool right_wall = rd > ld;
    int diag_diff = 0;
    if(no_walls)
    {
        rgb_set(GREEN);
        diag_diff=0;
    }
    else if (left_wall)
    {
        rgb_set(BLUE);

        diag_diff = ld - 42;
    }
    else if(right_wall)
    {
        rgb_set(RED);
        diag_diff = 42 - rd;
    }
    motors(SPEED + diag_diff*4, SPEED - diag_diff*4);
}

void motors(int16_t l_speed, int16_t r_speed)
{
    // do not allow values bigger than 1000 and smaller than -1000
    if (l_speed < - 1000)
    {
        l_speed = -1000;
    }
    else if (l_speed > 1000)
    {
        l_speed = 1000;
    }
    if (r_speed < -1000)
    {
        r_speed = - 1000;
    }
    else if (r_speed > 1000)
    {
        r_speed = 1000;
    }
    // correct the slight curving to right
    if (l_speed != 0 && r_speed != 0)
    {
        if (l_speed > 0) l_speed = l_speed - 12;
        else l_speed = l_speed + 12;
    }

    motor_set(l_speed, r_speed);
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
    print_direction();
    motor_set(0,0);
    _delay_ms(50);
}

void turn_around()
{
    // -500 and 500 for 720ms does 180 degrees
    motor_set(500, -500);
    _delay_ms(700);
    stop();
    robot_direction = n_direction() + 180;
    print_direction();
    _delay_ms(50);
}

void stop()
{
    motor_set(0, 0);
}

void gradual_stop()
{
    int speed = SPEED;
    int to_subtract = round(SPEED / 50);
    while(speed > 0)
    {
        _delay_ms(5);
        speed -= to_subtract;
        motors(speed, speed);
    }
}
void send_debug_msg(char * buff)
{
    uint16_t fl = get_front_left();
    uint16_t fr = get_front_right();
    int fd = fl - fr;
    uint16_t l = get_left();
    uint16_t r = get_right();
    int sd = l - r;
    uint16_t dl = get_left_diag();
    uint16_t dr = get_right_diag();
    int dd = dl - dr;
    sprintf(
        buff, "%3u,%3u,%4d, %3u,%3u,%4d, %3u,%3u,%4d \n\r",
        fl, fr, fd, dl, dr, dd, l, r, sd);
    radio_puts(buff);
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

void print_direction()
{
    sprintf(buff, "direction %d \n\r", n_direction());
    radio_puts(buff);
}

void print_wall_labyrinth()
{
    sprintf(buff, "\n\r Printing wall labyrinth: \n\r");
    radio_puts(buff);
    for (int row = 0; row < ARRAY_LENGTH; row++)
    {
        for (int column = 0; column < ARRAY_LENGTH; column++)
        {
            sprintf(buff, "%d ", Walls[row][column]);
            radio_puts(buff);
        }
        sprintf(buff, "\n\r");
        radio_puts(buff);
    }
}

void print_distance_labyrinth()
{
    sprintf(buff, "\n\r Printing distance labyrinth: \n\r");
    radio_puts(buff);
    for (int row = 0; row < ARRAY_LENGTH; row++)
    {
        for (int column = 0; column < ARRAY_LENGTH; column++)
        {
            sprintf(buff, "%d ", Distances[row][column]);
            radio_puts(buff);
        }
        sprintf(buff, "\n\r");
        radio_puts(buff);
    }
}

