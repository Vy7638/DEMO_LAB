/*
 * user.h
 *
 *  Created on: Dec 28, 2023
 *      Author: Duyen
 */

#ifndef INC_NAME_H_
#define INC_NAME_H_

#include "main.h"

#define CHAR_ROW	5		// So hang bang nhap chu
#define CHAR_COL	6		// So cot bang nhap chu

#define POS_X_CHAR	6		// Toa do x goc tren ben trai o dau tien nhap chu
#define POS_Y_CHAR	124		// Toa do y goc tren ben trai o dau tien nhap chu
#define CHAR_WIDTH	38		// Chieu rong o nhap chu
#define CHAR_HEIGHT	38		// Chieu dai o nhap chu

extern uint8_t flag_uppercase;
extern uint8_t index_x;
extern uint8_t index_y;
extern uint8_t index_x_old;
extern uint8_t index_y_old;
extern char char_upper[CHAR_ROW][CHAR_COL];
extern char char_lower[CHAR_ROW][CHAR_COL];

#endif /* INC_NAME_H_ */
