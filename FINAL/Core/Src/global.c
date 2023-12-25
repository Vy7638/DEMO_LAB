/*
 * global.c
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#include "global.h"

struct History history[MAX_HISTORY];
uint8_t num_history = 2;

uint32_t current_score = 10000;

int status = HOME;
uint32_t length = 1;
int move = 0;
uint32_t counter_time_score = 0;
uint32_t counter_time = 0;

int tempFlag = 0;
int tempStatus = 0;
int tempMove = 0;

struct Point snake[MAX_LENGTH_SNAKE];
struct Point food = {50, 50};


