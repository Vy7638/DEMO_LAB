/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software is delivered "as is" and shows the usage of other software
* components. It is provided as an example software which is intended to be
* modified and extended according to particular requirements.
*
* TARA Systems hereby disclaims all warranties and conditions with regard to the
* software, including all implied warranties and conditions of merchantability
* and non-infringement of any third party IPR or other rights which may result
* from the use or the inability to use the software.
*
********************************************************************************
*
* DESCRIPTION:
*   This file contains the necessary initialization of the SSD2119 display
*   controller and the necessary function to update a rectangular area of the
*   screen from an Embedded Wizard UI application.
*   The data transfer is done by using the hardware FSMC controller via the
*   16bit 8080 interface. The color format of the display is RGB565.
*
*   This driver assumes the following layout of the interface:
*   Data lines D0...D15 (FSMC):
*     D0   - PD14
*     D1   - PD15
*     D2   - PD0
*     D3   - PD1
*     D4   - PE7
*     D5   - PE8
*     D6   - PE9
*     D7   - PE10
*     D8   - PE11
*     D9   - PE12
*     D10  - PE13
*     D11  - PE14
*     D12  - PE15
*     D13  - PD8
*     D14  - PD9
*     D15  - PD10
*   Control lines (FSCM):
*     /CS  - PD7   (FSMC_NE1) => chip select, low active
*     /RD  - PD4   (FSMC_NOE) => read access, low active
*     /WR  - PD5   (FSMC_NWE) => write access, low active
*     DC   - PE3   (A19)      => register select (data/command)
*   Additional control lines:
*     /RST - PD3              => reset of LCD module, low active
*     BL   - PD13             => backlight
*
*   This module is just a template - adapt pin configuration and timing (!)
*   according your hardware layout and hardware constraints.
*   Adjust device parameters, initialization and gamma correction values
*   according your display and your needs.
*   This driver was tested on a STM32F4DIS-LCD (DM-LCD35RT) display module
*   (3,5" TFT 320x240).
*
*******************************************************************************/

#include "stm32f4xx_hal.h"
//#include "fonts.h"
#include "ssd2119_16bit_fsmc.h"
#include "string.h"

/* LCD reset pin */
#define LCD_RESET_PIN                   GPIO_PIN_3
#define LCD_RESET_GPIO_PORT             GPIOD
#define LCD_RESET_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE()
#define LCD_RESET_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOD_CLK_DISABLE()

/* backlight control pin */
#define LCD_BL_CTRL_PIN                 GPIO_PIN_13
#define LCD_BL_CTRL_GPIO_PORT           GPIOD
#define LCD_BL_CTRL_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOD_CLK_ENABLE()
#define LCD_BL_CTRL_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOD_CLK_DISABLE()

/* FSMC addresses for command and data port of SSD2119 */
#define SSD2119_Cmnd (__IO uint16_t*)((uint32_t)0x60000000)
#define SSD2119_Data (__IO uint16_t*)((uint32_t)0x60100000)

/* display specific settings */
#define DISPLAY_PIXEL_WIDTH             ((uint16_t)320)
#define DISPLAY_PIXEL_HEIGHT            ((uint16_t)240)

/* SSD2119 commands (from SSD2119 datasheet) */
#define SSD2119_DEVICE_CODE_READ_REG    0x00
#define SSD2119_OSC_START_REG           0x00
#define SSD2119_OUTPUT_CTRL_REG         0x01
#define SSD2119_LCD_DRIVE_AC_CTRL_REG   0x02
#define SSD2119_PWR_CTRL_1_REG          0x03
#define SSD2119_DISPLAY_CTRL_REG        0x07
#define SSD2119_FRAME_CYCLE_CTRL_REG    0x0B
#define SSD2119_PWR_CTRL_2_REG          0x0C
#define SSD2119_PWR_CTRL_3_REG          0x0D
#define SSD2119_PWR_CTRL_4_REG          0x0E
#define SSD2119_GATE_SCAN_START_REG     0x0F
#define SSD2119_SLEEP_MODE_1_REG        0x10
#define SSD2119_ENTRY_MODE_REG          0x11
#define SSD2119_SLEEP_MODE_2_REG        0x12
#define SSD2119_GEN_IF_CTRL_REG         0x15
#define SSD2119_PWR_CTRL_5_REG          0x1E
#define SSD2119_RAM_DATA_REG            0x22
#define SSD2119_FRAME_FREQ_REG          0x25
#define SSD2119_ANALOG_SET_REG          0x26
#define SSD2119_VCOM_OTP_1_REG          0x28
#define SSD2119_VCOM_OTP_2_REG          0x29
#define SSD2119_GAMMA_CTRL_1_REG        0x30
#define SSD2119_GAMMA_CTRL_2_REG        0x31
#define SSD2119_GAMMA_CTRL_3_REG        0x32
#define SSD2119_GAMMA_CTRL_4_REG        0x33
#define SSD2119_GAMMA_CTRL_5_REG        0x34
#define SSD2119_GAMMA_CTRL_6_REG        0x35
#define SSD2119_GAMMA_CTRL_7_REG        0x36
#define SSD2119_GAMMA_CTRL_8_REG        0x37
#define SSD2119_GAMMA_CTRL_9_REG        0x3A
#define SSD2119_GAMMA_CTRL_10_REG       0x3B
#define SSD2119_V_RAM_POS_REG           0x44
#define SSD2119_H_RAM_START_REG         0x45
#define SSD2119_H_RAM_END_REG           0x46
#define SSD2119_X_RAM_ADDR_REG          0x4E
#define SSD2119_Y_RAM_ADDR_REG          0x4F
static sFONT *LCD_Currentfonts;
#define ENTRY_MODE_DEFAULT              0x6830
static __IO uint16_t TextColor = 0x0000, BackColor = 0xFFFF;
void GetASCIICode(unsigned char* pBuffer, unsigned char ASCII, uint16_t FONTx)
{
	switch (FONTx)
	{
		case font_8x5:
			memcpy(pBuffer, font_8x5_table[(ASCII - 32)], 8);
			break;
		case font_8x16:
		default:
			memcpy(pBuffer, font_8x16_table[(ASCII - 32)], 16);
			break;
	}
}
/*************************** FMC Routines ************************************/
//static void FMC_BANK1_MspInit(void)
//{
//  GPIO_InitTypeDef gpio;
//
//  /* Enable FSMC clock */
//  __HAL_RCC_FSMC_CLK_ENABLE();
//
//  /* Enable GPIOs clock */
//  __HAL_RCC_GPIOD_CLK_ENABLE();
//  __HAL_RCC_GPIOE_CLK_ENABLE();
//
//  /* Common GPIO configuration */
//  gpio.Mode      = GPIO_MODE_AF_PP;
//  gpio.Pull      = GPIO_PULLUP;
//  gpio.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
//  gpio.Alternate = GPIO_AF12_FSMC;
//
//  /* GPIOD configuration */
//  gpio.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5
//                 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
//                 | GPIO_PIN_14 | GPIO_PIN_15;
//  HAL_GPIO_Init( GPIOD, &gpio );
//
//  /* GPIOE configuration */
//  gpio.Pin       = GPIO_PIN_3 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9
//                 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13
//                 | GPIO_PIN_14 | GPIO_PIN_15;
//  HAL_GPIO_Init( GPIOE, &gpio );
//}


//static void FMC_BANK1_Init(void)
//{
//  SRAM_HandleTypeDef hsram;
//  FMC_NORSRAM_TimingTypeDef sram_timing_rd;
//  FMC_NORSRAM_TimingTypeDef sram_timing_wr;
//
//  /* configure IPs */
//  hsram.Instance                       = FSMC_NORSRAM_DEVICE;
//  hsram.Extended                       = FSMC_NORSRAM_EXTENDED_DEVICE;
//
//  /* timing for READING */
//  sram_timing_rd.AddressSetupTime      = 9;
//  sram_timing_rd.AddressHoldTime       = 1;
//  sram_timing_rd.DataSetupTime         = 36;
//  sram_timing_rd.BusTurnAroundDuration = 1;
//  sram_timing_rd.CLKDivision           = 2;
//  sram_timing_rd.DataLatency           = 2;
//  sram_timing_rd.AccessMode            = FSMC_ACCESS_MODE_A;
//
//  /* timing for WRITTING */
//  sram_timing_wr.AddressSetupTime      = 1;
//  sram_timing_wr.AddressHoldTime       = 1;
//  sram_timing_wr.DataSetupTime         = 7;
//  sram_timing_wr.BusTurnAroundDuration = 0;
//  sram_timing_wr.CLKDivision           = 2;
//  sram_timing_wr.DataLatency           = 2;
//  sram_timing_wr.AccessMode            = FSMC_ACCESS_MODE_A;
//
//  hsram.Init.NSBank                    = FSMC_NORSRAM_BANK1;
//  hsram.Init.DataAddressMux            = FSMC_DATA_ADDRESS_MUX_DISABLE;
//  hsram.Init.MemoryType                = FSMC_MEMORY_TYPE_SRAM;
//  hsram.Init.MemoryDataWidth           = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
//  hsram.Init.BurstAccessMode           = FSMC_BURST_ACCESS_MODE_DISABLE;
//  hsram.Init.WaitSignalPolarity        = FSMC_WAIT_SIGNAL_POLARITY_LOW;
//  hsram.Init.WrapMode                  = FSMC_WRAP_MODE_DISABLE;
//  hsram.Init.WaitSignalActive          = FSMC_WAIT_TIMING_BEFORE_WS;
//  hsram.Init.WriteOperation            = FSMC_WRITE_OPERATION_ENABLE;
//  hsram.Init.WaitSignal                = FSMC_WAIT_SIGNAL_DISABLE;
//  hsram.Init.ExtendedMode              = FSMC_EXTENDED_MODE_DISABLE;
//  hsram.Init.AsynchronousWait          = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
//  hsram.Init.WriteBurst                = FSMC_WRITE_BURST_DISABLE;
//  hsram.Init.PageSize                  = FSMC_PAGE_SIZE_NONE;
//  hsram.Init.ContinuousClock           = FSMC_CONTINUOUS_CLOCK_SYNC_ONLY;
//
//  /* initialize the SRAM controller */
//  FMC_BANK1_MspInit();
//  HAL_SRAM_Init( &hsram, &sram_timing_rd, &sram_timing_wr );
//}


/* helper function to initialize reset and backlight pins of LCD and to perform
   proper reset of display */
static void SSD2119_ResetDisplay()
{
  GPIO_InitTypeDef gpio;

  /* enable clock for LCD reset pin */
  LCD_RESET_GPIO_CLK_ENABLE();

  /* configure GPIO for LCD reset pin */
  gpio.Pin   = LCD_RESET_PIN;
  gpio.Pull  = GPIO_NOPULL;
  gpio.Speed = GPIO_SPEED_FAST;
  gpio.Mode  = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init( LCD_RESET_GPIO_PORT, &gpio );

  /* apply hardware reset */
  HAL_GPIO_WritePin( LCD_RESET_GPIO_PORT, LCD_RESET_PIN, GPIO_PIN_RESET );
  HAL_Delay( 5 );   /* reset signal asserted during 5ms  */
  HAL_GPIO_WritePin( LCD_RESET_GPIO_PORT, LCD_RESET_PIN, GPIO_PIN_SET );
  HAL_Delay( 10 );  /* reset signal released during 10ms */
  HAL_GPIO_WritePin( LCD_RESET_GPIO_PORT, LCD_RESET_PIN, GPIO_PIN_RESET );
  HAL_Delay( 20 );  /* reset signal asserted during 20ms */
  HAL_GPIO_WritePin( LCD_RESET_GPIO_PORT, LCD_RESET_PIN, GPIO_PIN_SET );
  HAL_Delay( 10 );  /* reset signal released during 10ms */

  /* enable clock for LCD backlight control pin */
  LCD_BL_CTRL_GPIO_CLK_ENABLE();

  /* configure GPIO for LCD backlight control pin */
  gpio.Pin  = LCD_BL_CTRL_PIN;
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init( LCD_BL_CTRL_GPIO_PORT, &gpio );

  /* switch backlight on */
  HAL_GPIO_WritePin( LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET );
}


/* helper function to set a command register value */
static void SSD2119_WriteReg( uint8_t aReg, uint16_t aValue )
{
  *SSD2119_Cmnd = aReg;
  *SSD2119_Data = aValue;
}


/* helper function to read a data value */
/*
static uint16_t SSD2119_ReadData( void )
{
  return *SSD2119_Data;
}
*/


/*******************************************************************************
* FUNCTION:
*   SSD2119_Init
*
* DESCRIPTION:
*   The function initializes the FSMC hardware and all necessary GPIO in order
*   to reset and intialize the connected display hardware.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void SSD2119_Init( void )
{
  /* initialize FSMC hardware (16bit, 8080 interface) */
//  FMC_BANK1_Init();

  /* perform reset of LCD */
  SSD2119_ResetDisplay();

  /* Enter sleep mode (if we are not already there).*/
  SSD2119_WriteReg( SSD2119_SLEEP_MODE_1_REG, 0x0001 );

  /* Set initial power parameters. */
  SSD2119_WriteReg( SSD2119_PWR_CTRL_5_REG, 0x00B2 );
  SSD2119_WriteReg( SSD2119_VCOM_OTP_1_REG, 0x0006 );

  /* Start the oscillator.*/
  SSD2119_WriteReg( SSD2119_OSC_START_REG, 0x0001 );

  /* Set pixel format and basic display orientation (scanning direction).*/
  SSD2119_WriteReg( SSD2119_OUTPUT_CTRL_REG, 0x30EF ); /* 0x72EF for rotation by 180 degrees */
  SSD2119_WriteReg( SSD2119_LCD_DRIVE_AC_CTRL_REG, 0x0600 );

  /* Exit sleep mode.*/
  SSD2119_WriteReg( SSD2119_SLEEP_MODE_1_REG, 0x0000 );
  HAL_Delay( 100 );

  /* Configure pixel color format and MCU interface parameters.*/
  SSD2119_WriteReg( SSD2119_ENTRY_MODE_REG, ENTRY_MODE_DEFAULT );

  /* Set analog parameters */
  SSD2119_WriteReg( SSD2119_SLEEP_MODE_2_REG, 0x0999 );
  SSD2119_WriteReg( SSD2119_ANALOG_SET_REG, 0x3800 );

  /* Enable the display */
  SSD2119_WriteReg( SSD2119_DISPLAY_CTRL_REG, 0x0033 );

  /* Set VCIX2 voltage to 6.1V.*/
  SSD2119_WriteReg( SSD2119_PWR_CTRL_2_REG, 0x0005 );

  /* Configure gamma correction.*/
  SSD2119_WriteReg( SSD2119_GAMMA_CTRL_1_REG, 0x0000 );
  SSD2119_WriteReg( SSD2119_GAMMA_CTRL_2_REG, 0x0303 );
  SSD2119_WriteReg( SSD2119_GAMMA_CTRL_3_REG, 0x0407 );
  SSD2119_WriteReg( SSD2119_GAMMA_CTRL_4_REG, 0x0301 );
  SSD2119_WriteReg( SSD2119_GAMMA_CTRL_5_REG, 0x0301 );
  SSD2119_WriteReg( SSD2119_GAMMA_CTRL_6_REG, 0x0403 );
  SSD2119_WriteReg( SSD2119_GAMMA_CTRL_7_REG, 0x0707 );
  SSD2119_WriteReg( SSD2119_GAMMA_CTRL_8_REG, 0x0400 );
  SSD2119_WriteReg( SSD2119_GAMMA_CTRL_9_REG, 0x0a00 );
  SSD2119_WriteReg( SSD2119_GAMMA_CTRL_10_REG, 0x1000 );

  /* Configure Vlcd63 and VCOMl */
  SSD2119_WriteReg( SSD2119_PWR_CTRL_3_REG, 0x000A );
  SSD2119_WriteReg( SSD2119_PWR_CTRL_4_REG, 0x2E00 );
  HAL_Delay( 100 );

  /* Set the display size and ensure that the GRAM window is set to allow
     access to the full display buffer.*/
  SSD2119_WriteReg( SSD2119_V_RAM_POS_REG, (DISPLAY_PIXEL_HEIGHT - 1) << 8 );
  SSD2119_WriteReg( SSD2119_H_RAM_START_REG, 0x0000 );
  SSD2119_WriteReg( SSD2119_H_RAM_END_REG, DISPLAY_PIXEL_WIDTH - 1 );

  SSD2119_WriteReg( SSD2119_X_RAM_ADDR_REG, 0x00 );
  SSD2119_WriteReg( SSD2119_Y_RAM_ADDR_REG, 0x00 );
}


