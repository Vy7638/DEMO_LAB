/*
 * interface.c
 *
 *  Created on: Dec 6, 2023
 *      Author: ADMIN
 */

#include "interface.h"

void home_lcd(){
	lcd_ShowPicture(0, 0, 240, 320, gImage_home);
	lcd_Fill(POS_X_SECOND, POS_Y_SECOND,
			POS_X_SECOND + WIDTH, POS_Y_SECOND + HEIGHT, BROWN);
	lcd_Fill(POS_X_THIRD, POS_Y_THIRD,
			POS_X_THIRD + WIDTH, POS_Y_THIRD + HEIGHT, BROWN);
	lcd_Fill(POS_X_FORTH, POS_Y_FORTH,
			POS_X_FORTH + WIDTH, POS_Y_FORTH + HEIGHT, BROWN);
	lcd_ShowStr(POS_X_SECOND + 19, POS_Y_SECOND + 5, "NEWGAME", BLACK, BLACK, 24, 1);
	lcd_ShowStr(POS_X_THIRD + 12, POS_Y_THIRD + 5, "CONTINUE", BLACK, BLACK, 24, 1);
	lcd_ShowStr(POS_X_FORTH + 7, POS_Y_FORTH + 5, "HIGHSCORE", BLACK, BLACK, 24, 1);
}

void mode_game_lcd(){
	lcd_ShowPicture(0, 0, 240, 320, gImage_mode);
	lcd_Fill(POS_X_FIRST, POS_Y_FIRST,
				 POS_X_FIRST + WIDTH, POS_Y_FIRST + HEIGHT, BROWN);
	lcd_Fill(POS_X_SECOND, POS_Y_SECOND,
				 POS_X_SECOND + WIDTH, POS_Y_SECOND + HEIGHT, BROWN);
	lcd_Fill(POS_X_THIRD, POS_Y_THIRD,
				 POS_X_THIRD + WIDTH, POS_Y_THIRD + HEIGHT, BROWN);
	lcd_Fill(POS_X_FORTH, POS_Y_FORTH,
					 POS_X_FORTH + WIDTH, POS_Y_FORTH + HEIGHT, BROWN);
	lcd_ShowStr(POS_X_FIRST + 19, POS_Y_FIRST + 5, "CLASSIC", BLACK, BLACK, 24, 1);
	lcd_ShowStr(POS_X_SECOND + 31, POS_Y_SECOND + 5, "SPEED", BLACK, BLACK, 24, 1);
	lcd_ShowStr(POS_X_THIRD + 31, POS_Y_THIRD + 5, "TIMER", BLACK, BLACK, 24, 1);
	lcd_ShowStr(POS_X_FORTH + 35, POS_Y_FORTH + 5, "QUIT", BLACK, BLACK, 24, 1);
}

void highscore_lcd(){
	lcd_ShowPicture(0, 0, 240, 320, gImage_highscore);
	lcd_ShowStr(POS_X_RANK, POS_Y_RANK, "RANK", BLACK, BLACK, 16, 1);
	lcd_ShowStr(POS_X_NAME, POS_Y_RANK, "NAME", BLACK, BLACK, 16, 1);
	lcd_ShowStr(POS_X_SCORE, POS_Y_RANK, "SCORE", BLACK, BLACK, 16, 1);
	char c[10] = "";
	for(uint8_t i = 1; i <= num_history; i++){
		lcd_ShowStr(POS_X_RANK, POS_Y_RANK + i * SPACE, itoa(i, c, 10), BLACK, BLACK, 16, 1);
		lcd_ShowStr(POS_X_NAME, POS_Y_RANK + i * SPACE, history[i - 1].name, BLACK, BLACK, 16, 1);
		lcd_ShowStr(POS_X_SCORE, POS_Y_RANK + i * SPACE, itoa(history[i - 1].score, c, 10), BLACK, BLACK, 16, 1);
	}
	lcd_Fill(POS_X_FORTH, POS_Y_FORTH,
			POS_X_FORTH + WIDTH, POS_Y_FORTH + HEIGHT, BROWN);
	lcd_ShowStr(POS_X_FORTH + 35, POS_Y_FORTH + 5, "QUIT", BLACK, BLACK, 24, 1);
	lcd_DrawRectangle(POS_X_FORTH, POS_Y_FORTH,
			POS_X_FORTH + WIDTH, POS_Y_FORTH + HEIGHT, BLACK);
	lcd_DrawRectangle(POS_X_FORTH + 1, POS_Y_FORTH + 1,
			POS_X_FORTH + WIDTH - 1, POS_Y_FORTH + HEIGHT - 1, BLACK);
}

