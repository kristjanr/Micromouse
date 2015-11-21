#include "all.h"
#include <avr/eeprom.h>


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
    quadrature_init();
    radio_init(57600);

    while (!sw2_read() && !sw1_read())
    {
    }
    _delay_ms(500);

    // debug sensors
    while (!sw1_read())
    {
        debug_sensors();
    }
    _delay_ms(500);

    // turn left
    while (!sw1_read())
    {
        turn_L();
        _delay_ms(500);
    }
    _delay_ms(500);

    // turn right
    while (!sw1_read())
    {
        turn_R();
        _delay_ms(500);
    }
    _delay_ms(500);

    // turn around
    while (!sw1_read())
    {
        turn_R();
        turn_R();
        _delay_ms(500);
    }
    _delay_ms(500);

    // move straight
    while (!sw1_read())
    {
        straight(300);
    }
    _delay_ms(500);


    // move square by square
    while (!sw1_read())
    {
        move_one_square(SPEED);
        _delay_ms(500);
    }
    _delay_ms(500);


    // move square by square detecting wall
    while (!sw1_read())
    {
        move_one_square_turn_around_when_wall(SPEED);
        _delay_ms(500);
    }
    _delay_ms(500);

    while (!sw2_read() && !sw1_read())
    {
    }
    // start mapping run
    if (sw1_read())
    {
        // corner to centre
        rgb_set(OFF);
        _delay_ms(500);
        step();
        mapping_run();

        // back from centre to corner
        GOAL_COLUMN = 0;
        GOAL_ROW = 0;
        step();
        mapping_run();

        // do stuff with labyrinth
        put_walls_to_unvisited();
        rgb_set(YELLOW);
        flood();
        rgb_set(WHITE);
        eeprom_update_block(Walls, (uint8_t *) 1, ARRAYSIZE);
        eeprom_update_block(Distances, (uint8_t *) ARRAYSIZE + 1, ARRAYSIZE);
        rgb_set(OFF);
    }
    // start speed run
    else if (sw2_read())
    {
        rgb_set(WHITE);
        _delay_ms(500);

        // corner to centre
        eeprom_read_block(Walls, (uint8_t *) 1, ARRAYSIZE);
        eeprom_read_block(Distances, (uint8_t *) ARRAYSIZE + 1, ARRAYSIZE);
        rgb_set(YELLOW);
        next_square();
        turn_if_needed();
        rgb_set(OFF);
        speed_run();
    }
    return 0;
}
