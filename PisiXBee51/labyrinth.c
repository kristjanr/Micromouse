/*
 * labyrinth.c
 *
 * Created: 11.04.2015 18:08:04
 *  Author: Kristjan Roosild
 */
#include "labyrinth.h"
#define CHECKED 16
int highest_neighbouring_square(unsigned int, unsigned int);
int get_neighbour(int, int);
int max(int a[], int num_elements);
void set_next_square();
#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

void build_labyrinth()
{
    for (unsigned int row = 0; row < ARRAY_LENGTH; row++)
    {
        for (unsigned int column = 0; column < ARRAY_LENGTH; column++)
        {
            wall_arr[row][column] = NOT_VISITED;
            if (row == 0)
            {
                wall_arr[row][column] |= NWall;
            }
            if (row == 11)
            {
                wall_arr[row][column] |= SWall;
            }
            if (column == 0)
            {
                wall_arr[row][column] |= WWall;
            }
            if (column == 11)
            {
                wall_arr[row][column] |= EWall;
            }
        }
    }
}

void flood()
{
    unsigned int count = 0;

    // reset distance array
    for (unsigned int row = 0; row < ARRAY_LENGTH; row++)
    {
        for (unsigned int column = 0; column < ARRAY_LENGTH; column++)
        {
            distance_arr[row][column] = -1;
        }
    }
    distance_arr[GOAL_ROW][GOAL_COLUMN] = 0;
    wall_arr[GOAL_ROW][GOAL_COLUMN] = CHECKED;

    while (1)
    {
        count += 1;
        for (unsigned int row = 0; row < ARRAY_LENGTH; row++)
        {
            for (unsigned int column = 0; column < ARRAY_LENGTH; column++)
            {
                if (wall_arr[row][column] & CHECKED)
                {
                    continue;
                }
                if (highest_neighbouring_square(row, column) == (count -1))
                {
                    distance_arr[row][column] = count;
                    wall_arr[row][column] |= CHECKED;
                }
            }
        }
        if (wall_arr[0][0] & CHECKED)
        {
            break;
        }
    }
    set_next_square();
    // reset wall array to not checked
    for (unsigned int i = 0; i < ARRAY_LENGTH; i++) {
        for (unsigned int j = 0; j < ARRAY_LENGTH; j++) {
            wall_arr[i][j] &= ~CHECKED;
        }
    }
}

int highest_neighbouring_square(unsigned int row, unsigned int column)
{
    int north = -1;
    int south = -1;
    int west = -1;
    int east = -1;
    if (!(wall_arr[row][column] & NWall))
    {
        north = get_neighbour(row - 1, column);
    }
    if (!(wall_arr[row][column] & SWall))
    {
        south = get_neighbour(row + 1, column);
    }
    if (!(wall_arr[row][column] & WWall))
    {
        west = get_neighbour(row, column - 1);
    }
    if (!(wall_arr[row][column] & EWall))
    {
        east = get_neighbour(row, column + 1);
    }
    int a[4] = {north, south, east, west};
    return max(a, 4);
}

void set_next_square()
{
    int current_square_value = distance_arr[r_row][r_column];
    int north = -1;
    int south = -1;
    int west = -1;
    int east = -1;
    if (!(wall_arr[r_row][r_column] & NWall))
    {
        north = get_neighbour(r_row - 1, r_column);
    }
    if (!(wall_arr[r_row][r_column] & SWall))
    {
        south = get_neighbour(r_row + 1, r_column);
    }
    if (!(wall_arr[r_row][r_column] & WWall))
    {
        west = get_neighbour(r_row, r_column - 1);
    }
    if (!(wall_arr[r_row][r_column] & EWall))
    {
        east = get_neighbour(r_row, r_column + 1);
    }
    if (north == current_square_value -1)
    {
        n_row = r_row -1;
        n_column = r_column;
    }
    else if (south == current_square_value -1)
    {
        n_row = r_row +1;
        n_column = r_column;
    }
    else if (west == current_square_value -1)
    {
        n_row = r_row;
        n_column = r_column - 1;
    }
    else if (east == current_square_value -1)
    {
        n_row = r_row;
        n_column = r_column + 1;
    }
}

int get_neighbour(int row, int column)
{
    if (row < 0 || column < 0 || row > ARRAY_LENGTH -1 || column > ARRAY_LENGTH -1)
    {
        return -1;
    }
    return distance_arr[row][column];
}

int max(int a[], int num_elements)
{
    int i, max=-32000;
    for (i=0; i<num_elements; i++)
    {
        if (a[i]>max)
        {
            max=a[i];
        }
    }
    return(max);
}