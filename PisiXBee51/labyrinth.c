/*
 * labyrinth.c
 *
 * Created: 11.04.2015 18:08:04
 *  Author: Kristjan Roosild
 */
#include "labyrinth.h"
#define NOT_VISITED 0

void build_labyrinth()
{
    for (int row = 0; row < ARRAY_LENGTH; row++)
    {
        for (int column = 0; column < ARRAY_LENGTH; column++)
        {
            arr[column][row] = NOT_VISITED;
        }
    }
}