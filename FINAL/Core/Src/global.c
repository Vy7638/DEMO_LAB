/*
 * global.c
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#include "global.h"

struct History history[MAX_HISTORY];
uint8_t num_history = 2;

uint32_t current_score = 10000;

int status = 0;
int length = 1;
int move = 0;

struct pointTouch {
	int x_begin;
	int x_end;
	int y_begin;
	int y_end;
};

struct Point snake[MAX_LENGTH_SNAKE];

struct pointTouch pRight;
struct pointTouch pLeft;
struct pointTouch pUp;
struct pointTouch pDown;

struct pointTouch pQuit;
struct pointTouch pPause;

struct pointTouch pNewGame;
struct pointTouch pHighScore;

struct pointTouch pClassic;
struct pointTouch pSpeed;
struct pointTouch pTime;

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
