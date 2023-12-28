/*
 * fsm.c
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#include "fsm.h"

uint32_t counter = 0;

uint8_t confirm[4] = {0, 0, 0, 0};

void clr_confirm(){
	for (int i = 0; i < 4; i++){
		confirm[i] = 0;
	}
}

void temp_game(){
	tempFlag = 1;
	tempStatus = status;
	tempMove = move;
}

void continue_game(){
	if (tempFlag == 1){
		status = tempStatus;
		move = tempMove;
		reDraw_snake();
		point_food_lcd(food);
	}
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
		status = PAUSE;
		move = NOT;
		pause_lcd();
	}
}

void input_process(){
	switch (status) {
		case HOME:
			if (is_touch_newGame() == 1){
				if (confirm[1] == 1){
					clr_confirm();
					status = MODE;
					snake_init();
					mode_game_lcd();
				}
				else {
					clr_confirm();
					confirm[1] = 1;
					home_second_lcd();
				}
			}
			else if (is_touch_continue() == 1){
				if (confirm[2] == 1 && tempFlag == 1){
					clr_confirm();
					game_lcd();
					continue_game();
				}
				else {
					clr_confirm();
					confirm[2] = 1;
					home_third_lcd();
				}
			}
			else if (is_touch_highScore() == 1){
				if (confirm[3] == 1){
					clr_confirm();
					status = HIGHSCORE;
					highscore_lcd();
				}
				else {
					clr_confirm();
					confirm[3] = 1;
					home_forth_lcd();
				}

			}
			break;
		case MODE:
			if (is_touch_classic() == 1){
				if (confirm[0] == 1){
					clr_confirm();
					status = CLASSIC;
					setTimer3(500);
					game_lcd();					//hien thi man hinh choi game
					mode_7seg();				//ham hien thi che do choi tren led 7seg
					reDraw_snake();				//ham goi ve con ran
					point_food_lcd(food);
					score_lcd();				//ham diem so
				}
				else {
					clr_confirm();
					confirm[0] = 1;
					mode_first_lcd();
				}
			}
			else if (is_touch_speed() == 1){
				if (confirm[1] == 1){
					clr_confirm();
					status = SPEED;
					setTimer3(500);
					game_lcd();
					mode_7seg();
					reDraw_snake();
					point_food_lcd(food);
					score_lcd();
				}
				else {
					clr_confirm();
					confirm[1] = 1;
					mode_second_lcd();
				}
			}
			else if (is_touch_time() == 1){
				if (confirm[2] == 1){
					clr_confirm();
					status = TIME;
					setTimer3(500);
					game_lcd();
					mode_7seg();
					reDraw_snake();
					point_food_lcd(food);
					counter = 0;			//gia tri counter dem thoi gian duoc gan 0
					score_lcd();
				}
				else {
					clr_confirm();
					confirm[2] = 1;
					mode_third_lcd();
				}
			}
			if (is_touch_quit() == 1){
				if (confirm[3] == 1){
					clr_confirm();
					status = HOME;
					home_lcd();
				}
				else {
					clr_confirm();
					confirm[3] = 1;
					mode_forth_lcd();
				}
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
			break;
		case GAMEOVER:
			if (is_touch_quit_end() == 1){
				status = MODE;
				mode_game_lcd();
				reset_7seg();						//thoat che do choi game led 7 se quay tro ve 0
			}
			break;
		case WIN:
			if (is_touch_quit_end() == 1){
				status = MODE;
				mode_game_lcd();
				reset_7seg();
			}
			break;
		case PAUSE:
			if (is_touch_quit_end() == 1){
				status = MODE;
				mode_game_lcd();
				reset_7seg();
			}
			else if (is_touch_resume() == 1){
				game_lcd();
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
	if (move != NOT){
		move_snake();
	}
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

