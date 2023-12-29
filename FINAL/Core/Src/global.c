/*
 * global.c
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#include "global.h"

// Cac du lieu can de luu lich su
struct History history[MAX_HISTORY + 1];	// Mang chua lich su MAX_HISTORY lan choi co diem cao nhat
uint8_t num_history = 0;				// So lich su dang duoc luu hien tai

// Cac du lieu can de luu thong tin nguoi choi
char name[11] = "";					// Ten nguoi choi nhap vao
uint8_t name_length = 0;			// Do dai ten nguoi choi
uint32_t current_score = 1000;		// Diem hien tai cua nguoi choi
float current_temp = 37.5;			// Nhiet do hien tai
uint16_t current_hum = 70;			// Do am hien tai
uint16_t current_light = 4000;		// Anh sang hien tai
uint32_t current_time = 0;			// Thoi gian choi
uint8_t flag_user = FLAG_NOTSTART;	// Nguoi choi co dang choi khong

// Cac du lieu de nhap ten
uint8_t flag_change = 0;
uint8_t flag_uppercase = 0;
uint8_t index_r = 0;
uint8_t index_c = 0;
uint8_t index_r_old = 0;
uint8_t index_c_old = 0;

char char_upper[CHAR_ROW][CHAR_COL] = {
		{'A', 'B', 'C', 'D', 'E', 'F'},
		{'G', 'H', 'I', 'J', 'K', 'L'},
		{'M', 'N', 'O', 'P', 'Q', 'R'},
		{'S', 'T', 'U', 'V', 'W', 'X'},
		{'^', '.', '<', 'Y', 'Z', '>'}
};

char char_lower[CHAR_ROW][CHAR_COL] = {
		{'a', 'b', 'c', 'd', 'e', 'f'},
		{'g', 'h', 'i', 'j', 'k', 'l'},
		{'m', 'n', 'o', 'p', 'q', 'r'},
		{'s', 't', 'u', 'v', 'w', 'x'},
		{'^', ' ', '<', 'y', 'z', '>'}
};

int status = HOME;
int move = 0;
uint32_t length = 1;
uint32_t counter_time_score = 0;
int counter_time = 0;

int tempFlag = 0;
int tempStatus = 0;
int tempMove = 0;

struct Point snake[MAX_LENGTH_SNAKE];
struct Point food = {50, 50};
