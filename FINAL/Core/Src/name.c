/*
 * user.c
 *
 *  Created on: Dec 28, 2023
 *      Author: Duyen
 */

#include <name.h>

uint8_t flag_uppercase = 0;
uint8_t index_x = 5;
uint8_t index_y = 4;
uint8_t index_x_old = 0;
uint8_t index_y_old = 0;

char char_upper[CHAR_ROW][CHAR_COL] = {
		{'A', 'B', 'C', 'D', 'E', 'F'},
		{'G', 'H', 'I', 'J', 'K', 'L'},
		{'M', 'N', 'O', 'P', 'Q', 'R'},
		{'S', 'T', 'U', 'V', 'W', 'X'},
		{'^', '.', 'Y', 'Z', '.', '>'}
};

char char_lower[CHAR_ROW][CHAR_COL] = {
		{'a', 'b', 'c', 'd', 'e', 'f'},
		{'g', 'h', 'i', 'j', 'k', 'l'},
		{'m', 'n', 'o', 'p', 'q', 'r'},
		{'s', 't', 'u', 'v', 'w', 'x'},
		{'^', '.', 'y', 'z', '.', '>'}
};
