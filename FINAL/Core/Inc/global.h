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
	char name[11];
	uint16_t score;
};
extern struct History history[MAX_HISTORY + 1];
extern uint8_t num_history;

// Dinh nghia thong tin nguoi choi
extern char name[11];
extern uint8_t name_length;
extern uint32_t current_score;
extern float current_temp;
extern uint16_t current_hum;
extern uint16_t current_light;
extern uint32_t current_time;
extern uint8_t flag_user;

// Cac truong hop cua flag_playing
#define FLAG_NOTSTART	0
#define FLAG_PLAYING	1
#define FLAG_END		2
#define FLAG_PAUSE		3

// Dinh nghia bang chu cai
#define CHAR_ROW	5		// So hang bang nhap chu
#define CHAR_COL	6		// So cot bang nhap chu

#define IDX_R_UPPER	4
#define IDX_C_UPPER	0
#define IDX_R_DEL	4
#define IDX_C_DEL	2
#define IDX_R_ENTR	4
#define IDX_C_ENTR	5

extern uint8_t flag_change;
extern uint8_t flag_uppercase;
extern uint8_t index_r;
extern uint8_t index_c;
extern uint8_t index_r_old;
extern uint8_t index_c_old;
extern char char_upper[CHAR_ROW][CHAR_COL];
extern char char_lower[CHAR_ROW][CHAR_COL];

// Dinh nghia thong tin ran
#define MAX_LENGTH_SNAKE 	5	// Chieu dai toi da cua ran
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
extern int counter_time;

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