/*******************************************************************************
* FUNCTION:
*   SSD2119_ReadID
*
* DESCRIPTION:
*   The function assumes a connected and intialized SSD2119 display and tries
*   to read its ID.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   If successful, the function returns the ID of the display - 0 otherwise.
*
*******************************************************************************/
uint16_t SSD2119_ReadID(void)
{
  return SSD2119_ID;
}


/*******************************************************************************
* FUNCTION:
*   SSD2119_SetDataPosition
*
* DESCRIPTION:
*   The function sets the destination position within the framebuffer of the
*   display according the given x/y position.
*
* ARGUMENTS:
*   aX       - Horizontal position for next write access.
*   aY       - Vertical position for next write access.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void SSD2119_SetDataPosition( uint16_t aX, uint16_t aY )
{
  /* set the GRAM window to entire display size */
  SSD2119_WriteReg( SSD2119_V_RAM_POS_REG, ( DISPLAY_PIXEL_HEIGHT - 1 ) << 8 );
  SSD2119_WriteReg( SSD2119_H_RAM_START_REG, 0x0000 );
  SSD2119_WriteReg( SSD2119_H_RAM_END_REG, DISPLAY_PIXEL_WIDTH - 1 );

  SSD2119_WriteReg( SSD2119_X_RAM_ADDR_REG, aX );
  SSD2119_WriteReg( SSD2119_Y_RAM_ADDR_REG, aY );

  /* set command to make subsequent data transfers */
  *SSD2119_Cmnd = SSD2119_RAM_DATA_REG;
}


