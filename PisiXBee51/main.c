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

#define TURN_TIME 350 // for 90 degrees turn
#define TURN_SPEED 1000 //for 90 degrees turn
#define MOVE_SPEED 900 // not 1000 so could add speed for course correction

#define FRONT_LEFT_S 0
#define LEFT_S 1
#define LEFT_DIAG_S 4

#define FRONT_RIGHT_S 3
#define RIGHT_S  5
#define RIGHT_DIAG_S 2

int get_left_diag();

int get_right_diag();

void set_motor();

void go();

void delay_ms(uint16_t count) {
    while (count--) {
        _delay_ms(1);

    }
}

int get_front_right() {
    return adc_read(FRONT_RIGHT_S);
}

int get_left_right() {
    return adc_read(FRONT_LEFT_S);
}

int get_left() {
    return adc_read(LEFT_S);
}

int get_right() {
    return adc_read(RIGHT_S) - 2;
}

int get_right_diag() {
    // correction. The average difference is that left diag sens shows 2,57 more
    return adc_read(RIGHT_DIAG_S) + 3;
}

int get_left_diag() { return adc_read(LEFT_DIAG_S); }

void send_debug_msg() {
    char buff[30];
    int right_diag = adc_read(RIGHT_DIAG_S);
    int left_diag = adc_read(LEFT_DIAG_S);
    int diff = left_diag - right_diag;
    sprintf(buff, "%4d,%4d,%4d,%4d,%4d \n\r", left_diag, right_diag, diff, get_left(), get_right());
    radio_puts(buff);
}

void turn_right_and_back() {
    motor_left_set(TURN_SPEED);
    delay_ms(TURN_TIME);
    motor_left_set(0);
    delay_ms(TURN_TIME);
    motor_left_set(-TURN_SPEED);
    delay_ms(TURN_TIME);
    motor_left_set(0);

}

void turn_left_and_back() {
    motor_right_set(TURN_SPEED);
    delay_ms(TURN_TIME);
    motor_right_set(0);
    delay_ms(TURN_TIME);
    motor_right_set(-TURN_SPEED);
    delay_ms(TURN_TIME);
    motor_right_set(0);
}

void turn_around(int right) {
    if (right) {
        motor_set(TURN_SPEED, -TURN_SPEED);
    } else {
        motor_set(-TURN_SPEED, TURN_SPEED);
    }
    delay_ms(TURN_TIME);
    motor_set(0, 0);

}

void motors(int16_t l_speed, int16_t r_speed) {
    // correct the slight curving to right
    int16_t left = l_speed - 25;
    motor_set(left, r_speed);
}

void forward() {
    motors(MOVE_SPEED, MOVE_SPEED);
}

void straight(int move_time) {
    int count = move_time;
    int faster = MOVE_SPEED + 100;
    int slower = MOVE_SPEED - 100;
    while (count and get_front_left() < 100 && get_front_right() < 100) {
        int diag_diff = get_left_diag() - get_right_diag();
        if (diag_diff > 10 && get_right() > 35) {
            motors(faster, slower);
        } else if (diag_diff < -10 && get_left() > 35) {
            motors(slower, faster);
        } else {
            forward();
        }
        count -= 10;
        delay_ms(10);
    }
    motor_set(0, 0);
}

int main(void) {
    clock_init();        // Seadista systeemi kell 32MHz peale
    board_init();        // Seadista LED ja nupud
    adc_init();            // Seadista ADC kanal 0
    radio_init(57600);    // Seadista raadiomooduli UART
    motor_init();        // Seadista mootorikontroller
    bool go = true;
    while (go) {
        if (sw2_read()) {
            rgb_set(RED);
            delay_ms(1000);
            while (!sw1_read())
                go();
        }
        else if (sw1_read()) {
            rgb_set(BLUE);
            go = false;
        }
        else {
            rgb_set(GREEN);
            send_debug_msg();
            _delay_ms(500);
        }
    }
}
void drive_left() {
    motors(MOVE_SPEED - 500, MOVE_SPEED);
    _delay_ms(1000);
    forward();

}void drive_right() {
    motors(MOVE_SPEED, MOVE_SPEED - 500);
    _delay_ms(100);
    forward();
}

void go() {
    straight(50);
    if (get_left() < 35) {
        drive_left();
    } else if (get_right() < 35) {
        drive_right();
    }
}
