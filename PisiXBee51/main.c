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
    rgb_set(BLUE);
    while(1)
    {
        if(sw2_read())
        {
            rgb_set(RED);
            _delay_ms(1000);
            go();
        }
        _delay_ms(500);
        send_debug_msg(buff);
    }
}

bool wall()
{
    return get_front_left() > 180 && get_front_right() > 180;
}

void calibrate_front()
{
    motors(0, 0);
    _delay_ms(5);
    int count = 0;
    while(count < 50)
    {
        count += 1;
        uint16_t fl = get_front_left();
        uint16_t fr = get_front_right();
        int fd = fl - fr;
        int time = abs(fd);
        int speed = 400;
        if (fd > 5)
        {
            motors(-speed, speed);
        }
        else if (fd < -5)
        {
            motors(speed, -speed);
        }
        delay_ms(time);
    }
    motors(0, 0);
}

void go()
{
    while(!sw1_read())
    {
        calibrate_front();
        _delay_ms(1000);

        //int side_diff = abs(get_left() - get_right());
        //if (wall())
        //{
        //turn_around();
        //}
        //else if (side_diff > 35 && get_left() < 40)
        //{
        //turn(2);
        //}
        //else if (side_diff > 35 && get_right() < 40)
        //{
        //turn(1);
        //}
        //else if (side_diff <= 35 && get_right() < 40 && get_left() < 40 && !wall())
        //{
        //turn(0);
        //}
        //else
        //{
        //straight();
        //_delay_ms(5);
        //}
    }
    rgb_set(BLUE);
    stop();
    _delay_ms(300);
    rgb_set(GREEN);
}

void straight()
{
    int diag_diff = get_left_diag() - get_right_diag();
    motors(400 + diag_diff, 400 - diag_diff);
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
        int16_t left = l_speed - 12;
        motor_set(left, r_speed);
    }
    else
    {
        motor_set(l_speed, r_speed);
    }
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
    motor_set(500, -500);
    _delay_ms(675);
}

void stop()
{
    motor_set(0, 0);
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
    return adc_read(FRONT_RIGHT_S) + 4;
}

uint16_t get_left_diag()
{
    return adc_read(LEFT_DIAG_S);
}

uint16_t get_right_diag()
{
    return adc_read(RIGHT_DIAG_S) + 3;
}

uint16_t get_left()
{
    return adc_read(LEFT_S) + 2;
}

uint16_t get_right()
{
    return adc_read(RIGHT_S);
}
