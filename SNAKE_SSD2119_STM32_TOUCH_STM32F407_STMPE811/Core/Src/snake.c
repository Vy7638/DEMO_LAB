/*
 * snake.c
 *
 *  Created on: 15-11-2012
 *      Author: Arek
 */

//#include "stm32f4xx.h"
#include "snake.h"
//#include "systick.h"
#include "ssd2119_16bit_fsmc.h"
#include "stm32f4xx_hal.h"
#include "fonts.h"
//#include "main.h"
//#include "string.h"
//#include "debug.h"
char buf[20];
#define SPEED_VALUE_MS 1000
#define SNAKE_LIMIT 100
#define SNAKE_INIT_SIZE 5
#define SPEED_INCREASE_STEPS_CNT 1
#define SPEED_LIMIT 10
#define LCD_VERTICAL_MAX   240
#define LCD_HORIZONTAL_MAX 320
#define LCD_COLOR_WHITE          0xFFFF
#define LCD_COLOR_BLACK          0x0000
#define LCD_COLOR_GREY           0xF7DE
#define LCD_COLOR_BLUE           0x001F
#define LCD_COLOR_BLUE2          0x051F
#define LCD_COLOR_RED            0xF800
#define LCD_COLOR_MAGENTA        0xF81F
#define LCD_COLOR_GREEN          0x07E0
#define LCD_COLOR_CYAN           0x7FFF
#define LCD_COLOR_YELLOW         0xFFE0

#define SNAKE_COLOR_BODY LCD_COLOR_GREEN
#define SNAKE_COLOR_HEAD LCD_COLOR_YELLOW
#define TARGET_COLOR LCD_COLOR_RED
#define BACKGROUND_COLOR LCD_COLOR_WHITE
#define FRAME_COLOR LCD_COLOR_BLUE
#define TEXT_COLOR LCD_COLOR_BLUE
#define HEADER_TEXT_COLOR LCD_COLOR_BLACK

#define DISP_VER_RESOLUTION LCD_VERTICAL_MAX
#define DISP_HOR_RESOLUTION LCD_HORIZONTAL_MAX

#define MOVE_MARGIN 15
#define SNAKE_START_X_POSITION 145
#define SNAKE_START_Y_POSITION 145
#define SNAKE_STEP 10

#define TARGET_X_RATIO 30 // (LCD_HORIZONTAL_MAX - 20) / 10
#define TARGET_Y_RATIO 20 // (LCD_VERTICAL_MAX - HEADER_SIZE - 20) / 10

#define HEADER_SIZE 20	// size of title and points header
#define HEADER_TEXT_MARGIN 5	// number of pixels of text top margin in header

enum
{
    RET_OK,
    RET_NO_MOVE,
    RET_GAME_OVER,
};


typedef struct pos
{
	uint16_t x;
	uint16_t y;
}pos_t;

static pos_t g_snake[SNAKE_LIMIT];
static uint8_t g_snakeSize = SNAKE_INIT_SIZE;

typedef struct
{
	uint16_t x;
	uint16_t y;
    bool new;
} target_t;
target_t target =
        {
            .x = 105,
            .y = 55,
            .new = false,
        };

enum
{
    left,
    right,
    up,
    down,
    directionsCnt,
};

static uint32_t g_stepCountDown;
static uint8_t g_direction = up, g_timeH = 0, g_timeM = 0, g_timeS = 0;
static uint16_t g_snakeSpeed = SPEED_VALUE_MS, g_points = 0;
static uint8_t text[50];

static void snakeGrow(void);
static uint8_t moveSnake(uint8_t direction);
static void printDirections(void);

void printFrame(void)
{
	LCD_Clear(BACKGROUND_COLOR);
	LCD_SetTextColor(FRAME_COLOR);

	#define FRAME_SIZE 4
	LCD_DrawFullRectFill(0, HEADER_SIZE, DISP_HOR_RESOLUTION, FRAME_SIZE, true);
	LCD_DrawFullRectFill(0, HEADER_SIZE, FRAME_SIZE, DISP_VER_RESOLUTION - HEADER_SIZE, true);
	LCD_DrawFullRectFill(0, DISP_VER_RESOLUTION - FRAME_SIZE, DISP_HOR_RESOLUTION, FRAME_SIZE, true);
	LCD_DrawFullRectFill(DISP_HOR_RESOLUTION - FRAME_SIZE, HEADER_SIZE, FRAME_SIZE, DISP_VER_RESOLUTION - HEADER_SIZE, true);
}