void game_lcd(){
	lcd_Clear(WHITE);
	lcd_Fill(0, 200, 239, 319, LGRAYBLUE);
	lcd_Fill(POS_X_LEFT, POS_Y_LEFT,
			POS_X_LEFT + BTN_WIDTH, POS_Y_LEFT + BTN_HEIGHT, BROWN);
	lcd_Fill(POS_X_RIGHT, POS_Y_RIGHT,
			POS_X_RIGHT + BTN_WIDTH, POS_Y_RIGHT + BTN_HEIGHT, BROWN);
	lcd_Fill(POS_X_UP, POS_Y_UP,
			POS_X_UP + BTN_WIDTH, POS_Y_UP + BTN_HEIGHT, BROWN);
	lcd_Fill(POS_X_DOWN, POS_Y_DOWN,
			POS_X_DOWN + BTN_WIDTH, POS_Y_DOWN + BTN_HEIGHT, BROWN);
	lcd_DrawCircle(POS_X_PAUSE, POS_Y_PAUSE, RED, BTN_HEIGHT/2, 1);
	lcd_ShowStr(POS_X_PAUSE - 18, POS_Y_PAUSE - 8, "PAUSE", BLACK, BLACK, 16, 1);
	lcd_ShowStr(POS_X_CURSCORE, POS_Y_CURSCORE, "SCORE:", BLACK, BLACK, 16, 1);
}

void score_lcd(){
	lcd_ShowIntNum(POS_X_CURSCORE, POS_Y_CURSCORE + 16, current_score, 5, BLACK, LGRAYBLUE, 16);
}

void pause_lcd(){
	lcd_ShowPicture(0, 0, 240, 320, gImage_pause);
	lcd_Fill(POS_X_SECOND, POS_Y_SECOND,
			POS_X_SECOND + WIDTH, POS_Y_SECOND + HEIGHT, BROWN);
	lcd_Fill(POS_X_THIRD, POS_Y_THIRD,
			POS_X_THIRD + WIDTH, POS_Y_THIRD + HEIGHT, BROWN);
	lcd_ShowStr(POS_X_SECOND + 25, POS_Y_SECOND + 5, "RESUME", BLACK, BLACK, 24, 1);
	lcd_ShowStr(POS_X_THIRD + 35, POS_Y_THIRD + 5, "QUIT", BLACK, BLACK, 24, 1);
}

void game_over_lcd(){
	lcd_ShowPicture(0, 0, 240, 320, gImage_gameover);
	lcd_Fill(POS_X_THIRD, POS_Y_THIRD,
			POS_X_THIRD + WIDTH, POS_Y_THIRD + HEIGHT, BROWN);
	lcd_DrawRectangle(POS_X_THIRD, POS_Y_THIRD,
			POS_X_THIRD + WIDTH, POS_Y_THIRD + HEIGHT, BLACK);
	lcd_DrawRectangle(POS_X_THIRD + 1, POS_Y_THIRD + 1,
			POS_X_THIRD + WIDTH - 1, POS_Y_THIRD + HEIGHT - 1, BLACK);
	lcd_ShowStr(POS_X_THIRD + 35, POS_Y_THIRD + 5, "QUIT", BLACK, BLACK, 24, 1);
}

