/*
 * software_timer.c
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#include "software_timer.h"

#define TIMER_CYCLE_2 1

/*uint16_t flag_timer1 = 0;
uint16_t timer1_counter = 0;
uint16_t timer1_MUL = 0;*/

uint16_t flag_timer2 = 0;
uint16_t timer2_counter = 0;
uint16_t timer2_MUL = 0;



void timer_init(){
	HAL_TIM_Base_Start_IT(&htim2);
}

/*void setTimer1(uint16_t duration){
	timer1_MUL = duration / TIMER_CYCLE_2;
	timer1_counter = timer1_MUL;
	flag_timer1 = 0;
}*/

void setTimer2(uint16_t duration){
	timer2_MUL = duration / TIMER_CYCLE_2;
	timer2_counter = timer2_MUL;
	flag_timer2 = 0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM2){
		/*if(timer1_counter > 0){
			timer1_counter--;
			if(timer1_counter == 0) {
				flag_timer1 = 1;
				timer1_counter = timer1_MUL;
			}
		}*/
		if(timer2_counter > 0){
			timer2_counter--;
			if(timer2_counter == 0) {
				flag_timer2 = 1;
				timer2_counter = timer2_MUL;
			}
		}
		// 1ms interrupt here
		led7_Scan();
	}
}
