#ifndef LABYRINTH_H_
#define LABYRINTH_H_

#define ARRAY_LENGTH 12 // 12
#define NWall 1
#define EWall 2
#define SWall 4
#define WWall 8
#define VISITED 32
#define NOT_VISITED 0

extern int GOAL_ROW;
extern int GOAL_COLUMN;
extern int CurrentRow;
extern int CurrentColumn;
extern void set_wall(int, int, int);

void next_square();
void build_labyrinth();
void flood();
void put_walls_to_unvisited();
int Walls[ARRAY_LENGTH][ARRAY_LENGTH];
int Distances[ARRAY_LENGTH][ARRAY_LENGTH];
int NextRow;
int NextColumn;

#endif /* LABYRINTH_H_ */