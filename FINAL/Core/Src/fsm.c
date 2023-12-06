/*
 * fsm.c
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#include "fsm.h"

void input_process(){

}

void fsm(){
	switch (status) {
		case HOME:
			home_lcd();
			break;
		case MODE:
			mode_game_lcd();
			break;
		case HIGHSCORE:
			highscore_lcd();
			break;
		case CLASSIC:
			game_lcd();
			break;
		case SPEED:
			game_lcd();
			break;
		case TIME:
			game_lcd();
			break;
		case GAMEOVER:
			game_over_lcd();
			break;
		default:
			break;
	}
}

