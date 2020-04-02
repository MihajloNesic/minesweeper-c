#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "functions.h"

#define RESET           "\033[0m"
#define F_GRAY          "\033[1;30m"
#define B_GRAY          "\033[1;30;47m"
#define F_BLUE          "\033[0;34m"
#define F_GREEN         "\033[0;32m"
#define F_RED           "\033[1;31m"
#define F_D_BLUE        "\033[1;34m"
#define F_PINK          "\033[0;35m"
#define F_CYAN          "\033[0;36m"
#define F_L_GRAY        "\033[0;37m"
#define F_YELLOW        "\033[0;33m"
#define B_RED_F_BLACK   "\033[0;30;41m"
#define B_YELL_F_BLACK	"\033[7;49;33m"

/**
 * Initializes the minefield board.
 *
 * @param rows Number of rows
 * @param cols Number of columns
 * @param mine_probability Probability of a cell to be a mine
 * @return Minefield board
 */
minefield* create_field(int rows, int cols, int mine_probability) {
	int i, j;
	srand((unsigned)time(NULL));

	minefield* field = (minefield*)malloc(sizeof(minefield));
	field->rows = rows;
	field->cols = cols;
	field->cells = (cell*)malloc(rows * cols * sizeof(cell));

	// set default values and set mines
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			int index = get_index(field, i, j);
			int is_mine = (rand() % 100) < mine_probability;

			field->cells[index].x = i;
			field->cells[index].y = j;
			field->cells[index].neig_mines = 0;
			field->cells[index].is_mine = is_mine;
			field->cells[index].is_open = 0;
			field->cells[index].is_flag = 0;
		}
	}

	// set number of neighbour mines
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			int index = get_index(field, i, j);
			int neig_mines = neighbour_mines(field, i, j);
			field->cells[index].neig_mines = neig_mines;
		}
	}

	return field;
}

/**
 * Prints the minefield board.
 *
 * @param field Minefield board
 */
void print_field(minefield* field) {
	int i, j;

	// print top header
	for (i = 0; i < field->cols; i++) {
		if (i == 0) {
			printf("y \t  %d ", i);
		}
		else if(i<10) {
			printf(" %d ", i);
		}
		else {
			printf("%d ", i);
		}
	}
	printf("\nx\n\n");

	for (i = 0; i < field->rows; i++) {
		for (j = 0; j < field->cols; j++) {
			// print left header
			if (j == 0) {
				printf("%d\t ", i);
			}

			int index = get_index(field, i, j);
			print_cell(field->cells[index]);
		}
		printf("\n");
	}
}

/**
 * Prints the cell depending on it's value.
 *
 * @param cell The cell to print
 */
void print_cell(cell cell) {
	// if the cell is flagged
	if (cell.is_flag == 1) {
		printf(B_YELL_F_BLACK " F " RESET);
		return;
	}
	if (cell.is_open == 1) {
		// if the cell is a mine
		if (cell.is_mine == 1) {
			printf(B_RED_F_BLACK " O " RESET);
			return;
		}
		// if the cell is a zero
		if (cell.neig_mines == 0) {
			printf(F_GRAY " - " RESET);
			return;
		}

		// get cell number color
		switch (cell.neig_mines) {
			case 1:
				printf(F_BLUE);
				break;
			case 2:
				printf(F_GREEN);
				break;
			case 3:
				printf(F_RED);
				break;
			case 4:
				printf(F_D_BLUE);
				break;
			case 5:
				printf(F_PINK);
				break;
			case 6:
				printf(F_CYAN);
				break;
			case 7:
				printf(F_YELLOW);
				break;
			case 8:
				printf(F_L_GRAY);
				break;
		}

		// if the cell is a number
		printf(" %d " RESET, cell.neig_mines);

		return;
	}

	// if the cell is not opened
	printf(B_GRAY " D " RESET);
}

/**
 * Interact with the cell depending on mode.
 *
 * @param field Minefield board
 * @param x The x position (from 0)
 * @param y The y position (from 0)
 * @param mode Interaction mode; o - open; f - flag
 * @return 1 - if the cell is a mine; 0 - if the cell is not a mine or the cell is flagged
 */
int inter_cell(minefield* field, int x, int y, char mode) {
	if (mode == 'o' || mode == 'O') {
		return open_cell(field, x, y);
	}
	else if (mode == 'f' || mode == 'F') {
		flag_cell(field, x, y);
	}
	return 0;
}

/**
 * Opens the cell at (x, y).
 * If the cell is 0, it will open all neighbour zeros.
 *
 * @param field Minefield board
 * @param x The x position (from 0)
 * @param y The y position (from 0)
 * @return 1 - if the cell is a mine; 0 - if the cell is not a mine
 */
int open_cell(minefield* field, int x, int y) {
	int index = get_index(field, x, y);

	if (field->cells[index].is_flag == 1) {
		return 0;
	}

	// if cell is already open, open all it's neighbours
	if (field->cells[index].is_open == 1) {
		return open_neighbours(field, x, y);
	}

	field->cells[index].is_open = 1;
	// if cell is 0, open neighbour cells
	if (field->cells[index].neig_mines == 0) {
		open_zeros(field, x, y);
	}
	return field->cells[index].is_mine;
}