void youwin_lcd(){
	lcd_ShowPicture(0, 0, 240, 320, gImage_youwin);
	lcd_Fill(POS_X_THIRD, POS_Y_THIRD,
			POS_X_THIRD + WIDTH, POS_Y_THIRD + HEIGHT, BROWN);
	lcd_DrawRectangle(POS_X_THIRD, POS_Y_THIRD,
			POS_X_THIRD + WIDTH, POS_Y_THIRD + HEIGHT, BLACK);
	lcd_DrawRectangle(POS_X_THIRD + 1, POS_Y_THIRD + 1,
			POS_X_THIRD + WIDTH - 1, POS_Y_THIRD + HEIGHT - 1, BLACK);
	lcd_ShowStr(POS_X_THIRD + 35, POS_Y_THIRD + 5, "QUIT", BLACK, BLACK, 24, 1);
}

void home_not_select(){
	lcd_DrawRectangle(POS_X_SECOND, POS_Y_SECOND,
			POS_X_SECOND + WIDTH, POS_Y_SECOND + HEIGHT, GRAY);
	lcd_DrawRectangle(POS_X_SECOND + 1, POS_Y_SECOND + 1,
			POS_X_SECOND + WIDTH - 1, POS_Y_SECOND + HEIGHT - 1, GRAY);
	lcd_DrawRectangle(POS_X_THIRD, POS_Y_THIRD,
			POS_X_THIRD + WIDTH, POS_Y_THIRD + HEIGHT, GRAY);
	lcd_DrawRectangle(POS_X_THIRD + 1, POS_Y_THIRD + 1,
			POS_X_THIRD + WIDTH - 1, POS_Y_THIRD + HEIGHT - 1, GRAY);
	lcd_DrawRectangle(POS_X_FORTH, POS_Y_FORTH,
			POS_X_FORTH + WIDTH, POS_Y_FORTH + HEIGHT, GRAY);
	lcd_DrawRectangle(POS_X_FORTH + 1, POS_Y_FORTH + 1,
			POS_X_FORTH + WIDTH - 1, POS_Y_FORTH + HEIGHT - 1, GRAY);
}

void home_second_lcd(){
	home_not_select();
	lcd_DrawRectangle(POS_X_SECOND, POS_Y_SECOND,
			POS_X_SECOND + WIDTH, POS_Y_SECOND + HEIGHT, BLACK);
	lcd_DrawRectangle(POS_X_SECOND + 1, POS_Y_SECOND + 1,
			POS_X_SECOND + WIDTH - 1, POS_Y_SECOND + HEIGHT - 1, BLACK);
}

void home_third_lcd(){
	home_not_select();
	lcd_DrawRectangle(POS_X_THIRD, POS_Y_THIRD,
			POS_X_THIRD + WIDTH, POS_Y_THIRD + HEIGHT, BLACK);
	lcd_DrawRectangle(POS_X_THIRD + 1, POS_Y_THIRD + 1,
			POS_X_THIRD + WIDTH - 1, POS_Y_THIRD + HEIGHT - 1, BLACK);
}

void home_forth_lcd(){
	home_not_select();
	lcd_DrawRectangle(POS_X_FORTH, POS_Y_FORTH,
			POS_X_FORTH + WIDTH, POS_Y_FORTH + HEIGHT, BLACK);
	lcd_DrawRectangle(POS_X_FORTH + 1, POS_Y_FORTH + 1,
			POS_X_FORTH + WIDTH - 1, POS_Y_FORTH + HEIGHT - 1, BLACK);
}

void mode_first_lcd(){
	lcd_DrawRectangle(POS_X_FIRST, POS_Y_FIRST,
			POS_X_FIRST + WIDTH, POS_Y_FIRST + HEIGHT, BLACK);
	lcd_DrawRectangle(POS_X_FIRST + 1, POS_Y_FIRST + 1,
			POS_X_FIRST + WIDTH - 1, POS_Y_FIRST + HEIGHT - 1, BLACK);
	home_not_select();
}

void mode_second_lcd(){
	lcd_DrawRectangle(POS_X_FIRST, POS_Y_FIRST,
			POS_X_FIRST + WIDTH, POS_Y_FIRST + HEIGHT, GRAY);
	lcd_DrawRectangle(POS_X_FIRST + 1, POS_Y_FIRST + 1,
			POS_X_FIRST + WIDTH - 1, POS_Y_FIRST + HEIGHT - 1, GRAY);
	home_second_lcd();
}

