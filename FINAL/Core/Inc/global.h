/*
 * global.h
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

extern int status;

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


#endif /* INC_GLOBAL_H_ */
