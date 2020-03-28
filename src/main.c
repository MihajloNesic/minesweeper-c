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
	int c_row, c_col;
	int game_over = 0, is_win = 0;

	// easy - 8
	// medium - 12-15
	// hard - >15
	// impossible - 99
	minefield* field = create_field(15, 22, 13);

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
        printf("you win! :)");
    }
    else {
        open_all_mines(field);
        print_field(field);
        printf("game over :(");
    }

	free(field);

	return 0;
}