void initSnake(void)
{
	uint8_t part;
    g_snakeSize = SNAKE_INIT_SIZE;
    g_snakeSpeed = SPEED_VALUE_MS;
    g_direction = up;
    g_timeH = g_timeM = g_timeS = g_points = 0;

    g_snake[0].x = SNAKE_START_X_POSITION;
    g_snake[0].y = SNAKE_START_Y_POSITION;

    LCD_SetColors(SNAKE_COLOR_BODY, SNAKE_COLOR_HEAD);
    LCD_DrawFullCircle(g_snake[0].x, g_snake[0].y, 5);
    for (part = 1; part < g_snakeSize; part++)
    {
        g_snake[part].x = SNAKE_START_X_POSITION + (part * SNAKE_STEP);
        g_snake[part].y = SNAKE_START_Y_POSITION;
        LCD_DrawFullCircleFill(g_snake[part].x, g_snake[part].y, 5, true);
    }

    // print target
    LCD_SetColors(TARGET_COLOR, BACKGROUND_COLOR);
    LCD_DrawFullCircleFill(target.x, target.y, 3, true);

    // print header

	LCD_SetColors(HEADER_TEXT_COLOR, BACKGROUND_COLOR);

	LCD_SetFont(&Font8x12);
//	LCD_SetColors(HEADER_TEXT_COLOR, BACKGROUND_COLOR);

//	ILI9341_WriteString(HEADER_TEXT_MARGIN,COLUMN(25), (uint8_t *)"Points:  0", Font_8x12, HEADER_TEXT_COLOR, BACKGROUND_COLOR);
//	ILI9341_WriteString(HEADER_TEXT_MARGIN,COLUMN(6), (uint8_t *)"Time 0:00", Font_8x12, HEADER_TEXT_COLOR, BACKGROUND_COLOR);

	LCD_DisplayString(HEADER_TEXT_MARGIN,COLUMN(25),(uint8_t *)"Points:  0");
	LCD_DisplayString(HEADER_TEXT_MARGIN,COLUMN(6),(uint8_t *)"Time 0:00");

//	LCD_DisplayString(0,0,(uint8_t *)"Points:  0");
//	LCD_DisplayString(0,120,(uint8_t *)"Time 0:00");
//	LCD_TextFont(HEADER_TEXT_MARGIN,COLUMN(25),"Points:  0",Black,White, font_8x16);
//	LCD_TextFont(HEADER_TEXT_MARGIN,COLUMN(6),"Time 0:00",Black,White, font_8x16);

//	LCD_DisplayString(HEADER_TEXT_MARGIN,COLUMN(25),(uint8_t *)"Points:  0");
//	LCD_DisplayString(HEADER_TEXT_MARGIN,COLUMN(6),(uint8_t *)"Time 0:00");
}

static uint8_t moveSnake(uint8_t direction)
{
	uint8_t part, partsCount = g_snakeSize, ret_val = RET_OK;
    static uint8_t speedIncreaseCnt = SPEED_INCREASE_STEPS_CNT;

    if (direction >= directionsCnt)
        return RET_NO_MOVE;

    // remove last part of snake
    LCD_SetTextColor(BACKGROUND_COLOR);
    LCD_DrawFullRectFill(g_snake[partsCount-1].x - 5, g_snake[partsCount-1].y - 5, 11, 11, true);

    for (part = partsCount-1; part > 0; part--)
    {
        g_snake[part].x = g_snake[part-1].x;
        g_snake[part].y = g_snake[part-1].y;
    }
    // redraw new last part of snake
    LCD_SetColors(SNAKE_COLOR_BODY, SNAKE_COLOR_HEAD);
    LCD_DrawFullCircleFill(g_snake[partsCount-1].x, g_snake[partsCount-1].y, 5, true);

    switch (direction)
    {
    case left:
        if (MOVE_MARGIN >= g_snake[0].x)
            ret_val = RET_GAME_OVER; //snake[0].x = 790;

        g_snake[0].x -= SNAKE_STEP;
        break;
    case right:
        if((DISP_HOR_RESOLUTION - MOVE_MARGIN) <= g_snake[0].x)
            ret_val = RET_GAME_OVER; //snake[0].x = 0;

        g_snake[0].x += SNAKE_STEP;
        break;
    case up:
        if ((MOVE_MARGIN + HEADER_SIZE) >= g_snake[0].y)
            ret_val = RET_GAME_OVER; //snake[0].y = 470;

        g_snake[0].y -= SNAKE_STEP;
        break;
    case down:
        if ((DISP_VER_RESOLUTION - MOVE_MARGIN) <= g_snake[0].y)
            ret_val = RET_GAME_OVER; //snake[0].y = 0;

        g_snake[0].y += SNAKE_STEP;
        break;
    }

    for (part = partsCount-1; part > 0; part--)
    {
        if ((g_snake[0].x == g_snake[part].x) &&
            (g_snake[0].y == g_snake[part].y))
        {
            ret_val = RET_GAME_OVER;
        }
    }

    // print head on new position
    LCD_SetColors(SNAKE_COLOR_BODY, SNAKE_COLOR_HEAD);
    LCD_DrawFullCircle(g_snake[0].x, g_snake[0].y, 5);
    LCD_DrawFullCircleFill(g_snake[1].x, g_snake[1].y, 5, true);
    if ( (target.x == g_snake[0].x)
       &&(target.y == g_snake[0].y) )
    {
        target.new = true;
        g_points++;
        snakeGrow();
        if (0 == --speedIncreaseCnt)
        {
            speedIncreaseCnt = SPEED_INCREASE_STEPS_CNT;
            g_snakeSpeed -= g_snakeSpeed>>2;
            if (SPEED_LIMIT > g_snakeSpeed)
                g_snakeSpeed = SPEED_LIMIT;
        }
    }
//    sprintf(buf,"snake x=%d, y=%d\r\n", g_snake[0].x, g_snake[0].y);
    return ret_val;
}

