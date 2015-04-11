/*
 * labyrinth.c
 *
 * Created: 11.04.2015 18:08:04
 *  Author: Kristjan Roosild
 */
#include "labyrinth.h"
#define NOT_VISITED 'n'
#define INTERSECTION '.'
#define WALL ' '
int array_length = 23;

void *build_labyrinth()
{
    char buff[100];
    for (int row = 0; row < array_length; row++)
    {
        for (int column = 0; column < array_length; column++)
        {
            if (column % 2 == 0 &&  row % 2 ==0)
            {
                arr[column][row] = NOT_VISITED;
            }
            else if (column % 2 == 1 && row % 2 == 1)
            {
                arr[column][row] = INTERSECTION;
            }
            else
            {
                arr[column][row] = WALL;
            }
        }
    }
}