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
        {   arr[row][column] = NOT_VISITED;
            if (row == 0) {
                arr[row][column] += WWall;
            }
            if (row == 11) {
                arr[row][column] += EWall;
            }
            if (column == 0) {
                arr[row][column] += SWall;
            }
            if (column == 11) {
                arr[row][column] += NWall;
            }
        }
    }
}