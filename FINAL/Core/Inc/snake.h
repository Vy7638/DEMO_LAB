/*
 * snake.h
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#ifndef INC_SNAKE_H_
#define INC_SNAKE_H_

#include "stdlib.h"
#include "global.h"
#include "interface.h"


void snake_init();

int is_collision();
int is_eat();

void rand_food();

void move_snake();
void reDraw_snake();

#endif /* INC_SNAKE_H_ */
