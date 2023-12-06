/*
 * adc.h
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern ADC_HandleTypeDef hadc1;

void MX_ADC1_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_ADC_H_ */
