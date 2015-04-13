/*
 * labyrinth.h
 *
 * Created: 11.04.2015 18:32:49
 *  Author: Kristjan Roosild
 */


#ifndef LABYRINTH_H_
#define LABYRINTH_H_

#define ARRAY_LENGTH 7 // 12
#define GOAL_ROW 3 // 5
#define GOAL_COLUMN 2 // 5

#define NWall 1
#define EWall 2
#define SWall 4
#define WWall 8
#define NOT_VISITED 0
extern int r_row;
extern int r_column;
int n_row;
int n_column;

void flood();
int wall_arr[ARRAY_LENGTH][ARRAY_LENGTH];
int distance_arr[ARRAY_LENGTH][ARRAY_LENGTH];

void build_labyrinth();



#endif /* LABYRINTH_H_ */