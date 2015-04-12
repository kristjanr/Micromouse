/*
 * labyrinth.h
 *
 * Created: 11.04.2015 18:32:49
 *  Author: Kristjan Roosild
 */


#ifndef LABYRINTH_H_
#define LABYRINTH_H_
#include <avr/io.h>

#define ARRAY_LENGTH 12
#define NWall 1
#define EWall 2
#define SWall 4
#define WWall 8
#define GOAL_ROW 5
#define GOAL_COLUMN 5
#define NOT_VISITED 0

int arr[ARRAY_LENGTH][ARRAY_LENGTH];

void build_labyrinth();



#endif /* LABYRINTH_H_ */