/*
 * esp_process.c
 *
 *  Created on: Dec 30, 2023
 *      Author: ADMIN
 */

#include "esp_process.h"

uint32_t counter_sensor = 0;
uint8_t temp_user = 0;
uint8_t light = 0;

void user_esp(){
	if (flag_user == FLAG_PLAYING){
		if (temp_user == 0) {
			uart_EspSendBytes("A", 1);
			temp_user = 1;
		}
	}
	else {
		if (temp_user == 1){
			uart_EspSendBytes("a", 1);
			temp_user = 0;
		}
	}

//	if (button_count[13] == 1){
//		light = 1 - light;
//		if(light == 1){
//			uart_EspSendBytes("A", 1);
//		} else {
//			uart_EspSendBytes("a", 1);
//		}
//	}
//	if(light == 1){
//		HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 1);
//	} else {
//		HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, 0);
//	}
}

void sensor_esp(){
	if (++counter_sensor % 100 == 0 ){
		counter_sensor = 0;
		uart_EspSendBytes("L", 1);
		uart_EspSendBytes(current_light, 4);
		uart_EspSendBytes(current_hum, 2);
		//uart_EspSendBytes(current_temp, 1);
	}
}

void process_esp(){
	user_esp();
	sensor_esp();
}
