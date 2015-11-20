#include "all.h"

int main(void) {
    clock_init();
    board_init();
    adc_init();
    motor_init();
    build_labyrinth();
    rgb_set(PINK);
    quadrature_init();
    radio_init(57600);

    while (!sw2_read() && !sw1_read()) {
    }
    _delay_ms(500);

    // debug sensors
    while (!sw1_read()) {
        debug_sensors();
    }
    _delay_ms(500);

    // turn left
    while (!sw1_read()) {
        turn_L();
        _delay_ms(500);
    }
    _delay_ms(500);

    // turn right
    while (!sw1_read()) {
        turn_R();
        _delay_ms(500);
    }
    _delay_ms(500);

    // turn around
    while (!sw1_read()) {
        turn_R();
        turn_R();
        _delay_ms(500);
    }
    _delay_ms(500);

    // move straight
    while (!sw1_read()) {
        straight(300);
    }
    _delay_ms(500);


    // move square by square
    while (!sw1_read()) {
        move_one_square(SPEED);
        _delay_ms(500);
    }
    _delay_ms(500);


    // move square by square detecting wall
    while (!sw1_read()) {
        move_one_square_turn_around_when_wall(SPEED);
        _delay_ms(500);
    }
    _delay_ms(500);


    if (sw1_read()) {
        rgb_set(WHITE);
        _delay_ms(500);
        eeprom_read_block(Walls, (uint8_t *) 1, ARRAYSIZE);
        rgb_set(YELLOW);
        GOAL_COLUMN = 3;
        GOAL_ROW = 3;
        flood();
        next_square();
        turn_if_needed();
        rgb_set(OFF);
        speed_run();
    }
    else if (sw2_read()) {
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
        eeprom_update_block(Walls, (uint8_t *) 1, ARRAYSIZE);
        rgb_set(OFF);
    }
    return 0;
}