/*******************************************************************************
* FUNCTION:
*   SSD2119_SetDataWindow
*
* DESCRIPTION:
*   The function sets the destination position and size within the framebuffer
*   of the display according the given rectangle.
*
* ARGUMENTS:
*   aX       - Horizontal position for next write access.
*   aY       - Vertical position for next write access.
*   aW       - Width of the data window in pixel.
*   aH       - Height of the data window in pixel.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void SSD2119_SetDataWindow( uint16_t aX, uint16_t aY, uint16_t aW, uint16_t aH )
{
  int xEnd = aX + aW - 1;
  int yEnd = aY + aH - 1;

  if ( xEnd >= DISPLAY_PIXEL_WIDTH )
    xEnd = DISPLAY_PIXEL_WIDTH - 1;
  if ( yEnd >= DISPLAY_PIXEL_HEIGHT )
    yEnd = DISPLAY_PIXEL_HEIGHT - 1;

  SSD2119_WriteReg( SSD2119_V_RAM_POS_REG, (( yEnd & 0xFF ) << 8) | ( aY & 0xFF ));
  SSD2119_WriteReg( SSD2119_H_RAM_START_REG, aX );
  SSD2119_WriteReg( SSD2119_H_RAM_END_REG, xEnd );

  SSD2119_WriteReg( SSD2119_X_RAM_ADDR_REG, aX );
  SSD2119_WriteReg( SSD2119_Y_RAM_ADDR_REG, aY );

  /* set command to make subsequent data transfers */
  *SSD2119_Cmnd = SSD2119_RAM_DATA_REG;
}


