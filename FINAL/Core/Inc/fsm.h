/*
 * fsm.h
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "global.h"
#include "lcd.h"
#include "sensor.h"
#include "interface.h"
#include "fun_touch.h"
#include "button.h"
#include "snake.h"

void input_process();	//xu ly du lieu dau vao
void fsm_ingame();

#endif /* INC_FSM_H_ */
