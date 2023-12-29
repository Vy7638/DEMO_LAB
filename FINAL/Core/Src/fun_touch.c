/*
 * fun_touch.c
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#include "fun_touch.h"

// Tinh khoang cach hai diem
float distance_of_two_point(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
	return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

int is_touch_right(){
	if (!touch_IsTouched()) return 0;
	return (touch_GetX() > POS_X_RIGHT
			&& touch_GetX() < POS_X_RIGHT + BTN_WIDTH
			&& touch_GetY() > POS_Y_RIGHT
			&& touch_GetY() < POS_Y_RIGHT + BTN_HEIGHT);
}

int is_touch_left(){
	if (!touch_IsTouched()) return 0;
	return (touch_GetX() > POS_X_LEFT
			&& touch_GetX() < POS_X_LEFT + BTN_WIDTH
			&& touch_GetY() > POS_Y_LEFT
			&& touch_GetY() < POS_Y_LEFT + BTN_HEIGHT);
}

int is_touch_up(){
	if (!touch_IsTouched()) return 0;
	return (touch_GetX() > POS_X_UP
			&& touch_GetX() < POS_X_UP + BTN_WIDTH
			&& touch_GetY() > POS_Y_UP
			&& touch_GetY() < POS_Y_UP + BTN_HEIGHT);
}

int is_touch_down(){
	if (!touch_IsTouched()) return 0;
	return (touch_GetX() > POS_X_DOWN
			&& touch_GetX() < POS_X_DOWN + BTN_WIDTH
			&& touch_GetY() > POS_Y_DOWN
			&& touch_GetY() < POS_Y_DOWN + BTN_HEIGHT);
}

int is_touch_quit(){
	if (!touch_IsTouched()) return 0;
	return (touch_GetX() > POS_X_FORTH
			&& touch_GetX() < POS_X_FORTH + WIDTH
			&& touch_GetY() > POS_Y_FORTH
			&& touch_GetY() < POS_Y_FORTH + HEIGHT);
}

int is_touch_quit_end(){
	if (!touch_IsTouched()) return 0;
	return (touch_GetX() > POS_X_THIRD
			&& touch_GetX() < POS_X_THIRD + WIDTH
			&& touch_GetY() > POS_Y_THIRD
			&& touch_GetY() < POS_Y_THIRD + HEIGHT);
}

int is_touch_resume(){
	if (!touch_IsTouched()) return 0;
	return (touch_GetX() > POS_X_SECOND
			&& touch_GetX() < POS_X_SECOND + WIDTH
			&& touch_GetY() > POS_Y_SECOND
			&& touch_GetY() < POS_Y_SECOND + HEIGHT);
}

int is_touch_pause(){
	if (!touch_IsTouched()) return 0;
	return distance_of_two_point(touch_GetX(), touch_GetY(), POS_X_PAUSE, POS_Y_PAUSE) < BTN_HEIGHT / 2;
}

int is_touch_newGame(){
	if (!touch_IsTouched()) return 0;
	return (touch_GetX() > POS_X_SECOND
			&& touch_GetX() < POS_X_SECOND + WIDTH
			&& touch_GetY() > POS_Y_SECOND
			&& touch_GetY() < POS_Y_SECOND + HEIGHT);
}

int is_touch_continue(){
	if (!touch_IsTouched()) return 0;
	return (touch_GetX() > POS_X_THIRD
			&& touch_GetX() < POS_X_THIRD + WIDTH
			&& touch_GetY() > POS_Y_THIRD
			&& touch_GetY() < POS_Y_THIRD + HEIGHT);
}

int is_touch_highScore(){
	if (!touch_IsTouched()) return 0;
	return (touch_GetX() > POS_X_FORTH
			&& touch_GetX() < POS_X_FORTH + WIDTH
			&& touch_GetY() > POS_Y_FORTH
			&& touch_GetY() < POS_Y_FORTH + HEIGHT);
}

int is_touch_classic(){
	if (!touch_IsTouched()) return 0;
	return (touch_GetX() > POS_X_FIRST
			&& touch_GetX() < POS_X_FIRST + WIDTH
			&& touch_GetY() > POS_Y_FIRST
			&& touch_GetY() < POS_Y_FIRST + HEIGHT);
}

int is_touch_speed(){
	if (!touch_IsTouched()) return 0;
	return (touch_GetX() > POS_X_SECOND
			&& touch_GetX() < POS_X_SECOND + WIDTH
			&& touch_GetY() > POS_Y_SECOND
			&& touch_GetY() < POS_Y_SECOND + HEIGHT);
}

int is_touch_time(){
	if (!touch_IsTouched()) return 0;
	return (touch_GetX() > POS_X_THIRD
			&& touch_GetX() < POS_X_THIRD + WIDTH
			&& touch_GetY() > POS_Y_THIRD
			&& touch_GetY() < POS_Y_THIRD + HEIGHT);
}

int is_touch_char(){
	if (!touch_IsTouched()) return 0;
	return (touch_GetX() > POS_X_CHAR
			&& touch_GetX() < POS_X_CHAR + CHAR_COL*CHAR_WIDTH
			&& touch_GetY() > POS_Y_CHAR
			&& touch_GetY() < POS_Y_CHAR + CHAR_ROW*CHAR_HEIGHT);
}
