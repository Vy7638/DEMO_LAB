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
#define MODE		1		// Tu home bam newgame
#define CONTINUE	2		// Tu home bam continue
#define HIGHSCORE	3		// Tu home bam highscore

#define CLASSIC		11		// Tu mode bam classic
#define SPEED		12		// Tu mode bam speed
#define TIME		13		// Tu mode bam time

#define PAUSE		15
#define GAMEOVER	16
#define WIN			17
#define NAME		18

// Dinh nghia danh sach lich su
#define MAX_HISTORY	9		// So lich su toi da duoc luu
struct History{
	char name[10];
	uint16_t score;
};
extern struct History history[MAX_HISTORY];
extern uint8_t num_history;

// Dinh nghia thong tin nguoi choi
extern char name[10];
extern uint32_t current_score;
extern float current_temp;
extern uint16_t current_hum;
extern uint16_t current_light;

// Dinh nghia thong tin ran
#define MAX_LENGTH_SNAKE 	100	// Chieu dai toi da cua ran
#define RADIUS_SNAKE		4	// Ban kinh than ran

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

// Cac bien tam luu tru trang thai cua ran
extern int tempFlag;
extern int tempStatus;
extern int tempMove;

extern struct Point snake[MAX_LENGTH_SNAKE];
extern struct Point food;

#endif /* INC_GLOBAL_H_ */
