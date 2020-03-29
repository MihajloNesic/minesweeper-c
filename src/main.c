// =======================================================
// Title:  Minesweeper
// Author: Mihajlo Nesic (https://mihajlonesic.gitlab.io)
// Date:   27 Mar 2020
// =======================================================

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "functions.h"

void clear() {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
	system("clear");
#endif
#if defined(_WIN32) || defined(_WIN64)
	system("cls");
#endif
}

int main() {
	int f_row, f_col, f_mine;
	int c_row, c_col;
	int game_over = 0, is_win = 0;
	char again;

	clear();
	printf("=== Minesweeper ===\nby Mihajlo Nesic\n\n");

BEGINNING:

	printf("rows> ");
	if (scanf("%d", &f_row) != 1) {
		return -1;
	}

	printf("cols> ");
	if (scanf("%d", &f_col) != 1) {
		return -1;
	}

	printf("mine_probability (1-12-easy;12-15-medium;>15-hard)> ");
	int scan_mine = scanf("%d", &f_mine);
	if (scan_mine != 1) {
		return -1;
	}

	if (f_row < 1 || f_col < 1 || scan_mine != 1 || f_mine < 1) {
		clear();
		goto BEGINNING;
	}

	// easy - 8
	// medium - 12-15
	// hard - >15
	// impossible - 99
	minefield* field = create_field(f_row, f_col, f_mine);

	clear();

START:
	print_field(field);
	printf("x y> ");
	scanf("%d %d", &c_row, &c_col);

	game_over = open_cell(field, c_row, c_col);
	is_win = check_win(field);

	clear();

	if (game_over == 1 || is_win == 1) {
		goto END;
	}

	goto START;

END:
	if (is_win == 1) {
		open_all(field);
		print_field(field);
		printf("you win! :)\n");
	}
	else {
		open_all_mines(field);
		print_field(field);
		printf("game over :(\n");
	}

	printf("again? (y/n)> ");
	scanf(" %c", &again);

	if(again == 'y' || again == 'Y') {
		clear();
		goto BEGINNING;
	}

	printf("bye\n");

	free(field);

	return 0;
}