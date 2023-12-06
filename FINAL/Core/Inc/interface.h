/*
 * interface.h
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#ifndef INC_INTERFACE_H_
#define INC_INTERFACE_H_

#include "lcd.h"
#include "global.h"

void home_lcd(); 				//man hinh chinh khi chua bat dau game
void mode_game_lcd();			//lua chon che do choi
void highscore_lcd();			//hien thi ket qua cua nguoi choi diem cao nhat (co the 3 nguoi)
void game_lcd();				//man hinh game
void game_over_lcd();			//hien thi ket qua

void point_snake_lcd(struct Point snake);			//ve 1 diem cua ran
void point_food_lcd(struct Point food);				//ve moi
void point_clr(struct Point clr);				//to de diem do thanh mau nen (ho tro ran di chuyen)

#endif /* INC_INTERFACE_H_ */
