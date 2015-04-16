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
            Walls[row][column] = NOT_VISITED;
            if (row == 0)
            {
                Walls[row][column] |= NWall;
            }
            if (row == ARRAY_LENGTH - 1)
            {
                Walls[row][column] |= SWall;
            }
            if (column == 0)
            {
                Walls[row][column] |= WWall;
            }
            if (column == ARRAY_LENGTH - 1)
            {
                Walls[row][column] |= EWall;
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
            Distances[row][column] = -1;
        }
    }
    Distances[GOAL_ROW][GOAL_COLUMN] = 0;
    Walls[GOAL_ROW][GOAL_COLUMN] = CHECKED;
    int all_checked = 0;
    while (!all_checked)
    {
        all_checked = 1;
        count += 1;
        for (unsigned int row = 0; row < ARRAY_LENGTH; row++)
        {
            for (unsigned int column = 0; column < ARRAY_LENGTH; column++)
            {
                if (Walls[row][column] & CHECKED)
                {
                    continue;
                }
                else
                {
                    all_checked  = 0;
                }
                if (highest_neighbouring_square(row, column) == (count -1))
                {
                    Distances[row][column] = count;
                    Walls[row][column] |= CHECKED;
                }
            }
        }

        /*
        if (Walls[0][0] & CHECKED && Walls[ARRAY_LENGTH-1][0] & CHECKED && Walls[0][ARRAY_LENGTH-1] & CHECKED && Walls[ARRAY_LENGTH-1][ARRAY_LENGTH-1] & CHECKED)
        {
            break;
        }
        */
    }
    set_next_square();
    // reset wall array to not checked
    for (unsigned int i = 0; i < ARRAY_LENGTH; i++)
    {
        for (unsigned int j = 0; j < ARRAY_LENGTH; j++)
        {
            Walls[i][j] &= ~CHECKED;
        }
    }
}

int highest_neighbouring_square(unsigned int row, unsigned int column)
{
    int north = -1;
    int south = -1;
    int west = -1;
    int east = -1;
    if (!(Walls[row][column] & NWall))
    {
        north = get_neighbour(row - 1, column);
    }
    if (!(Walls[row][column] & SWall))
    {
        south = get_neighbour(row + 1, column);
    }
    if (!(Walls[row][column] & WWall))
    {
        west = get_neighbour(row, column - 1);
    }
    if (!(Walls[row][column] & EWall))
    {
        east = get_neighbour(row, column + 1);
    }
    int a[4] = {north, south, east, west};
    return max(a, 4);
}

void set_next_square()
{
    int current_square_value = Distances[CurrentRow][CurrentColumn];
    int north = -1;
    int south = -1;
    int west = -1;
    int east = -1;
    if (!(Walls[CurrentRow][CurrentColumn] & NWall))
    {
        north = get_neighbour(CurrentRow - 1, CurrentColumn);
    }
    if (!(Walls[CurrentRow][CurrentColumn] & SWall))
    {
        south = get_neighbour(CurrentRow + 1, CurrentColumn);
    }
    if (!(Walls[CurrentRow][CurrentColumn] & WWall))
    {
        west = get_neighbour(CurrentRow, CurrentColumn - 1);
    }
    if (!(Walls[CurrentRow][CurrentColumn] & EWall))
    {
        east = get_neighbour(CurrentRow, CurrentColumn + 1);
    }

    if (north == current_square_value -1)
    {
        NextRow = CurrentRow -1;
        NextColumn = CurrentColumn;
    }

    if (south == current_square_value -1)
    {
        NextRow = CurrentRow +1;
        NextColumn = CurrentColumn;
    }

    if (west == current_square_value -1)
    {
        NextRow = CurrentRow;
        NextColumn = CurrentColumn - 1;
    }
    if (east == current_square_value -1)
    {
        NextRow = CurrentRow;
        NextColumn = CurrentColumn + 1;
    }
}

int get_neighbour(int row, int column)
{
    if (row < 0 || column < 0 || row > ARRAY_LENGTH -1 || column > ARRAY_LENGTH -1)
    {
        return -1;
    }
    return Distances[row][column];
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