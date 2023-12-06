/*
 * tim.h
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#ifndef INC_TIM_H_
#define INC_TIM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim13;

void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM13_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_TIM_H_ */
