/*
 * fsm.c
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#include "fsm.h"

uint8_t confirm[4] = {0, 0, 0, 0};

void clr_confirm(){
	for(int i = 0; i < 4; i++){
		confirm[i] = 0;
	}
}

void temp_game(){
	tempFlag = 1;
	tempStatus = status;
	tempMove = move;
	flag_user = FLAG_PAUSE;
}

void continue_game(){
	if(tempFlag == 1){
		status = tempStatus;
		move = tempMove;
		reDraw_snake();
		point_food_lcd(food);
		score_lcd();
		flag_user = FLAG_PLAYING;
	}
}

void input_inGame(){
	if((is_touch_right() == 1 || button_count[6] == 1) && move != LEFT){
		move = RIGHT;
	}else if((is_touch_left() == 1 || button_count[4] == 1) && move != RIGHT){
		move = LEFT;
	}else if((is_touch_up() == 1 || button_count[1] == 1) && move != DOWN){
		move = UP;
	}else if((is_touch_down() == 1 || button_count[9] == 1) && move != UP){
		move = DOWN;
	}else if(is_touch_pause() == 1 || button_count[5] == 1){
		temp_game();
		status = PAUSE;
		move = NOT;
		pause_lcd();
	}
}

void input_enterName(){
	flag_change = 1;
	if(is_touch_char() == 1){
		index_r = (touch_GetY() - POS_Y_CHAR)/CHAR_HEIGHT;
		index_c = (touch_GetX() - POS_X_CHAR)/CHAR_WIDTH;
	}else{
		if(button_count[1] == 1){
			if(index_r > 0) index_r--;
		}else if(button_count[9] == 1){
			if(index_r < CHAR_ROW - 1) index_r++;
		}else if(button_count[4] == 1){
			if(index_c > 0) index_c--;
		}else if(button_count[6] == 1){
			if(index_c < CHAR_COL - 1) index_c++;
		}else if(button_count[5] == 1){
			index_r = index_r_old;
			index_c = index_c_old;
		}else{
			flag_change = 0;
		}
	}
	if(flag_change == 1){
		if(index_r == index_r_old && index_c == index_c_old){
			if(index_r == IDX_R_UPPER && index_c == IDX_C_UPPER){
				if(flag_uppercase == 1){
					flag_uppercase = 0;
					lowercase_lcd();
				}else{
					flag_uppercase = 1;
					uppercase_lcd();
				}
			}else if(index_r == IDX_R_DEL && index_c == IDX_C_DEL){
				if(name_length > 0){
					name_length--;
					name[name_length] = '\0';
					displayName_lcd();
				}
			}else if(index_r == IDX_R_ENTR && index_c == IDX_C_ENTR){
				if(name_length > 0){
					status = tempStatus;
					setTimer3(500);
					game_lcd();					// Hien thi man hinh choi game
					mode_7seg();				// Ham hien thi che do choi tren led 7seg
					reDraw_snake();				// Ham goi ve con ran
					point_food_lcd(food);
					environment_lcd();			// Ham moi truong
					score_lcd();				// Ham diem so
					flag_user = FLAG_PLAYING;
					return;
				}
			}else if(name_length < 10){
				if(flag_uppercase == 1){
					name[name_length] = char_upper[index_r][index_c];
				}else{
					name[name_length] = char_lower[index_r][index_c];
				}
				name_length++;
				displayName_lcd();
			}
		}
		selectbox_lcd();
	}
}

void input_process(){
	switch(status){
		case HOME:
			if(is_touch_newGame() == 1 || button_count[3] == 1){
				if(confirm[1] == 1){
					clr_confirm();
					status = MODE;
					user_init();
					snake_init();
					mode_game_lcd();
				}else{
					clr_confirm();
					confirm[1] = 1;
					home_second_lcd();
				}
			}else if(is_touch_continue() == 1 || button_count[7] == 1){
				if(confirm[2] == 1 && tempFlag == 1){
					clr_confirm();
					game_lcd();
					continue_game();
				}else{
					clr_confirm();
					confirm[2] = 1;
					home_third_lcd();
				}
			}else if(is_touch_highScore() == 1 || button_count[11] == 1){
				if(confirm[3] == 1){
					clr_confirm();
					status = HIGHSCORE;
					highscore_lcd();
				}else{
					clr_confirm();
					confirm[3] = 1;
					home_forth_lcd();
				}
			}
			break;
		case MODE:
			if(is_touch_classic() == 1 || button_count[3] == 1){
				if(confirm[0] == 1){
					clr_confirm();
					tempStatus = CLASSIC;
					status = NAME;
					entername_lcd();
				}else{
					clr_confirm();
					confirm[0] = 1;
					mode_first_lcd();
				}
			}else if(is_touch_speed() == 1 || button_count[7] == 1){
				if(confirm[1] == 1){
					clr_confirm();
					tempStatus = SPEED;
					status = NAME;
					entername_lcd();
				}else{
					clr_confirm();
					confirm[1] = 1;
					mode_second_lcd();
				}
			}else if(is_touch_time() == 1 || button_count[11] == 1){
				if(confirm[2] == 1){
					clr_confirm();
					tempStatus = TIME;
					status = NAME;
					entername_lcd();
				}else{
					clr_confirm();
					confirm[2] = 1;
					mode_third_lcd();
				}
			}
			if(is_touch_quit() == 1 || button_count[15] == 1){
				if(confirm[3] == 1){
					clr_confirm();
					status = HOME;
					home_lcd();
				}else{
					clr_confirm();
					confirm[3] = 1;
					mode_forth_lcd();
				}
			}
			break;
		case HIGHSCORE:
			if(is_touch_quit() == 1 || button_count[3] == 1){
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
			if(is_touch_quit() == 1 || button_count[3] == 1){
				status = HOME;
				home_lcd();
				reset_7seg();				// Thoat che do choi game led 7 se quay tro ve 0
			}
			break;
		case WIN:
			if(is_touch_quit() == 1 || button_count[3] == 1){
				status = HOME;
				home_lcd();
				reset_7seg();
			}
			break;
		case NAME:
			input_enterName();
			break;
		case PAUSE:
			if(is_touch_quit_end() == 1 || button_count[7] == 1){
				if(confirm[2] == 1){
					clr_confirm();
					status = HOME;
					home_lcd();
				}else{
					clr_confirm();
					confirm[2] = 1;
					pause_third_lcd();
				}
			}else if(is_touch_resume() == 1 || button_count[3] == 1){
				if(confirm[1] == 1){
					clr_confirm();
					game_lcd();
					continue_game();
				}else{
					clr_confirm();
					confirm[1] = 1;
					pause_second_lcd();
				}
			}
			break;
		default:
			break;
	}
}

// Cap nhat thong tin moi truong
void environment_cal(){
	sensor_Read();
	current_temp = sensor_GetTemperature();
	current_hum = ((float)sensor_GetPotentiometer() / 4095.0) * 100;
	current_light = 4095 - sensor_GetLight();
	environment_lcd();
}

// Tinh toan cap nhat diem so
void score_cal(){
	current_score = current_score + (400 / counter_time_score) * 5 + 5;
	counter_time_score = 0;
	score_lcd();
}

void inGame(){
	if(move != NOT){
		move_snake();
	}
	if(is_collision() == 1){
		status = GAMEOVER;
		game_over_lcd();
		user_history();
		snake_init();
	}else if(is_eat() == 1){
		length++;
		score_cal();
		rand_food();
	}else if(length == MAX_LENGTH_SNAKE){
		status = WIN;
		youwin_lcd();
		user_history();
		snake_init();
	}
}

void fsm_ingame(){
	switch(status){
		case CLASSIC:
			counter_time_score++;
			if(flag_timer4 == 1){
				flag_timer4 = 0;
				environment_cal();
			}
			if(flag_timer3 == 1){
				flag_timer3 = 0;
				inGame();
			}
			break;
		case SPEED:
			counter_time_score++;
			if(flag_timer4 == 1){
				flag_timer4 = 0;
				environment_cal();
			}
			if(flag_timer3 == 1){
				flag_timer3 = 0;
				inGame();
				if(length == 5){
					setTimer3(200);
				}else if(length == 10){
					setTimer3(100);
				}else if(length == 15){
					setTimer3(50);
				}
			}
			break;
		case TIME:
			counter_time_score++;
			if(flag_timer4 == 1){
				flag_timer4 = 0;
				environment_cal();
			}
			if(flag_timer3 == 1){
				flag_timer3 = 0;
				inGame();
				if(counter_time == 50){
					setTimer3(200);
				}else if(counter_time == 35){
					setTimer3(100);
				}else if(counter_time == 20){
					setTimer3(50);
				}else if(counter_time <= 0){
					status = WIN;
					youwin_lcd();
					user_history();
					snake_init();
				}
			}
			break;
		default:
			break;
	}
}
