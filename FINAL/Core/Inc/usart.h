/*
 * usart.h
 *
 *  Created on: Nov 18, 2023
 *      Author: ADMIN
 */

#ifndef INC_USART_H_
#define INC_USART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_USART_H_ */
