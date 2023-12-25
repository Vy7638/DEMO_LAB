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
#define MODE 		1		//tu home bam newgame
#define CONTINUE	2		//tu home bam continue
#define HIGHSCORE	3		//tu home bam highscore

#define NAME		10
#define CLASSIC		11
#define SPEED		12
#define TIME		13

#define PAUSE		21
#define GAMEOVER	22
#define WIN			23

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
extern uint32_t length;
extern uint32_t counter_time_score;
extern uint32_t counter_time;

struct Point{
	int x;
	int y;
};
//cac bien tam luu tru trang thai cua ran
extern int tempFlag;
extern int tempStatus;
extern int tempMove;


extern struct Point snake[MAX_LENGTH_SNAKE];
extern struct Point food;

#endif /* INC_GLOBAL_H_ */