static void snakeGrow(void)
{
    if (SNAKE_LIMIT == g_snakeSize)
        return;

    g_snake[g_snakeSize].x = g_snake[g_snakeSize-1].x;
    g_snake[g_snakeSize].y = g_snake[g_snakeSize-1].y;
    g_snakeSize++;
}

bool snakeTask(void)
{
	uint8_t part;
	static uint8_t old_sec = 0;
	static uint16_t old_points = 0;
	uint8_t sec = HAL_GetTick() / 1000;

	if (old_sec < sec)
	{
		g_timeS++;
		old_sec = sec;
		if (60 == g_timeS)
		{
			g_timeS = 0;
			g_timeM++;
		}
		if (60 == g_timeM)
		{
			g_timeM = 0;
			g_timeH++;
		}

//		LCD_SetFont(&Font8x12);
//		LCD_SetColors(HEADER_TEXT_COLOR, BACKGROUND_COLOR);
		if (g_timeH)
			sprintf(buf,"Time %d:%02d:%02d", g_timeH, g_timeM, g_timeS);
		else
			sprintf(buf,"Time %d:%02d", g_timeM, g_timeS);

		LCD_DisplayString(HEADER_TEXT_MARGIN,COLUMN(6),buf);

//		LCD_TextFont(HEADER_TEXT_MARGIN,COLUMN(6),buf,HEADER_TEXT_COLOR,BACKGROUND_COLOR, font_8x16);
	}

	if (old_points < g_points)
	{
		old_points = g_points;
//		LCD_SetFont(&Font8x12);
//		LCD_SetColors(HEADER_TEXT_COLOR, BACKGROUND_COLOR);
		sprintf(buf,"Points:%3d", g_points);
		LCD_DisplayString(HEADER_TEXT_MARGIN,COLUMN(25),buf);
//		LCD_TextFont(HEADER_TEXT_MARGIN,COLUMN(25),buf,HEADER_TEXT_COLOR,BACKGROUND_COLOR, font_8x16);

	}

    if(g_stepCountDown < HAL_GetTick())
    {
        g_stepCountDown = HAL_GetTick() + g_snakeSpeed;
        if (RET_GAME_OVER == moveSnake(g_direction))
        {
//        	LCD_SetFont(&Font16x24);
        	LCD_SetColors(TEXT_COLOR, BACKGROUND_COLOR);
//            LCD_DisplayString(LINE(5), COLUMN(6), (uint8_t *)"GAME OVER");
//            LCD_TextFont(LINE(5), COLUMN(6),buf,TEXT_COLOR,BACKGROUND_COLOR, font_8x16);
            HAL_Delay(1000);
            return false;
        }
    }

    if (target.new)
    {
        target.x = (HAL_GetTick()%TARGET_X_RATIO + 1) * 10 + 5;
        target.y = (HAL_GetTick()%TARGET_Y_RATIO + 1) * 10 + 5 + HEADER_SIZE;
        for (part = 0; part < g_snakeSize; part++)
        {
            if ((target.x == g_snake[part].x) &&
                (target.y == g_snake[part].y))
            {
            	// target cannot be on the snake, place the new one
                return true;
            }
        }
        LCD_SetColors(TARGET_COLOR, BACKGROUND_COLOR);
        LCD_DrawFullCircleFill(target.x, target.y, 3, true);
        target.new = false;
    }
    return true;
}

