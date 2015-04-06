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
#define SPEED 800
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
bool wall();
void calibrate_front();
void back_to_center();
void gradual_stop();
int turn_seq = 0;
// bool directions[] = {false, false, true, true, true, true, false, true, true, true, true, true, false, false, true};
#define RIGHT 1
#define LEFT 2
#define RANDOM 0


int main(void)
{
    clock_init();        // Seadista systeemi kell 32MHz peale
    board_init();        // Seadista LED ja nupud
    adc_init();            // Seadista ADC kanal 0
    radio_init(57600);    // Seadista raadiomooduli UART
    motor_init();        // Seadista mootorikontroller
    char buff[100];
    rgb_set(PINK);
    while(1)
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
}

void go()
{
    int32_t count = 0;
    char buff[100];
    while(!sw1_read())
    {
        _delay_ms(5);
        count ++;
        if (count % 10 == 0)
        {
            send_debug_msg(buff);
        }
        straight();
        if (wall())
        {
            gradual_stop();
            rgb_set(RED);
            _delay_ms(1000);

            //rgb_set(YELLOW);
            //back_to_center();

            rgb_set(WHITE);
            _delay_ms(1000);
            calibrate_front();

            rgb_set(BLUE);
            _delay_ms(1000);
            turn_around();
            _delay_ms(1000);
        }
    }
    stop();
}

bool wall()
{
    return (get_front_left() + get_front_right()) > 180;
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
    bool both_walls = (ld > 25 && rd > 14);
    bool only_left_wall = !both_walls && rd < 12;
    bool only_right_wall = !both_walls && ld < 12;
    int diag_diff = 0;
    // compensate right diagonal sensor
    rd = rd + 12;
    if(both_walls)
    {
        rgb_set(GREEN);
        diag_diff = ld - rd;
    }
    else if(only_left_wall)
    {
        rgb_set(BLUE);

        diag_diff = ld - 45;
    }
    else if(only_right_wall)
    {
        rgb_set(RED);
        diag_diff = 45 - rd;
    }
    else
    {
        diag_diff=0;
    }
    motors(SPEED + diag_diff*3, SPEED - diag_diff*3);
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
    if (direction != RANDOM)
    {
        turn_seq ++;
        _delay_ms(500);
        if (direction == RIGHT)
        {
            motors(500, 0);
        }
        else if (direction == LEFT)
        {
            motors(0, 500);
        }
        _delay_ms(625);
        motors(500, 500);
        _delay_ms(300);
    }
    else
    {
        turn(RIGHT);
    }
}

void turn_around()
{
    // -500 and 500 for 720ms does 180 degrees
    motor_set(500, -500);
    _delay_ms(700);
    stop();
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
    return adc_read(FRONT_LEFT_S) + 10;
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