/*******************************************************************************
* FUNCTION:
*   SSD2119_WriteData
*
* DESCRIPTION:
*   Write a single data word to the SSD2119 display.
*
* ARGUMENTS:
*   aData    - 16 bit value to be sent to the SSD2119 display.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void SSD2119_WriteData( uint16_t aData )
{
  *SSD2119_Data = aData;
}


void Lcd_SetPoint(uint16_t x,uint16_t y,uint16_t color)
{
	SSD2119_SetDataPosition( x, y );
	*SSD2119_Cmnd = 0x0022;
	*SSD2119_Data = color;
}


void LCD_DrawLine1( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
  short dx,dy;
  short temp;

  if( x0 > x1 )
  {
    temp = x1;
    x1 = x0;
    x0 = temp;
  }
  if( y0 > y1 )
  {
    temp = y1;
    y1 = y0;
    y0 = temp;
  }

  dx = x1-x0;
  dy = y1-y0;

  if( dx == 0 )
  {
    do
    {
    	Lcd_SetPoint(x0, y0, color);
      y0++;
    }
    while( y1 >= y0 );
    return;
  }
  if( dy == 0 )
  {
    do
    {
    	Lcd_SetPoint(x0, y0, color);
      x0++;
    }
    while( x1 >= x0 );
		return;
  }

	/* Based on Bresenham's line algorithm  */
  if( dx > dy )
  {
    temp = 2 * dy - dx;
    while( x0 != x1 )
    {
    	Lcd_SetPoint(x0,y0,color);
	    x0++;
	    if( temp > 0 )
	    {
	      y0++;
	      temp += 2 * dy - 2 * dx;
	 	  }
      else
      {
			  temp += 2 * dy;
			}
    }
    Lcd_SetPoint(x0,y0,color);
  }
  else
  {
    temp = 2 * dx - dy;
    while( y0 != y1 )
    {
    	Lcd_SetPoint(x0,y0,color);
      y0++;
      if( temp > 0 )
      {
        x0++;
        temp+=2*dy-2*dx;
      }
      else
	  {
        temp += 2 * dy;
	  }
    }
    Lcd_SetPoint(x0,y0,color);
  }
}