/**
 * Toggles flag on cell at (x, y).
 * The player cannot toggle flag if the cell is open.
 *
 * @param field Minefield board
 * @param x The x position (from 0)
 * @param y The y position (from 0)
 */
void flag_cell(minefield* field, int x, int y) {
	int index = get_index(field, x, y);

	if (field->cells[index].is_open == 1) {
		return;
	}

	if (field->cells[index].is_flag == 1) {
		field->cells[index].is_flag = 0;
	}
	else {
		field->cells[index].is_flag = 1;
	}
}

/**
 * Opens all neighbour zeros from (x, y).
 * If the cell is not 0, it will open it as well.
 * Recursive.
 *
 * @param field Minefield board
 * @param x The x position (from 0)
 * @param y The y position (from 0)
 */
void open_zeros(minefield* field, int x, int y) {
	int i, j;
	int index = get_index(field, x, y);

	for (i = x - 1; i <= x + 1; i++) {
		for (j = y - 1; j <= y + 1; j++) {
			int neig_index = get_index(field, i, j);
			if (neig_index != -1 && neig_index != index) {
				if (field->cells[neig_index].is_open == 0 && field->cells[neig_index].is_flag == 0) {
					open_cell(field, i, j);
				}
			}
		}
	}
}

/**
 * Opens all neighbour cells from (x, y).
 * If the cell is a flag, it will not open it.
 *
 * @param field Minefield board
 * @param x The x position (from 0)
 * @param y The y position (from 0)
 * @return 1 - if any neighbour cell is a mine
 */
int open_neighbours(minefield* field, int x, int y) {
	int i, j;
	int index = get_index(field, x, y);

	for (i = x - 1; i <= x + 1; i++) {
		for (j = y - 1; j <= y + 1; j++) {
			int neig_index = get_index(field, i, j);
			if (neig_index != -1 && neig_index != index) {
				if (field->cells[neig_index].is_open == 0 && field->cells[neig_index].is_flag == 0) {
					if (field->cells[neig_index].is_mine == 1) {
						return 1;
					}

					field->cells[neig_index].is_open = 1;
					// if cell is 0, open neighbour cells
					if (field->cells[neig_index].neig_mines == 0) {
						open_zeros(field, i, j);
					}
				}
			}
		}
	}
	return 0;
}

/**
 * Opens all cells in the minefield.
 *
 * @param field Minefield board
 */
void open_all(minefield* field) {
	int i, j;
	for (i = 0; i < field->rows; i++) {
		for (j = 0; j < field->cols; j++) {
			int index = get_index(field, i, j);
			field->cells[index].is_flag = 0;
			field->cells[index].is_open = 1;
		}
	}
}

/**
 * Opens all mine cells.
 *
 * @param field Minefield board
 */
void open_all_mines(minefield* field) {
	int i, j;
	for (i = 0; i < field->rows; i++) {
		for (j = 0; j < field->cols; j++) {
			int index = get_index(field, i, j);
			if(field->cells[index].is_mine == 1) {
				field->cells[index].is_flag = 0;
				field->cells[index].is_open = 1;
			}
		}
	}
}

/**
 * Checks if the player won the game.
 *
 * @param field Minefield board
 * @return 1 - if there is no un-opened non-mine cells
 */
int check_win(minefield* field) {
	int i, j;
	for (i = 0; i < field->rows; i++) {
		for (j = 0; j < field->cols; j++) {
			int index = get_index(field, i, j);
			// if there is a non-mine cell that is not open, the player did not win
			if (field->cells[index].is_mine == 0 && field->cells[index].is_open == 0) {
				return 0;
			}
		}
	}
	return 1;
}

/**
 * Returns an index of the cell in the minefield cell array.
 * Returns -1 if position was out of bounds.
 *
 * @param field Minefield board
 * @param x The x position (from 0)
 * @param y The y position (from 0)
 * @return the index in array; -1 - if the position was out of bounds
 */
int get_index(minefield* field, int x, int y) {
	if (x < 0 || x >= field->rows || y < 0 || y >= field->cols) {
		return -1;
	}
	return x * field->cols + y;
}

/**
 * Returns a number of neighbour mines.
 * Return -1 if the cell at position (x, y) is a mine.
 *
 * @param field Minefiled board
 * @param x The x position (from 0)
 * @param y The y position (from 0)
 * @return number of neighbour mines; -1 - if the cell is a mine
 */
int neighbour_mines(minefield* field, int x, int y) {
	int i, j, mines = 0;
	int index = get_index(field, x, y);

	if (field->cells[index].is_mine == 1) {
		return -1;
	}

	for (i = x - 1; i <= x + 1; i++) {
		for (j = y - 1; j <= y + 1; j++) {
			int neig_index = get_index(field, i, j);
			if (neig_index != -1 && neig_index != index) {
				if (field->cells[neig_index].is_mine == 1) {
					mines++;
				}
			}
		}
	}

	return mines;
}