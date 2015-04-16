/*
 * labyrinth.h
 *
 * Created: 11.04.2015 18:32:49
 *  Author: Kristjan Roosild
 */


#ifndef LABYRINTH_H_
#define LABYRINTH_H_

#define ARRAY_LENGTH 7 // 12
extern int GOAL_ROW; // 5
extern int GOAL_COLUMN; // 5

#define NWall 1
#define EWall 2
#define SWall 4
#define WWall 8
#define Visited 32
#define NOT_VISITED 0
extern int CurrentRow;
extern int CurrentColumn;
int NextRow;
int NextColumn;

void flood();
int Walls[ARRAY_LENGTH][ARRAY_LENGTH];
int Distances[ARRAY_LENGTH][ARRAY_LENGTH];

void build_labyrinth();



#endif /* LABYRINTH_H_ */