void LCD_PutCharFont( uint16_t Xpos, uint16_t Ypos, uint8_t ASCII, uint16_t charColor, uint16_t bkColor, uint16_t FONTx)
{
	uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    uint8_t len_x, len_y;

    switch (FONTx)
    {
			case font_8x5:
				len_x = 8;
    		len_y = 5;
				break;
    	case font_8x16:
    	default:
    		len_x = 8;
    		len_y = 16;
    		break;
    }

    GetASCIICode(buffer, ASCII, FONTx);
    for( i=0; i<len_y; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<len_x; j++ )
        {
            if( (tmp_char >> 7 - j) & 0x01 == 0x01 )
            {
            	Lcd_SetPoint( Xpos + j, Ypos + i, charColor );
            }
            else
            {
            	Lcd_SetPoint( Xpos + j, Ypos + i, bkColor );
            }
        }
    }
}

void LCD_PutChar(uint16_t Xpos, uint16_t Ypos, uint8_t ASCII, uint16_t charColor, uint16_t bkColor)
{
	LCD_PutCharFont(Xpos, Ypos, ASCII, charColor, bkColor, font_8x16);
}



void LCD_CleanPutCharFont( uint16_t Xpos, uint16_t Ypos, uint8_t ASCII, uint16_t charColor, uint16_t FONTx)
{
	uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    uint8_t len_x, len_y;

    switch (FONTx)
    {
			case font_8x5:
				len_x = 8;
    		len_y = 5;
				break;
    	case font_8x16:
    	default:
    		len_x = 8;
    		len_y = 16;
    		break;
    }

    GetASCIICode(buffer, ASCII, FONTx);
    for( i=0; i<len_y; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<len_x; j++ )
        {
            if( ((tmp_char >> (7 - j)) & 0x01) == 0x01 )
            {
            	Lcd_SetPoint( Xpos + j, Ypos + i, charColor );
            }
        }
    }
}

void LCD_CleanPutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCII, uint16_t charColor)
{
	LCD_CleanPutCharFont(Xpos, Ypos, ASCII, charColor, font_8x16);
}




//void LCD_TextFont(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor, uint16_t FONTx)
//{
//    uint8_t TempChar;
//    uint8_t delta_x, delta_y;
//
//    switch (FONTx)
//    {
//			case font_8x5:
//				delta_x = 8;
//    		delta_y = 5;
//				break;
//    	case font_8x16:
//    	default:
//    		delta_x = 8;
//    		delta_y = 16;
//    		break;
//    }
//
//    do
//    {
//        TempChar = *str++;
//        LCD_PutCharFont( Xpos, Ypos, TempChar, Color, bkColor, FONTx );
//        if( Xpos < MAX_X - delta_x )
//        {
//            Xpos += delta_x;
//        }
//        else if ( Ypos < MAX_Y - delta_y )
//        {
//            Xpos = 0;
//            Ypos += delta_y;
//        }
//        else
//        {
//            Xpos = 0;
//            Ypos = 0;
//        }
//    }
//    while ( *str != 0 );
//}


void LCD_CleanText(uint16_t Xpos, uint16_t Ypos, char *str, uint16_t Color)
{
	LCD_CleanTextFont(Xpos, Ypos, str, Color, font_8x16);
}
void LCD_CleanTextFont(uint16_t Xpos, uint16_t Ypos, char *str, uint16_t Color, uint16_t FONTx)
{
    uint8_t TempChar;
    do
    {
        TempChar = *str++;
        LCD_CleanPutCharFont( Xpos, Ypos, TempChar, Color, FONTx);
        if( Xpos < MAX_X - 8 )
        {
            Xpos += 8;
        }
        else if ( Ypos < MAX_Y - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }
        else
        {
            Xpos = 0;
            Ypos = 0;
        }
    }
    while ( *str != 0 );
}

void LCD_DrawPicture(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *pic)
{
	uint16_t x_index, y_index;
	uint32_t pic_index = 0;

    //for(y_index = y; y_index < (y+h); y_index++)
    //if wrong orientation
    for(y_index = (y+h); y_index > 0 ; y_index--)
    {
        for(x_index = x; x_index < (x+w); x_index++)
        {
			LCD_SetPoint(x_index, y_index, pic[pic_index]);
			pic_index++;
		}
	}
}

