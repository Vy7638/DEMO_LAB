/*
 * snake.c
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#include "snake.h"

void snake_init(){
	snake[0].x = (POS_X_MAX - POS_X_MIN) / 2;
	snake[0].y = (POS_Y_MAX - POS_Y_MIN) / 2;
	for (int i = 1; i < MAX_LENGTH_SNAKE; i++){
		snake[i].x = 0;
		snake[i].y = 0;
	}
	length = 1;
	current_score = 0;
	counter_time_score = 0;
	counter_time = 60;
	tempFlag = 0;
}

int is_collision_food(struct Point p){
	if (p.x <= POS_X_MIN || p.x >= POS_X_MAX ||
		p.y <= POS_Y_MIN || p.y >= POS_Y_MAX){
		return 1;
	}
	for (int i = 0; i < length; i++){
		if (p.x == snake[i].x && p.y == snake[i].y)
			return 1;
	}
	return 0;
}

int is_collision(){
	if (snake[0].x <= POS_X_MIN || snake[0].x >= POS_X_MAX ||
		snake[0].y <= POS_Y_MIN || snake[0].y >= POS_Y_MAX){
		return 1;
	}
	for (int i = 1; i < length; i++){
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
			return 1;
	}
	return 0;
}

int is_eat(){
	if (snake[0].x == food.x && snake[0].y == food.y) return 1;
	return 0;
}

int get_rand(int max){
	return 0 + (int)(rand()*(max - 0 + 1.0)/(1.0 + RAND_MAX));
}

void rand_food(){
	struct Point p;
	p.x = get_rand(POS_X_MAX);
	p.y = get_rand(POS_Y_MAX);
	while (is_collision_food(p) == 1){
		p.x = get_rand(POS_X_MAX);
		p.y = get_rand(POS_Y_MAX);
	}
	food = p;
	point_food_lcd(food);
}

void move_snake(){
	point_clr(snake[length - 1]);
	for (int i = length - 1; i > 0; i--){
		snake[i].x = snake[i-1].x;
		snake[i].y = snake[i-1].y;
	}
	switch (move) {
		case RIGHT:
			snake[0].x += 1;
			break;
		case LEFT:
			snake[0].x -= 1;
			break;
		case UP:
			snake[0].y += 1;
			break;
		case DOWN:
			snake[0].y -= 1;
			break;
		default:
			break;
	}
}

void reDraw_snake(){
	point_head_lcd(snake[0]);
	for (int i = 1; i < length; i++){
		point_snake_lcd(snake[i]);
	}
}
