/*
 * fun_touch.h
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#ifndef INC_FUN_TOUCH_H_
#define INC_FUN_TOUCH_H_

#include "global.h"
#include "touch.h"

int is_touch_right();
int is_touch_left();
int is_touch_up();			//di chuyen phai, trai, len, xuong
int is_touch_down();

int is_touch_quit();
int is_touch_pause();

int is_touch_newGame();
int is_touch_highScore();

int is_touch_classic();
int is_touch_speed();
int is_touch_time();

#endif /* INC_FUN_TOUCH_H_ */