void LCD_DrawPicture8bit(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *pic)
{
	uint16_t color;
	uint16_t x_index, y_index;
	uint32_t pic_index = 0;

    //for(y_index = y; y_index < (y+h); y_index++)
    //if wrong orientation
	for(y_index = (y+h); y_index > 0 ; y_index--)
	{
		for(x_index = x; x_index < (x+w); x_index++)
		{
			color = (pic[pic_index+1] << 8) | (pic[pic_index]);
			LCD_SetPoint(x_index, y_index, color);
			pic_index += 2;
		}
	}
}

void LCD_DrawCircle1(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
	int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    LCD_SetPoint(x0, y0+r, color);
    LCD_SetPoint(x0, y0-r, color);
    LCD_SetPoint(x0+r, y0, color);
    LCD_SetPoint(x0-r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        LCD_SetPoint(x0 + x, y0 + y, color);
        LCD_SetPoint(x0 - x, y0 + y, color);
        LCD_SetPoint(x0 + x, y0 - y, color);
        LCD_SetPoint(x0 - x, y0 - y, color);

        LCD_SetPoint(x0 + y, y0 + x, color);
        LCD_SetPoint(x0 - y, y0 + x, color);
        LCD_SetPoint(x0 + y, y0 - x, color);
        LCD_SetPoint(x0 - y, y0 - x, color);
    }
}

void LCD_FillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
	int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    int16_t i;

    for (i=y0-r; i<=y0+r; i++)
    	Lcd_SetPoint(x0, i, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        for (i=y0-y; i<=y0+y; i++) {
        	Lcd_SetPoint(x0+x, i, color);
        	Lcd_SetPoint(x0-x, i, color);
        }
        for (i=y0-x; i<=y0+x; i++) {
        	Lcd_SetPoint(x0+y, i, color);
        	Lcd_SetPoint(x0-y, i, color);
        }
    }
}

void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	if (x > MAX_X)
		x = MAX_X;
	if (y > MAX_Y)
		y = MAX_Y;

	if ((x+w) > MAX_X)
		w = MAX_X - x;

	if ((y+h) > MAX_Y)
		h = MAX_Y - y;

	LCD_DrawLine1(x, y, x, y+h, color);
	LCD_DrawLine1(x, y, x+w, y, color);
	LCD_DrawLine1(x+w, y+h, x, y+h, color);
	LCD_DrawLine1(x+w, y+h, x+w, y, color);
}

void LCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
	uint16_t x_index, y_index;

	if (x > MAX_X)
		x = MAX_X;
	if (y > MAX_Y)
		y = MAX_Y;

	if ((x+w) > MAX_X)
		w = MAX_X - x;

	if ((y+h) > MAX_Y)
		h = MAX_Y - y;

	for(x_index = x; x_index < x+w; x_index++)
	{
		for(y_index = y; y_index < y+h; y_index++)
		{
			Lcd_SetPoint(x_index, y_index, color);
		}
	}

}

void LCD_DrawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	if (x0 > MAX_X)
		x0 = MAX_X;
	if (y0 > MAX_Y)
		y0 = MAX_Y;
	if (x1 > MAX_X)
		x1 = MAX_X;
	if (y1 > MAX_Y)
		y1 = MAX_Y;
	if (x2 > MAX_X)
		x2 = MAX_X;
	if (y2 > MAX_Y)
		y2 = MAX_Y;

	LCD_DrawLine1(x0, y0, x1, y1, color);
	LCD_DrawLine1(x0, y0, x2, y2, color);
	LCD_DrawLine1(x2, y2, x1, y1, color);
}

void swap(signed int *a, signed int *b)
{
    signed int temp = 0x0000;

    temp = *b;
    *b = *a;
    *a = temp;
}
void Draw_V_Line(signed int x1, signed int y1, signed int y2, unsigned colour)
{
    if(y1 > y2)
    {
       swap(&y1, &y2);
    }

    while(y2 > (y1 - 1))
    {
    	Lcd_SetPoint(x1, y2, colour);
        y2--;
    }
}


void Draw_H_Line(signed int x1, signed int x2, signed int y1, unsigned colour)
{
    if(x1 > x2)
    {
       swap(&x1, &x2);
    }

    while(x2 > (x1 - 1))
    {
    	Lcd_SetPoint(x2, y1, colour);
        x2--;
    }
}
void Draw_Rectangle(signed int x1, signed int y1, signed int x2, signed int y2, unsigned char fill, unsigned char type, unsigned int colour, unsigned int back_colour)
{
     signed int i = 0x0000;
     signed int xmin = 0x0000;
     signed int xmax = 0x0000;
     signed int ymin = 0x0000;
     signed int ymax = 0x0000;

     switch(fill)
     {
         case 1:
         {
             if(x1 < x2)
             {
                xmin = x1;
                xmax = x2;
             }
             else
             {
                xmin = x2;
                xmax = x1;
             }

             if(y1 < y2)
             {
                ymin = y1;
                ymax = y2;
             }
             else
             {
                ymin = y2;
                ymax = y1;
             }

             for(; xmin <= xmax; ++xmin)
             {
                 for(i = ymin; i <= ymax; ++i)
                 {
                	 Lcd_SetPoint(xmin, i, colour);
                 }
             }

             break;
         }
         default:
         {
             Draw_V_Line(x1, y1, y2, colour);
             Draw_V_Line(x2, y1, y2, colour);
             Draw_H_Line(x1, x2, y1, colour);
             Draw_H_Line(x1, x2, y2, colour);
             break;
         }
     }

     if(type != 0)
     {
    	 Lcd_SetPoint(x1, y1, back_colour);
    	 Lcd_SetPoint(x1, y2, back_colour);
    	 Lcd_SetPoint(x2, y1, back_colour);
    	 Lcd_SetPoint(x2, y2, back_colour);
     }
}


