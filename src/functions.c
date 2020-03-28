#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include "functions.h"

#define RESET 15
#define F_GRAY 8
#define B_GRAY 120
#define F_BLUE 9
#define F_GREEN 10
#define F_RED 12
#define F_D_BLUE 3
#define F_PINK 13
#define F_CYAN 11
#define F_L_GRAY 7
#define F_YELLOW 14
#define B_RED_F_BLACK 64

/**
 * Initializes the minefield board.
 *
 * @param rows Number of rows
 * @param cols Number of columns
 * @param mine_probability Probability of a cell to be a mine
 * @return Minefiled board
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
 * @param field Minefiled board
 */
void print_field(minefield* field) {
	int i, j;

	// print top header
	for (i = 0; i < field->cols; i++) {
		if (i == 0) {
			printf("  \t  %d ", i);
		}
		else if(i<10) { 
			printf(" %d ", i); 
		}
		else {
			printf("%d ", i);
		}
	}
	printf("\n\n");

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
	if (cell.is_open == 1) {
		// if the cell is a mine
		if (cell.is_mine == 1) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), B_RED_F_BLACK);
			printf(" O ");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RESET);
			return;
		}
		// if the cell is a zero
		if (cell.neig_mines == 0) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), F_GRAY);
			printf(" - ");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RESET);
			return;
		}

		// get cell number color
		switch (cell.neig_mines) {
		case 1:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), F_BLUE);
			break;
		case 2:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), F_GREEN);
			break;
		case 3:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), F_RED);
			break;
		case 4:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), F_D_BLUE);
			break;
		case 5:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), F_PINK);
			break;
		case 6:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), F_CYAN);
			break;
		case 7:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), F_YELLOW);
			break;
		case 8:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), F_L_GRAY);
			break;
		}

		// if the cell is a number
		printf(" %d ", cell.neig_mines);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RESET);

		return;
	}

	// if the cell is not opened
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), B_GRAY);
	printf(" D ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RESET);
}

/**
 * Opens the cell at (x, y).
 * If the cell is 0, it will open all neighbour zeros (in a 'plus').
 *
 * @param field Minefiled board
 * @param x The x position (from 0)
 * @param y The y position (from 0)
 * @return 1 - if the cell is a mine; 0 - if the cell is not a mine
 */
int open_cell(minefield* field, int x, int y) {
	int index = get_index(field, x, y);
	field->cells[index].is_open = 1;
	// if cell is 0, open neighbour cells
	if (field->cells[index].neig_mines == 0) {
		open_zeros(field, x, y);
	}
	return field->cells[index].is_mine;
}

/**
 * Opens all neighbour zeros from (x, y).
 * If the cell is not 0, it will open it as well.
 * Recursive.
 * 
 * @param field Minefiled board
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
				if (field->cells[neig_index].is_open == 0) {
					open_cell(field, i, j);
				}
			}
		}
	}
}

/**
 * Opens all cells in the minefield.
 *
 * @param field Minefiled board
 */
void open_all(minefield* field) {
	int i, j;
	for (i = 0; i < field->rows; i++) {
		for (j = 0; j < field->cols; j++) {
            int index = get_index(field, i, j);
            field->cells[index].is_open = 1;
		}
	}
}

/**
 * Opens all mine cells.
 *
 * @param field Minefiled board
 */
void open_all_mines(minefield* field) {
    int i, j;
    for (i = 0; i < field->rows; i++) {
        for (j = 0; j < field->cols; j++) {
            int index = get_index(field, i, j);
            if(field->cells[index].is_mine == 1) {
                field->cells[index].is_open = 1;
            }
        }
    }
}

/**
 * Checks if the player won the game.
 *
 * @param field Minefiled board
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
 * Returns -1 if posiotion was out of bounds. 
 *
 * @param field Minefiled board
 * @param x The x position (from 0)
 * @param y The y position (from 0)
 * @return the index in array; -1 - if the position was out of bounds
 */
int get_index(minefield* field, int row, int col) {
	if (row < 0 || row >= field->rows || col < 0 || col >= field->cols) {
		return -1;
	}
	return row * field->cols + col;
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

/**
 * Returns a number of neighbour zeros.
 *
 * @param field Minefiled board
 * @param x The x position (from 0)
 * @param y The y position (from 0)
 * @return number of neighbour zeros
 */
int neighbour_zeros(minefield* field, int x, int y) {
	int i, j, zeros = 0;
	int index = get_index(field, x, y);

	for (i = x - 1; i <= x + 1; i++) {
		for (j = y - 1; j <= y + 1; j++) {
			int neig_index = get_index(field, i, j);
			if (neig_index != -1 && neig_index != index) {
				if (field->cells[neig_index].neig_mines == 0) {
					zeros++;
				}
			}
		}
	}

	return zeros;
}