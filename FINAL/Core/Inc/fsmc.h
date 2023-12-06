/*
 * fsmc.h
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#ifndef INC_FSMC_H_
#define INC_FSMC_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

extern SRAM_HandleTypeDef hsram1;

void MX_FSMC_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_FSMC_H_ */