void Draw_H_Bar(signed int x1, signed int x2, signed int y1, signed int bar_width, signed int bar_value, unsigned int border_colour, unsigned int bar_colour, unsigned int back_colour, unsigned char border)
{
    switch(border)
    {
        case 1:
        {
            Draw_Rectangle((x1 + 1), (y1 + 1), (x1 + bar_value), (y1 + bar_width - 1), 1, 0, bar_colour, back_colour);
            Draw_Rectangle((x2 - 1), (y1 + 1), (x1 + bar_value + 1), (y1 + bar_width - 1), 1, 0, back_colour, back_colour);
            Draw_Rectangle(x1, y1, x2, (y1 + bar_width), 0, 0, border_colour, back_colour);
            break;
        }
        default:
        {
            Draw_Rectangle(x1, y1, (x1 + bar_value), (y1 + bar_width), 1, 0, bar_colour, back_colour);
            Draw_Rectangle(x2, y1, (x1 + bar_value), (y1 + bar_width), 1, 0, back_colour, back_colour);
            break;
        }
    }
}

void Draw_V_Bar(signed int x1, signed int y1, signed int y2, signed int bar_width, signed int bar_value, unsigned int border_colour, unsigned int bar_colour, unsigned int back_colour, unsigned char border)
{
    switch(border)
    {
        case 1:
        {
            Draw_Rectangle((x1 + 1), (y2 - 1), (x1 + bar_width - 1), (y2 - bar_value), 1, 0, bar_colour, back_colour);
            Draw_Rectangle((x1 + 1), (y2 - bar_value - 1), (x1 + bar_width - 1), (y1 + 1), 1, 0, back_colour, back_colour);
            Draw_Rectangle(x1, y1, (x1 + bar_width), y2, 0, 0, border_colour, back_colour);
            break;
        }
        default:
        {
            Draw_Rectangle(x1, y2, (x1 + bar_width), (y2 - bar_value), 1, 0, bar_colour, back_colour);
            Draw_Rectangle(x1, (y2 - bar_value), (x1 + bar_width), y1, 1, 0, back_colour, back_colour);
            break;
        }
    }
}



void LCD_Clear(uint16_t color)
{

	uint32_t i;
	*SSD2119_Cmnd = 0x0022;
	for(i=0;i<76800;i++)
	{
		*SSD2119_Data = color;
	}

}
/*******************************************************************************
* FUNCTION:
*   SSD2119_WriteMultipleData
*
* DESCRIPTION:
*   Write a sequence of data words to the SSD2119 display.
*
* ARGUMENTS:
*   aData    - pointer to 16 bit values to be sent to the SSD2119 display.
*   aSize    - Number of data values to be sent.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void SSD2119_WriteMultipleData( uint16_t* aData, uint32_t aSize )
{
  while ( aSize-- )
    *SSD2119_Data = *aData++;
}


/* msy */

void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

void LCD_SetColors(uint16_t _TextColor, uint16_t _BackColor)
{
  TextColor = _TextColor;
  BackColor = _BackColor;
}

void LCD_GetColors(uint16_t *_TextColor, uint16_t *_BackColor)
{
  *_TextColor = TextColor; *_BackColor = BackColor;
}

void LCD_SetTextColor(uint16_t Color)
{
  TextColor = Color;
}

void LCD_SetBackColor(uint16_t Color)
{
  BackColor = Color;
}


void LCD_WriteRAM_Prepare(void)
{
	*SSD2119_Cmnd = SSD2119_RAM_DATA_REG;
}

void LCD_WriteRAM(uint16_t RGB_Code)
{
  /* Write 16-bit GRAM Reg */
  *SSD2119_Data = RGB_Code;
}

void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
  *SSD2119_Cmnd = LCD_Reg;
  /* Write 16-bit Reg */
  *SSD2119_Data = LCD_RegValue;
}

void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  /* Set the X address of the display cursor.*/
  LCD_WriteReg(SSD2119_X_RAM_ADDR_REG, Xpos);

  /* Set the Y address of the display cursor.*/
  LCD_WriteReg(SSD2119_Y_RAM_ADDR_REG, Ypos);
}

void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
  uint32_t i = 0;

  LCD_SetCursor(Xpos, Ypos);
  if(Direction == LCD_DIR_HORIZONTAL)
  {
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    for(i = 0; i < Length; i++)
    {
      LCD_WriteRAM(TextColor);
    }
  }
  else
  {
    for(i = 0; i < Length; i++)
    {
      LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
      LCD_WriteRAM(TextColor);
      Ypos++;
      LCD_SetCursor(Xpos, Ypos);
    }
  }
}

