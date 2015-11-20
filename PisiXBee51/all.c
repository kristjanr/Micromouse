#include "all.h"

// debugging methods begin

void straight_sense_only() {
    uint16_t ld = get_left_diag();
    uint16_t rd = get_right_diag();
    bool no_walls = rd < 25 && ld < 25;
    bool left_wall = ld > rd;
    bool right_wall = rd > ld;
    int diag_diff = 0;
    if (no_walls) {
        rgb_set(GREEN);
    }
    else if (left_wall) {
        rgb_set(BLUE);
    }
    else if (right_wall) {
        rgb_set(RED);
    }
}

void debug_sensors() {
    rgb_set(OFF);
    char BUF[30];
    sprintf(BUF, "%d %d %d %d %d %d\n\r",
            get_front_left(),
            get_left_diag(),
            get_left(),
            get_front_right(),
            get_right_diag(),
            get_right()
    );
    _delay_ms(500);
    radio_puts(BUF);
    straight_sense_only();
}

void move_one_square_turn_around_when_wall(int max_speed) {
    LEFTENC = 0;
    RIGHTENC = 0;
    while (LEFTENC + RIGHTENC < 2160) {
        if (speed < max_speed) {
            speed += 25;
        }
        straight(speed);
        if (wall()) {
            rgb_set(RED);
            turn_R();
            turn_R();
            return 0;
        }
        else {
            rgb_set(WHITE);
        }
    }
}
// debugging methods end

void speed_run() {
    int squares = 1;
    while (!(CurrentRow == GOAL_ROW && CurrentColumn == GOAL_COLUMN)) {
        move_one_square(MAX_SPEED);
        squares += 1;
        set_loc();
        next_square();
        turn_if_needed();
    }
    stop();
}

void mapping_run() {
    int squares = 1;
    while (!sw1_read() && !(CurrentRow == GOAL_ROW && CurrentColumn == GOAL_COLUMN)) {
        rgb_set(WHITE);
        move_one_square(SPEED);
        stop();
        squares += 1;
        set_loc();
        step();
    }
    stop();
}

void move_one_square(int max_speed) {
    LEFTENC = 0;
    RIGHTENC = 0;
    speed = 0;
    while (LEFTENC + RIGHTENC < 2160) {
        if (speed < max_speed) {
            speed += 25;
        }
        straight(speed);
        if (wall()) {
            rgb_set(RED);
            // comment in if all else works
            // calibrate_front();
            return 0;
        }
        else {
            rgb_set(WHITE);
        }
    }
}

void straight(int speed) {
    uint16_t ld = get_left_diag();
    uint16_t rd = get_right_diag();
    bool no_walls = rd < 25 && ld < 25;
    bool left_wall = ld > rd;
    bool right_wall = rd > ld;
    // TODO: calc diag diff
    int diag_diff = 0;

    if (no_walls) {
        rgb_set(GREEN);
        motors(speed, speed);
    }
    else if (left_wall) {
        // diag_diff = (ld - 42) * 4;
        rgb_set(BLUE);
        motors(speed + diag_diff, speed - diag_diff);
    }
    else if (right_wall) {
        //diag_diff = (38 - rd) * 4;
        rgb_set(RED);
        motors(speed + diag_diff, speed - diag_diff);
    }
}

void step() {
    read_set_walls();
    flood();
    next_square();
    turn_if_needed();
}

void turn_if_needed() {
    int next_direction = get_next_direction();
    if (next_direction == LEFT) {
        stop();
        _delay_ms(50);
        turn_L();
        _delay_ms(50);
    }
    else if (next_direction == RIGHT) {
        stop();
        _delay_ms(50);
        turn_R();
        _delay_ms(50);
    }
    else if (next_direction == BACKWARD) {
        turn_R();
        turn_R();
    }
}

void turn_R() {
    RIGHTENC = 0;
    while (RIGHTENC > -330) {
        rgb_set(BLUE);
        motor_set(TURN_SPEED, -TURN_SPEED);
    }
    motor_set(0, 0);
}

void turn_L() {
    LEFTENC = 0;
    while (LEFTENC > -330) {
        rgb_set(BLUE);
        motor_set(-TURN_SPEED, TURN_SPEED);
    }
    motor_set(0, 0);
}

