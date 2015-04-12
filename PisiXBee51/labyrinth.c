/*
 * labyrinth.c
 *
 * Created: 11.04.2015 18:08:04
 *  Author: Kristjan Roosild
 */
#include "labyrinth.h"


void build_labyrinth()
{
    for (uint8_t row = 0; row < ARRAY_LENGTH; row++)
    {
        for (uint8_t column = 0; column < ARRAY_LENGTH; column++)
        {
            arr[row][column] = NOT_VISITED;
            if (row == 0)
            {
                arr[row][column] |= NWall;
            }
            if (row == 11)
            {
                arr[row][column] |= SWall;
            }
            if (column == 0)
            {
                arr[row][column] |= WWall;
            }
            if (column == 11)
            {
                arr[row][column] |= EWall;
            }
        }
    }
}

void flood()
{

}