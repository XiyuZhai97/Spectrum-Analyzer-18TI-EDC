#ifndef __LCD_h 
#define __LCD_h

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include "stm32f4xx.h"
#include "config.h"
#include "LCD/AsciiLib.h" 

/* Private define ------------------------------------------------------------*/
#define USE_16BIT_PMP

/*********************************************************************
* Overview: Horizontal and vertical display resolution
*                  (from the glass datasheet).
*********************************************************************/
//#define DISP_HOR_RESOLUTION				320
//#define DISP_VER_RESOLUTION				240

//#define DISP_ORIENTATION					0
//#define DISP_ORIENTATION					90
//#define DISP_ORIENTATION					180
#define DISP_ORIENTATION					90

/* Private define ------------------------------------------------------------*/

#if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

#define  MAX_X  320
#define  MAX_Y  240   

#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

#define  MAX_X  240
#define  MAX_Y  320   

#endif

/*********************************************************************
* Overview: Horizontal synchronization timing in pixels
*                  (from the glass datasheet).
*********************************************************************/
//#define DISP_HOR_PULSE_WIDTH		20    /* 20 */
//#define DISP_HOR_BACK_PORCH			51	  /* 48	*/
//#define DISP_HOR_FRONT_PORCH		20	  /* 20 */

/*********************************************************************
* Overview: Vertical synchronization timing in lines
*                  (from the glass datasheet).
*********************************************************************/
//#define DISP_VER_PULSE_WIDTH		2	  /* 2 */
//#define DISP_VER_BACK_PORCH			12	  /* 16 */
//#define DISP_VER_FRONT_PORCH		4	  /* 4 */

/*********************************************************************
* Definition for SPI interface for HIMAX 8238-A relevant to hardware 
* layout; Hardware dependent!
*********************************************************************/
#define GPIO3 3
#define GPIO2 2
#define GPIO1 1
#define GPIO0 0
#define LCD_RESET (1<<GPIO3)	   /* LCD Reset signal (Reset for display panel, NOT ssd1963) */
#define LCD_SPENA (1<<GPIO0)	   /* SPI EN signal */
#define LCD_SPCLK (1<<GPIO1)	   /* SPI CLK */
#define LCD_SPDAT (1<<GPIO2)	   /* SPI DATA */

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#define RGB565CONVERT(red, green, blue) (int) (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))

/* Private function prototypes -----------------------------------------------*/
void LCD_Initializtion(void);
void LCD_Clear(u16 Color);	
//void LCD_SetBacklight(uint8_t intensity);
u16  LCD_GetPoint(u16 Xpos,u16 Ypos);
void LCD_SetPoint(u16 Xpos,u16 Ypos,uint16_t point);
void PutChar(u16 Xpos,u16 Ypos,u8 c,u16 charColor,u16 bkColor);
void LCD_DrawLine(u16 x0,u16 y0,u16 x1,u16 y1,u16 color);
void PutChinese(u16 Xpos,u16 Ypos,u8 *str,u16 Color,u16 bkColor); 
void GUI_Text(u16 Xpos, u16 Ypos, u8 *str,u16 Color, u16 bkColor);
void GUI_Chinese(u16 Xpos, u16 Ypos, u8 *str,u16 Color, u16 bkColor);	
void LCD_ShowPicture(u16 x, u16 y, u16 wide, u16 high,u8 *pic);
void Prepare_Write_RAM(void);
void PutNum( uint16_t Xpos, uint16_t Ypos, uint8_t num, uint16_t charColor, uint16_t bkColor );
void GUI_Num(uint16_t Xpos, uint16_t Ypos, u32 num,u8 len,uint16_t Color, uint16_t bkColor);
__inline void LCD_WriteIndex(u16 index);
__inline void LCD_WriteData(u16 data);
__inline u16  LCD_ReadData(void);
__inline u16  LCD_ReadReg(u16 LCD_Reg);
__inline void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue);
void LCD_SetCursor(u16 Xpos,u16 Ypos);
void delay_ms(u16 ms);

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