void mode_third_lcd(){
	lcd_DrawRectangle(POS_X_FIRST, POS_Y_FIRST,
			POS_X_FIRST + WIDTH, POS_Y_FIRST + HEIGHT, GRAY);
	lcd_DrawRectangle(POS_X_FIRST + 1, POS_Y_FIRST + 1,
			POS_X_FIRST + WIDTH - 1, POS_Y_FIRST + HEIGHT - 1, GRAY);
	home_third_lcd();
}

void mode_forth_lcd(){
	lcd_DrawRectangle(POS_X_FIRST, POS_Y_FIRST,
			POS_X_FIRST + WIDTH, POS_Y_FIRST + HEIGHT, GRAY);
	lcd_DrawRectangle(POS_X_FIRST + 1, POS_Y_FIRST + 1,
			POS_X_FIRST + WIDTH - 1, POS_Y_FIRST + HEIGHT - 1, GRAY);
	home_forth_lcd();
}

void pause_second_lcd(){
	lcd_DrawRectangle(POS_X_SECOND, POS_Y_SECOND,
			POS_X_SECOND + WIDTH, POS_Y_SECOND + HEIGHT, BLACK);
	lcd_DrawRectangle(POS_X_SECOND + 1, POS_Y_SECOND + 1,
			POS_X_SECOND + WIDTH - 1, POS_Y_SECOND + HEIGHT - 1, BLACK);
	lcd_DrawRectangle(POS_X_THIRD, POS_Y_THIRD,
			POS_X_THIRD + WIDTH, POS_Y_THIRD + HEIGHT, GRAY);
	lcd_DrawRectangle(POS_X_THIRD + 1, POS_Y_THIRD + 1,
			POS_X_THIRD + WIDTH - 1, POS_Y_THIRD + HEIGHT - 1, GRAY);
}

void pause_third_lcd(){
	lcd_DrawRectangle(POS_X_SECOND, POS_Y_SECOND,
			POS_X_SECOND + WIDTH, POS_Y_SECOND + HEIGHT, GRAY);
	lcd_DrawRectangle(POS_X_SECOND + 1, POS_Y_SECOND + 1,
			POS_X_SECOND + WIDTH - 1, POS_Y_SECOND + HEIGHT - 1, GRAY);
	lcd_DrawRectangle(POS_X_THIRD, POS_Y_THIRD,
			POS_X_THIRD + WIDTH, POS_Y_THIRD + HEIGHT, BLACK);
	lcd_DrawRectangle(POS_X_THIRD + 1, POS_Y_THIRD + 1,
			POS_X_THIRD + WIDTH - 1, POS_Y_THIRD + HEIGHT - 1, BLACK);
}

void point_head_lcd(struct Point head){
	lcd_DrawCircle(head.x, head.y, GREEN, RADIUS_SNAKE, 1);
	lcd_DrawCircle(head.x, head.y, BLACK, 1, 1);
}

void point_snake_lcd(struct Point snake){
	lcd_DrawCircle(snake.x, snake.y, GREEN, RADIUS_SNAKE, 1);
	lcd_DrawLine(snake.x - RADIUS_SNAKE/2 - 1, snake.y - RADIUS_SNAKE/2 - 1,
				 snake.x + RADIUS_SNAKE/2 + 2, snake.y + RADIUS_SNAKE/2 + 2, BROWN);
	lcd_DrawLine(snake.x - RADIUS_SNAKE/2 - 1, snake.y - RADIUS_SNAKE/2,
				 snake.x + RADIUS_SNAKE/2 + 1, snake.y + RADIUS_SNAKE/2 + 2, BROWN);
	lcd_DrawLine(snake.x - RADIUS_SNAKE/2,	   snake.y - RADIUS_SNAKE/2 - 1,
				 snake.x + RADIUS_SNAKE/2 + 2, snake.y + RADIUS_SNAKE/2 + 1, BROWN);
}

void point_food_lcd(struct Point food){
	lcd_DrawCircle(food.x, food.y, RED, RADIUS_SNAKE, 1);
}

void point_clr(struct Point clr){
	lcd_DrawCircle(clr.x, clr.y, WHITE, RADIUS_SNAKE, 1);
}
