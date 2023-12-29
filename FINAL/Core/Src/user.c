/*
 * user.c
 *
 *  Created on: Dec 29, 2023
 *      Author: Duyen
 */

#include "user.h"

void user_init(){
	memset(name, '\0', sizeof(name));	// Ten nguoi choi nhap vao
	name_length = 0;					// Do dai ten nguoi choi
	current_score = 0;					// Diem hien tai cua nguoi choi
	current_temp = 0.0;					// Nhiet do hien tai
	current_hum = 0;					// Do am hien tai
	current_light = 0;					// Anh sang hien tai
	current_time = 0;					// Tong thoi gian choi hien tai
	flag_user = FLAG_NOTSTART;			// Trang thai nguoi choi
}

void user_history(){
	flag_user = FLAG_END;
	if(num_history == 0){
		strcpy(history[0].name, name);
		history[0].score = current_score;
		num_history++;
		return;
	}
	uint8_t i;
	for(i = num_history; i > 0; i--){
		if(history[i - 1].score < current_score){
			history[i] = history[i - 1];
		}else{
			break;
		}
	}
	strcpy(history[i].name, name);
	history[i].score = current_score;
	if(num_history < MAX_HISTORY){
		num_history++;
	}
}
