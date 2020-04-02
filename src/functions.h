#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "structs.h"

minefield* create_field(int rows, int cols, int mine_probability);
void print_field(minefield* field);
void print_cell(cell cell);
int inter_cell(minefield* field, int x, int y, char mode);
int open_cell(minefield* field, int x, int y);
void flag_cell(minefield* field, int x, int y);
void open_zeros(minefield* field, int x, int y);
int open_neighbours(minefield* field, int x, int y);
void open_all(minefield* field);
void open_all_mines(minefield* field);
int check_win(minefield* field);
int get_index(minefield* field, int x, int y);
int neighbour_mines(minefield* field, int x, int y);

#endif