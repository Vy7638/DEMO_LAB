/*
 * fsm.h
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "global.h"
#include "interface.h"

#define HOME	0

#define MODE 		1
#define HIGHSCORE	2
#define GAMEOVER	3

#define CLASSIC		11
#define SPEED		12
#define TIME		13

void input_process();	//xu ly du lieu dau vao
void fsm();

#endif /* INC_FSM_H_ */
