/*
 * global.c
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#include "global.h"

// Cac du lieu can de luu lich su
struct History history[MAX_HISTORY];	// Mang chua lich su MAX_HISTORY lan choi co diem cao nhat
uint8_t num_history = 0;				// So lich su dang duoc luu hien tai

// Cac du lieu can de luu thong tin nguoi choi
char name[10] = "";					// Ten nguoi choi nhap vao
uint32_t current_score = 1000;			// Diem hien tai cua nguoi choi
float current_temp = 37.5;				// Nhiet do hien tai
uint16_t current_hum = 70;			// Do am hien tai
uint16_t current_light = 4000;			// Anh sang hien tai

int status = HOME;
int move = 0;
uint32_t length = 1;
uint32_t counter_time_score = 0;
uint32_t counter_time = 0;

int tempFlag = 0;
int tempStatus = 0;
int tempMove = 0;

struct Point snake[MAX_LENGTH_SNAKE];
struct Point food = {50, 50};
