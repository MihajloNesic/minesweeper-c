#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

typedef struct cell {
	int x;
	int y;
	int is_mine;
	int neig_mines;
	int is_open;
} cell;

typedef struct minefield {
	int rows;
	int cols;
	cell* cells;
} minefield;

#endif