/*
 * fsm.c
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#include "fsm.h"

uint32_t counter = 0;

void temp_game(){
	tempFlag = 1;
	tempStatus = status;
	tempMove = move;
}

void continue_game(){
	status = tempStatus;
	move = tempMove;
	reDraw_snake();
}

void input_inGame(){
	if (is_touch_right() == 1 && move != LEFT){
		move = RIGHT;
	}
	else if (is_touch_left() == 1 && move != RIGHT){
		move = LEFT;
	}
	else if (is_touch_up() == 1 && move != DOWN){
		move = UP;
	}
	else if (is_touch_down() == 1 && move != UP){
		move = DOWN;
	}
	else if (is_touch_pause() == 1){
		temp_game();
		move = NOT;
		pause_lcd();
	}
}

void input_process(){
	switch (status) {
		case HOME:
			if (is_touch_newGame() == 1 || button_count[0] == 1){
				tempFlag = 0;
				status = MODE;
				snake_init();
				mode_game_lcd();
			}
			else if (is_touch_continue() == 1){
				if (tempFlag == 1)
					continue_game();
				else {
					home_third_lcd();
				}
			}
			else if (is_touch_highScore() == 1){
				status = HIGHSCORE;
				highscore_lcd();
			}
			break;
		case MODE:
			if (is_touch_classic() == 1){
				status = CLASSIC;
				setTimer3(500);
				game_lcd();
				reDraw_snake();
				score_lcd();
			}
			else if (is_touch_speed() == 1){
				status = SPEED;
				setTimer3(500);
				game_lcd();
				reDraw_snake();
				score_lcd();
			}
			else if (is_touch_time() == 1){
				status = TIME;
				setTimer3(500);
				game_lcd();
				reDraw_snake();
				score_lcd();
			}
			if (is_touch_quit() == 1){
				status = HOME;
				home_lcd();
			}
			break;
		case HIGHSCORE:
			if (is_touch_quit() == 1){
				status = HOME;
				home_lcd();
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
			counter = 0;
			break;
		case GAMEOVER:
			if (is_touch_quit_end() == 1){
				status = MODE;
				mode_game_lcd();
			}
			break;
		case WIN:
			if (is_touch_quit_end() == 1){
				status = MODE;
				mode_game_lcd();
			}
			break;
		case PAUSE:
			if (is_touch_quit_end() == 1){
				status = MODE;
				mode_game_lcd();
			}
			else if (is_touch_resume() == 1){
				continue_game();
			}
			break;
		default:
			break;
	}
}
//tinh toan cap nhat diem so
void score_cal(){
	current_score = current_score + (400 / counter_time_score) * 5 + 5;
	counter_time_score = 0;
	score_lcd();
}

void inGame(){
	move_snake();
	if (is_collision() == 1){
		status = GAMEOVER;
		snake_init();
		game_over_lcd();
	}
	else if (is_eat() == 1){
		length++;
		score_cal();
		rand_food();
	}
	else if (length == 100){
		status = WIN;
		youwin_lcd();
		snake_init();
	}
}

void fsm_ingame(){
	switch (status) {
		case CLASSIC:
			counter_time_score++;
			if (flag_timer3 == 1){
				flag_timer3 = 0;
				inGame();
			}
			break;
		case SPEED:
			counter_time_score++;
			if (flag_timer3 == 1){
				flag_timer3 = 0;
				inGame();
			}
			if (length == 5){
				setTimer3(200);
			}
			else if (length == 10){
				setTimer3(100);
			}
			else if (length == 15){
				setTimer3(50);
			}
			break;
		case TIME:
			counter_time_score++;
			if (++counter % 20 == 0){
				counter_time--;
				update_7seg_time();
			}
			if (flag_timer3 == 1){
				flag_timer3 = 0;
				inGame();
			}
			if (counter_time <= 50){
				setTimer3(200);
			}
			else if (counter_time <= 35){
				setTimer3(100);
			}
			else if (counter_time <= 20){
				setTimer3(50);
			}
			else if (counter_time <= 0){
				status = GAMEOVER;
				game_over_lcd();
				snake_init();
			}
			break;
		default:
			break;
	}
}

