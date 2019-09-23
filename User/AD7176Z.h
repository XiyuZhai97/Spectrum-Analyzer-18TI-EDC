#ifndef _AD7176Z_H
#define _AD7176Z_H
#include "ws_spi.h"
#define BUF1_WRITE 0x84
#define BUF2_WRITE 0x87
#define BUF1_READ 0xD4
#define BUF2_READ 0xD6
#define BBUF1_TO_MM_PAGE_PROG_WITH_ERASE 0x83
#define BBUF2_TO_MM_PAGE_PROG_WITH_ERASE 0x86
#define MM_PAGE_TO_B1_XFER 0x53
#define MM_PAGE_TO_B2_XFER 0x55
#define PAGE_ERASE 0x81   // 512/528 bytes per page
#define SECTOR_ERASE 0x7C // 128k bytes per sector
#define READ_STATE_REGISTER 0x47
#define Read_ID 0x47

#define RCC_GPIO_CS					RCC_AHB1Periph_GPIOB
#define RCC_AHBxPeriphClockCmd		RCC_AHB1PeriphClockCmd
#define GPIO_PIN_CS					GPIO_Pin_12
#define GPIO_CS_PORT				GPIOB

#define AT45DBXX_Enable 			GPIO_ResetBits(GPIO_CS_PORT,GPIO_PIN_CS);
#define AT45DBXX_Disable 			GPIO_SetBits(GPIO_CS_PORT,GPIO_PIN_CS);


void AT45DBXX_Init(void);
static void AT45DBXX_BUSY(void);
void AD7176_Read_ID();

#endif /*_AT45DBXX_H*/