void LCD_DrawFullRectFill(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint8_t Fill)
{
  uint16_t color_bkp = TextColor;

  LCD_DrawLine(Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);
  LCD_DrawLine(Xpos, (Ypos+Height), Width, LCD_DIR_HORIZONTAL);

  LCD_DrawLine(Xpos, Ypos, Height, LCD_DIR_VERTICAL);
  LCD_DrawLine((Xpos+Width-1), Ypos, Height, LCD_DIR_VERTICAL);

  Height--;
  Ypos++;
  Xpos++;
  Width-=2;

  if (0 == Fill)
    LCD_SetTextColor(BackColor);

  while(Height--)
  {
    LCD_DrawLine(Xpos, Ypos++, Width, LCD_DIR_HORIZONTAL);
  }

  LCD_SetTextColor(color_bkp);
}

void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;/* Decision Variable */
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */

  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;

  while (CurX <= CurY)
  {
    LCD_SetCursor(Xpos + CurX, Ypos + CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos + CurX, Ypos - CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos - CurX, Ypos + CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos - CurX, Ypos - CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos + CurY, Ypos + CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos + CurY, Ypos - CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos - CurY, Ypos + CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    LCD_SetCursor(Xpos - CurY, Ypos - CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_WriteRAM(TextColor);
    if (D < 0)
    {
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
}


void LCD_DrawFullCircleFill(uint16_t Xpos, uint16_t Ypos, uint16_t Radius, uint8_t Fill)
{
  int32_t  D;    /* Decision Variable */
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */
  uint16_t color_bkp = TextColor;

  D = 3 - (Radius << 1);

  CurX = 0;
  CurY = Radius;

  if (0 == Fill)
  {
      LCD_SetTextColor(BackColor);
  }

  while (CurX <= CurY)
  {
    if(CurY > 0)
    {
      LCD_DrawLine(Xpos - CurX, Ypos - CurY, 2*CurY, LCD_DIR_VERTICAL);
      LCD_DrawLine(Xpos + CurX, Ypos - CurY, 2*CurY, LCD_DIR_VERTICAL);
    }

    if(CurX > 0)
    {
      LCD_DrawLine(Xpos - CurY, Ypos - CurX, 2*CurX, LCD_DIR_VERTICAL);
      LCD_DrawLine(Xpos + CurY, Ypos - CurX, 2*CurX, LCD_DIR_VERTICAL);
    }
    if (D < 0)
    {
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }

  LCD_SetTextColor(color_bkp);
  if (0 == Fill)
  {
	  LCD_DrawCircle(Xpos, Ypos, Radius);
  }
}

void LCD_DrawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
	LCD_DrawFullCircleFill(Xpos, Ypos, Radius, 0);
}

sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}


void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  uint32_t index = 0, i = 0;
  uint16_t  Xaddress = 0;
  Xaddress = Xpos;

  LCD_SetCursor(Ypos, Xaddress);

  for(index = 0; index < LCD_Currentfonts->Height; index++)
  {
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    for(i = 0; i < LCD_Currentfonts->Width; i++)
    {

      if((((c[index] & ((0x80 << ((LCD_Currentfonts->Width / 12 ) * 8 ) ) >> i)) == 0x00) &&(LCD_Currentfonts->Width <= 12))||
        (((c[index] & (0x1 << i)) == 0x00)&&(LCD_Currentfonts->Width > 12 )))

      {
        LCD_WriteRAM(BackColor);
      }
      else
      {
        LCD_WriteRAM(TextColor);
      }
    }
    Xaddress++;
    LCD_SetCursor(Ypos, Xaddress);
  }
}

void LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii)
{
  Ascii -= 32;
  LCD_DrawChar(Line, Column, &LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height]);
}


void LCD_DisplayString(uint16_t Line, uint16_t Column, uint8_t *ptr)
{
  uint16_t refcolumn = Column;

  /* Send the string character by character on lCD */
  while (*ptr != 0)
  {
    /* Display one character on LCD */
    LCD_DisplayChar(Line, refcolumn, *ptr);
    /* Decrement the column position by 16 */
    refcolumn += LCD_Currentfonts->Width;
	if (refcolumn >= LCD_PIXEL_WIDTH) {
		break;
	}
    /* Point on the next character */
    ptr++;
  }
}

//#define LCD_PIXEL_WIDTH          320
//#define LCD_PIXEL_HEIGHT         240

//void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
//    if((x >= LCD_PIXEL_WIDTH) || (y >= LCD_PIXEL_HEIGHT))
//        return;
//    LCD_SetCursor(x, y);
//    *SSD2119_Data = color;
//}
//
//static void ILI9341_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
//{
//    uint32_t i, b, j;
//    LCD_SetCursor(x+font.width-1, y+font.height-1);
//    for (i = 0; i < font.height; i++)
//    {
//        b = font.data[(ch-32) * font.height + i];
//        for (j = 0; j < font.width; j++)
//        {
//            if ((b << j) & 0x8000)
//                ILI9341_DrawPixel(x+j, y+i, color);
//
//            else
//                ILI9341_DrawPixel(x+j, y+i, bgcolor);
//        }
//    }
//}
//
//
//void ILI9341_WriteString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor) {
//    while(*str) {
//        if(x + font.width >= LCD_PIXEL_WIDTH) {
//            x = 0;
//            y += font.height;
//            if(y + font.height >= LCD_PIXEL_HEIGHT) {
//                break;
//            }
//
//            if(*str == ' ') {
//                // skip spaces in the beginning of the new line
//                str++;
//                continue;
//            }
//        }
//
//        ILI9341_WriteChar(x, y, *str, font, color, bgcolor);
//        x += font.width;
//        str++;
//    }
//}
