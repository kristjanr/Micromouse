/*
* main.c
*
* Created: 04.04.2015 12:00:00
*  Author: Kristjan Roosild
*/

#include <avr/io.h>
#include <stdlib.h>
#include "drivers/board.h"
#include "drivers/adc.h"
#include "drivers/motor.h"
#include "drivers/com.h"
#include "drivers/gyro.h"
#include "labyrinth.h"
#define FRONT_LEFT_S 0
#define LEFT_S 1
#define LEFT_DIAG_S 4

#define FRONT_RIGHT_S 3
#define RIGHT_S  5
#define RIGHT_DIAG_S 2
#define SPEED 800
void delay_ms();
void send_debug_msg();
uint16_t get_left();
uint16_t get_left_diag();
uint16_t get_front_left();
uint16_t get_right();
uint16_t get_right_diag();
uint16_t get_front_right();
uint16_t one_square_delay();
void forward();
void turn(int);
void motors();
void straight();
void go();
void stop();
void turn_around();
bool wall();
void calibrate_front();
void back_to_center();
void gradual_stop();
void put_n_squares(int);
#define N 0
#define E 90
#define S 180
#define W -90
int normalize_direction(int);
#define LEFT_DIRECTION -90
#define RIGHT_DIRECTION 90
#define RIGHT 1
#define LEFT 2
#define RANDOM 0
#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))
int turns_array[] = {RIGHT, 0, 0, 0, 0, LEFT, RIGHT, LEFT, 0, LEFT, 0, RIGHT, LEFT, LEFT, RIGHT, LEFT, LEFT, RIGHT, 0, 0};
int turns = 0;
bool turn_if_needed();
int robot_direction = N;
void send_direction();
void print_labyrinth();
char buff[100];
int column = 0;
int row = 0;
void set_loc();
void read_set_walls();
void add_front_wall_info();
void add_left_wall_info();
void add_right_wall_info();

int main(void)
{
    clock_init();        // Seadista systeemi kell 32MHz peale
    board_init();        // Seadista LED ja nupud
    adc_init();            // Seadista ADC kanal 0
    radio_init(57600);    // Seadista raadiomooduli UART
    motor_init();        // Seadista mootorikontroller
    build_labyrinth();
    rgb_set(PINK);
    /* while(1)
    {
        if(sw2_read())
        {
            rgb_set(OFF);
            _delay_ms(1000);
            go();
        }
        _delay_ms(500);
        send_debug_msg(buff);
    }
    rgb_set(PINK);
    */
    print_labyrinth();
    _delay_ms(100);
    read_set_walls();
    _delay_ms(100);
    print_labyrinth();
    return 0;
}

void print_labyrinth()
{
    sprintf(buff, "\n\r Printing labyrinth: \n\r");
    radio_puts(buff);
    for (int column = 0; column < ARRAY_LENGTH; ++column)
    {
        for (int row = 0; row < ARRAY_LENGTH; ++row)
        {
            sprintf(buff, "%d ", arr[column][row]);
            radio_puts(buff);
        }
        sprintf(buff, "\n\r");
        radio_puts(buff);
    }
}

void set_loc()
{
    if(robot_direction == N)
    {
        column += 1;
    }
    else if (robot_direction == S)
    {
        column -= 1;
    }
    else if (robot_direction == E)
    {
        row += 1;
    }
    else if (robot_direction == W)
    {
        row -= 1;
    }
}

void read_set_walls()
{
    // read front wall
    if (get_front_left() > 80 && get_front_right() > 80) add_front_wall_info();
    // read right wall
    if (get_right() > 80) add_right_wall_info();
    // read left wall
    if (get_left() > 80) add_left_wall_info();
}

void add_front_wall_info()
{
    if (robot_direction == N)
    {
        arr[row][column] += 1;
    }
    else if (robot_direction == E)
    {
        arr[row][column] += 2;
    }
    else if (robot_direction == S)
    {
        arr[row][column] += 4;
    }
    else if (robot_direction == W)
    {
        arr[row][column] += 8;
    }
}

void add_right_wall_info()
{
    if (robot_direction == N)
    {
        arr[row][column] += 2;
    }
    else if (robot_direction == E)
    {
        arr[row][column] += 4;
    }
    else if (robot_direction == S)
    {
        arr[row][column] += 8;
    }
    else if (robot_direction == W)
    {
        arr[row][column] += 1;
    }
}

void add_left_wall_info()
{
    if (robot_direction == N)
    {
        arr[row][column] += 8;
    }
    else if (robot_direction == E)
    {
        arr[row][column] += 1;
    }
    else if (robot_direction == S)
    {
        arr[row][column] += 2;
    }
    else if (robot_direction == W)
    {
        arr[row][column] += 4;
    }
}

void go()
{
    int32_t count = 0;
    int squares = 1;
    while(!sw1_read() && turns < NELEMS(turns_array)) //turns < NELEMS(turns_array)
    {
        _delay_ms(5);
        count ++;
        // with this debug message sending, the one square delay is 130. Without:
        //if (count % 10 == 0)
        //{
        //send_debug_msg(buff);
        //}
        straight();
        if(count % one_square_delay() == 0)
        {
            squares += 1;
            sprintf(buff, "sq: %d \n\r", squares);
            radio_puts(buff);
            gradual_stop();
            print_labyrinth();
            set_loc();
            read_set_walls();
        }
        if (wall())
        {
            gradual_stop();
            if(count % one_square_delay() > one_square_delay() - 40)
            {
                count = 0;
                squares += 1;
                sprintf(buff, "wall! sq: %d \n\r", squares);
                radio_puts(buff);
            }
            //calibrate_front();
            if (!turn_if_needed())
            {
                rgb_set(RED);
                turn_around();
            }
        }
    }
    stop();
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

void send_direction()
{
    sprintf(buff, "direction %d \n\r", n_direction());
    radio_puts(buff);
}

bool turn_if_needed()
{
    sprintf(buff, "turns before: %d \n\r", turns);
    radio_puts(buff);
    if (get_left() < 80 && turns_array[turns] == LEFT)
    {
        turn(LEFT);
        turns += 1;
        return true;
    }
    else if (get_right() < 80 && turns_array[turns] == RIGHT)
    {
        turn(RIGHT);
        turns += 1;
        return true;
    }
    else if (get_left() < 80 || get_right() < 80)
    {
        turns += 1;
        return false;
    }
    else
    {
        return false;
    }
}

uint16_t one_square_delay()
{
    return 140;
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
    send_direction();
    motor_set(0,0);
    _delay_ms(50);
}

void turn_around()
{
    // -500 and 500 for 720ms does 180 degrees
    motor_set(500, -500);
    _delay_ms(700);
    robot_direction = n_direction() + 180;
    send_direction();
    stop();
    _delay_ms(10);
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
