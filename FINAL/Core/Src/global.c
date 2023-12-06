/*
 * global.c
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#include "global.h"

int status = 0;
int length = 1;
int move = 0;

void pointTouch_init(){
	pRight.x_begin = 0;
	pRight.x_end = 100;
	pRight.y_begin = 0;
	pRight.y_end = 100;
	// tuong tu cho cac bien con lai
}

void snake_init(){
	snake[0].x = 120;
	snake[0].y = 160;
	for (int i = 1; i < MAX_LENGTH_SNAKE; i++){
		snake[i].x = 0;
		snake[i].y = 0;
	}
}