void read_set_walls() {
    // read front wall
    if (get_front_left() > 130 && get_front_right() > 130) add_front_wall_info();
    // read right wall
    if (get_right() > 90) add_right_wall_info();
    // read left wall
    if (get_left() > 90) add_left_wall_info();
    Walls[CurrentRow][CurrentColumn] |= VISITED;
}

int wall() {
    if (get_front_left() > 90 || get_front_right() > 90)
        return 1;
    else return 0;
}

void calibrate_front() {
    int fl = get_front_left();
    int fr = get_front_right();
    // TODO: needs changing?
    if (fl < 50 && fr < 50) return;
    int count = 0;
    while (count < 75) {
        count++;
        int fd = (fl - fr) / 10;
        int speed = abs(fd);
        if (fd > 5) {
            motors(-speed, speed);
        }
        else if (fd < -5) {
            motors(speed, -speed);
        }
        delay_ms(5);
        fl = get_front_left();
        fr = get_front_right();
    }
    stop();
}

void motors(int16_t l_speed, int16_t r_speed) {
    // do not allow values bigger than MAX_SPEED and smaller than -MAX_SPEED
    if (l_speed < -MAX_SPEED) {
        l_speed = -MAX_SPEED;
    }
    else if (l_speed > MAX_SPEED) {
        l_speed = MAX_SPEED;
    }
    if (r_speed < -MAX_SPEED) {
        r_speed = -MAX_SPEED;
    }
    else if (r_speed > MAX_SPEED) {
        r_speed = MAX_SPEED;
    }
    // correct the slight curving to right
    if (l_speed != 0 && r_speed != 0) {
        if (l_speed > 0) l_speed = l_speed - 10;
        else l_speed = l_speed + 0;
    }

    motor_set(l_speed, r_speed);
}

void stop() {
    rgb_set(OFF);
    motor_set(0, 0);
}

int get_next_direction() {
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

void set_loc() {
    if (n_direction() == N) {
        CurrentRow -= 1;
    }
    else if (n_direction() == S) {
        CurrentRow += 1;
    }
    else if (n_direction() == E) {
        CurrentColumn += 1;
    }
    else if (n_direction() == W) {
        CurrentColumn -= 1;
    }
}

void add_front_wall_info() {
    if (n_direction() == N) {
        set_wall(NWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == E) {
        set_wall(EWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == S) {
        set_wall(SWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == W) {
        set_wall(WWall, CurrentRow, CurrentColumn);
    }
}

void add_right_wall_info() {
    if (n_direction() == N) {
        set_wall(EWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == E) {
        set_wall(SWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == S) {
        set_wall(WWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == W) {
        set_wall(NWall, CurrentRow, CurrentColumn);
    }
}

void add_left_wall_info() {
    if (n_direction() == N) {
        set_wall(WWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == E) {
        set_wall(NWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == S) {
        set_wall(EWall, CurrentRow, CurrentColumn);
    }
    else if (n_direction() == W) {
        set_wall(SWall, CurrentRow, CurrentColumn);
    }
}

void set_wall(int wall, int row, int column) {
    Walls[row][column] |= wall;
    if (wall == NWall && row - 1 >= 0) {
        Walls[row - 1][column] |= SWall;
    }
    else if (wall == SWall && row + 1 < ARRAY_LENGTH) {
        Walls[row + 1][column] |= NWall;
    }
    else if (wall == EWall && column + 1 < ARRAY_LENGTH) {
        Walls[row][column + 1] |= WWall;
    }
    else if (wall == WWall && column - 1 >= 0) {
        Walls[row][column - 1] |= EWall;
    }
}

int n_direction() {
    if (robot_direction == 360) {
        return N;
    }
    else if (robot_direction == -180) {
        return S;
    }
    else if (robot_direction == 270) {
        return W;
    }
    else {
        return robot_direction;
    }
}

void delay_ms(uint16_t count) {
    while (count--) {
        _delay_ms(1);
    }
}

uint16_t get_front_left() {
    return adc_read(FRONT_LEFT_S);
}

uint16_t get_front_right() {
    return adc_read(FRONT_RIGHT_S);
}

uint16_t get_left_diag() {
    return adc_read(LEFT_DIAG_S);
}

uint16_t get_right_diag() {
    return adc_read(RIGHT_DIAG_S);
}

uint16_t get_left() {
    return adc_read(LEFT_S);
}

uint16_t get_right() {
    return adc_read(RIGHT_S);
}

