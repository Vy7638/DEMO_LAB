/*
 * buzzer.h
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "tim.h"

void buzzer_init();

void buzzer_SetVolume(uint8_t _duty_cycle);

#endif /* INC_BUZZER_H_ */
