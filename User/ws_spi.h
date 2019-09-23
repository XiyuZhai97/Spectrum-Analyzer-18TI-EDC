#ifndef __SPI_H
#define __SPI_H
#include "stm32f4xx.h"
static void SPI_Rcc_Configuration(void);
static void GPIO_Configuration(void);
void SPI_Configuration(void);
void SPI_Send_byte(u16 data);
u16 SPI_Receive_byte(void);
#endif 

