/*
 * fsm.c
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#include "fsm.h"

int tempMove;

void input_inGame(){
	if (is_touch_right() == 1 && move != LEFT){
		move = RIGHT;
	}
	if (is_touch_left() == 1 && move != RIGHT){
		move = LEFT;
	}
	if (is_touch_up() == 1 && move != DOWN){
		move = UP;
	}
	if (is_touch_down() == 1 && move != UP){
		move = DOWN;
	}
	if (is_touch_pause() == 1){
		if (move == NOT){
			move = tempMove;
		}
		else {
			tempMove = move;
			move = NOT;
		}
	}
	if (is_touch_quit() == 1){
		status = GAMEOVER;
		move = NOT;
		snake_init();
		lcd_Clear(WHITE);
	}
}

void input_process(){
	switch (status) {
		case HOME:
			if (is_touch_newGame() == 1){
				status = MODE;
				lcd_Clear(WHITE);
			}
			else if (is_touch_highScore() == 1){
				status = HIGHSCORE;
				lcd_Clear(WHITE);
			}
			break;
		case MODE:
			if (is_touch_classic() == 1){
				status = CLASSIC;
				lcd_Clear(WHITE);
			}
			else if (is_touch_speed() == 1){
				status = SPEED;
				lcd_Clear(WHITE);
			}
			else if (is_touch_time() == 1){
				status = MODE;
				lcd_Clear(WHITE);
			}
			if (is_touch_quit() == 1){
				status = HOME;
				lcd_Clear(WHITE);
			}
			break;
		case HIGHSCORE:
			if (is_touch_quit() == 1){
				status = HOME;
				lcd_Clear(WHITE);
			}
			break;
		case CLASSIC:
			input_inGame();
			break;
		case SPEED:
			input_inGame();
			break;
		case TIME:
			input_inGame();
			break;
		case GAMEOVER:
			if (is_touch_quit() == 1){
				status = MODE;
				lcd_Clear(WHITE);
			}
			break;
		default:
			break;
	}
}

void fsm(){
	switch (status) {
		case HOME:
			home_lcd();
			break;
		case MODE:
			mode_game_lcd();
			break;
		case HIGHSCORE:
			highscore_lcd();
			break;
		case CLASSIC:
			game_lcd();
			break;
		case SPEED:
			game_lcd();
			break;
		case TIME:
			game_lcd();
			break;
		case GAMEOVER:
			game_over_lcd();
			break;
		default:
			break;
	}
}

