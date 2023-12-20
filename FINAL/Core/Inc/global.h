/*
 * global.h
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"

// Dinh nghia cac man hinh
#define HOME		0
#define MODE 		1
#define HIGHSCORE	2
#define GAMEOVER	3

#define CLASSIC		11
#define SPEED		12
#define TIME		13

// Dinh nghia danh sach lich su
#define MAX_HISTORY	9	// So lich su toi da duoc luu
struct History{
	char name[10];
	uint16_t score;
};
extern struct History history[MAX_HISTORY];
extern uint8_t num_history;

extern uint32_t current_score;

#define MAX_LENGTH_SNAKE 	100
#define RADIUS_SNAKE		4

#define	NOT			0
#define RIGHT		1
#define LEFT		2
#define UP			3
#define DOWN		4

extern int status;
extern int move;
extern int length;

struct Point{
	int x;
	int y;
};

extern struct pointTouch pRight;
extern struct pointTouch pLeft;
extern struct pointTouch pUp;
extern struct pointTouch pDown;

extern struct pointTouch pQuit;
extern struct pointTouch pPause;

extern struct pointTouch pNewGame;
extern struct pointTouch pHighScore;

extern struct pointTouch pClassic;
extern struct pointTouch pSpeed;
extern struct pointTouch pTime;

void pointTouch_init();
void snake_init();

#endif /* INC_GLOBAL_H_ */
