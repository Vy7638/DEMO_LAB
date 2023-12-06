/*
 * button.h
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"
#include "spi.h"

extern uint16_t button_count[16];

void button_init();
void button_Scan();

#endif /* INC_BUTTON_H_ */
