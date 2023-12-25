/*
 * interface.h
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#ifndef INC_INTERFACE_H_
#define INC_INTERFACE_H_

#include "lcd.h"
#include "led_7seg.h"
#include "global.h"
#include "picture.h"

#define POS_X_FIRST 65		// Toa do x goc tren ben trai CLASSIC
#define POS_Y_FIRST 100		// Toa do y goc tren ben trai CLASSIC
#define POS_X_SECOND 65		// Toa do x goc tren ben trai NEWGAME/SPEED
#define POS_Y_SECOND 150	// Toa do y goc tren ben trai NEWGAME/SPEED
#define POS_X_THIRD 65		// Toa do x goc tren ben trai CONTINUE/TIME
#define POS_Y_THIRD 200		// Toa do y goc tren ben trai CONTINUE/TIME
#define POS_X_FORTH	65		// Toa do x goc tren ben trai HIGHSCORE/QUIT
#define POS_Y_FORTH	250		// Toa do y goc tren ben trai HIGHSCORE/QUIT
#define WIDTH 120			// Chieu rong o
#define HEIGHT 35			// Chieu dai o

#define POS_X_RANK	10		// Toa do x dat chu RANK
#define POS_X_NAME	60		// Toa do x dat chu NAME
#define POS_X_SCORE	160		// Toa do x dat chu SCORE
#define POS_Y_RANK	80		// Toa do y dat chu RANK
#define SPACE		16		// Khoang cach den dong tiep theo

#define POS_X_MIN	0		// Toa do x nho nhat ran duoc di chuyen
#define POS_Y_MIN	0		// Toa do y nho nhat ran duoc di chuyen
#define POS_X_MAX	239		// Toa do x lon nhat ran duoc di chuyen
#define POS_Y_MAX	199		// Toa do y lon nhat ran duoc di chuyen

#define POS_X_LEFT	5
#define POS_Y_LEFT	265
#define POS_X_RIGHT	165
#define POS_Y_RIGHT	265
#define POS_X_UP	85
#define POS_Y_UP	205
#define POS_X_DOWN	85
#define POS_Y_DOWN	265

#define POS_X_PAUSE		200
#define POS_Y_PAUSE		230
#define POS_X_CURSCORE	5
#define POS_Y_CURSCORE	205
#define BTN_WIDTH		70
#define BTN_HEIGHT		50

void home_lcd(); 				// Man hinh chinh khi chua bat dau game
void mode_game_lcd();			// Lua chon che do choi
void highscore_lcd();			// Hien thi ket qua cua nguoi choi diem cao nhat (10 nguoi)
void game_lcd();				// Man hinh game
void score_lcd();				// Hien thi diem hien tai
void pause_lcd();				// Man hinh tam ngung
void game_over_lcd();			// Hien thi ket qua thua
void youwin_lcd();				// Hien thi ket qua thang

void home_second_lcd();
void home_third_lcd();
void home_forth_lcd();

void mode_first_lcd();
void mode_second_lcd();
void mode_third_lcd();
void mode_forth_lcd();

void pause_second_lcd();
void pause_third_lcd();

void point_head_lcd(struct Point head);		// Ve dau ran
void point_snake_lcd(struct Point snake);	// Ve 1 diem cua ran
void point_food_lcd(struct Point food);		// Ve moi
void point_clr(struct Point clr);			// To de diem do thanh mau nen (ho tro ran di chuyen)

void mode_7seg();
void update_7seg_time();

#endif /* INC_INTERFACE_H_ */
