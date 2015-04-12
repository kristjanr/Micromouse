/*
 * labyrinth.c
 *
 * Created: 11.04.2015 18:08:04
 *  Author: Kristjan Roosild
 */
#include "labyrinth.h"
#define CHECKED 16
int highest_neighbouring_square(uint8_t, uint8_t);
int get_neighbour(uint8_t, uint8_t);
int max(int a[], int num_elements);

void build_labyrinth()
{
    for (uint8_t row = 0; row < ARRAY_LENGTH; row++)
    {
        for (uint8_t column = 0; column < ARRAY_LENGTH; column++)
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
    for (uint8_t row = 0; row < ARRAY_LENGTH; row++)
    {
        for (uint8_t column = 0; column < ARRAY_LENGTH; column++)
        {
            distance_arr[row][column] = -1;
        }
    }
    distance_arr[5][5] = 0;
}

void flood()
{
    uint16_t count = 0;
    while (1)
    {
        count += 1;
        for (uint8_t row = 0; row < ARRAY_LENGTH; row++)
        {
            for (uint8_t column = 0; column < ARRAY_LENGTH; column++)
            {
                if (wall_arr[row][column] & CHECKED)
                {
                    continue;
                }
                if (highest_neighbouring_square(row, column) == count -1)
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
}

int highest_neighbouring_square(uint8_t row, uint8_t column)
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
        north = get_neighbour(row + 1, column);
    }
    if (!(wall_arr[row][column] & WWall))
    {
        north = get_neighbour(row, column - 1);
    }
    if (!(wall_arr[row][column] & EWall))
    {
        north = get_neighbour(row, column + 1);
    }
    int a[4] = {north, south, east, west};
    return max(a, 4);
}

int get_neighbour(uint8_t row, uint8_t column)
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
