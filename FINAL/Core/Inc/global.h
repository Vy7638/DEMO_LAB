/*
 * global.h
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#define HOME	0

#define MODE 		1
#define HIGHSCORE	2
#define GAMEOVER	3

#define CLASSIC		11
#define SPEED		12
#define TIME		13

#define MAX_LENGTH_SNAKE 	100

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

void pointTouch_init();
void snake_init();

#endif /* INC_GLOBAL_H_ */