void calcDirection(uint16_t x, uint16_t y)
{
//#define MARGIN 10
//    if ((x < MARGIN) ||
//        (x > (DISP_HOR_RESOLUTION - MARGIN)) ||
//        (y < MARGIN) ||
//        (y > (DISP_VER_RESOLUTION - MARGIN)))
//    {
//        return;
//    }


	  if(x >=2680 && x<=3900 && y>=1440 && y<=2700)
	  {
		  g_direction = left;
	  }

	  if(x >=200 && x<=1370 && y>=1440 && y<=2670)
	  {
		  g_direction = right;
	  }

	  if(x >=1430 && x<=2700 && y>=280 && y<=1400)
	  {
		  g_direction = up;
	  }

	  if(x >=1400 && x<=2700 && y>=2700 && y<=3750)
	  {
		  g_direction = down;
	  }

//    // position calculation
//    if(y < ((HEADER_SIZE + DISP_VER_RESOLUTION)>>1))
//    {
//        // upper position
//        if (x + HEADER_SIZE< y)
//        {
//            // left position
//            g_direction = left;
////            my_printf("left\r\n");
//        }
//        else if ((DISP_HOR_RESOLUTION - x + HEADER_SIZE) < y)
//        {
//            // right position
//            g_direction = right;
////            my_printf("right\r\n");
//        }
//        else
//        {
//            // middle position
//            g_direction = up;
////            my_printf("up\r\n");
//        }
//    }
//    else
//    {
//        // lower position
//        if ((DISP_VER_RESOLUTION - y) > x)
//        {
//            // left position
//            g_direction = left;
////            my_printf("left\r\n");
//        }
//        else if ((DISP_VER_RESOLUTION - y) > (DISP_HOR_RESOLUTION - x))
//        {
//            // right position
//            g_direction = right;
////            my_printf("right\r\n");
//        }
//        else
//        {
//            // middle position
//            g_direction = down;
////            my_printf("down\r\n");
//        }
//    }
}

void snakeOptions(void)
{
	printDirections();
	HAL_Delay(1000);
}

void printDirections(void)
{
	uint16_t x, y, color;

    for (y = 0; y < DISP_VER_RESOLUTION; y++)
    {
    	LCD_SetCursor(0, y);
    	LCD_WriteRAM_Prepare();
    	for (x = 0; x < DISP_HOR_RESOLUTION; x++)
    	{
			// position calculation
			if(y < ((HEADER_SIZE + DISP_VER_RESOLUTION)>>1))
			{
				// upper position
				if (x + HEADER_SIZE< y)
				{
					// left position
					color = Red;
				}
				else if ((DISP_HOR_RESOLUTION - x + HEADER_SIZE) < y)
				{
					// right position
					color = Green;
				}
				else
				{
					// middle position
					color = Blue;
				}
			}
			else
			{
				// lower position
				if ((DISP_VER_RESOLUTION - y) > x)
				{
					// left position
					color = Red;
				}
				else if ((DISP_VER_RESOLUTION - y) > (DISP_HOR_RESOLUTION - x))
				{
					// right position
					color = Green;
				}
				else
				{
					// middle position
					color = Black;
				}
			}
	    	LCD_WriteRAM(color);
    	}
    }
	LCD_SetFont(&Font8x12);
	LCD_SetColors(White, Blue);
//    LCD_TextFont(HEADER_TEXT_MARGIN, COLUMN(6),"Touch screen snake control",White,Blue, font_8x16);

//    LCD_TextFont(LINE(2), COLUMN(9),(uint8_t *)"UP",White,Blue, font_8x16);

//    LCD_TextFont(LINE(5), COLUMN(1),(uint8_t *)"LEFT",White,Blue, font_8x16);

//    LCD_TextFont(LINE(5), COLUMN(14), (uint8_t *)"RIGHT",White,Blue, font_8x16);

//    LCD_TextFont(LINE(8), COLUMN(8),(uint8_t *)"DOWN",White,Blue, font_8x16);
    LCD_DisplayString(HEADER_TEXT_MARGIN, COLUMN(6), (uint8_t *)"Touch screen snake control");
	LCD_SetFont(&Font16x24);
    LCD_DisplayString(LINE(2), COLUMN(9), (uint8_t *)"UP");
    LCD_SetBackColor(Red);
    LCD_DisplayString(LINE(5), COLUMN(1), (uint8_t *)"LEFT");
    LCD_SetBackColor(Green);
    LCD_DisplayString(LINE(5), COLUMN(14), (uint8_t *)"RIGHT");
    LCD_SetBackColor(Black);
    LCD_DisplayString(LINE(8), COLUMN(8), (uint8_t *)"DOWN");
}


