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
//			for(uint8_t i = 0; i <= name_length; i++){
//				uart_EspSendBytes(name[i], 1);
//			}
		}
	}
	else {
		if (temp_user == 1){
			uart_EspSendBytes("a", 1);
			temp_user = 0;
		}
	}
}

uint8_t li1 = 0;
uint8_t li2 = 0;

float tp;
uint8_t tp1 = 0;
uint8_t tp2 = 0;

void sensor_esp(){

		uart_EspSendBytes("L", 1);
		li1 = current_light / 100;
		li2 = current_light % 100;
		uart_EspSendBytes(&li1, 1);
		uart_EspSendBytes(&li2, 1);
		uart_EspSendBytes(&current_hum, 1);
		tp = current_temp;
		tp = tp * 100;
		tp1 = tp / 100;
		tp2 = (int)tp % 100;
		uart_EspSendBytes(&tp1, 1);
		uart_EspSendBytes(&tp2, 1);
//	}
}

void process_esp(){
	if (++counter_sensor % 100 == 0 ){
			counter_sensor = 0;
	user_esp();
	sensor_esp();
	}
}





