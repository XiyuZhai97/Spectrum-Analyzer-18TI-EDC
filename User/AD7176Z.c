#include "stm32f4xx.h"
#include "usart.h"
#include "AD7176Z.h"
#include "delay.h"
#include "LCD/LCD.h"
void AT45DBXX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	SPI_Configuration();

  RCC_AHBxPeriphClockCmd(RCC_GPIO_CS,ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIO_CS_PORT, &GPIO_InitStructure);
  	
  AT45DBXX_Disable;
}

static void AT45DBXX_BUSY(void)
{
	u8 AT45DBXXStruct;
	AT45DBXX_Enable;
	SPI_Send_byte(READ_STATE_REGISTER);
	do
	{AT45DBXXStruct = SPI_Receive_byte();}
	while(!(AT45DBXXStruct & 0x80));
	AT45DBXX_Disable;	
}
void AD7176_Read_ID()
{	u16 ID1;
	u16 ID2;
	char IDB[20];

	AT45DBXX_Enable;	
	SPI_Send_byte(0X47); //÷¥––∂¡»°id√¸¡Ó		  	
	delay_us(50);
	ID1=SPI_Receive_byte();
	ID2=SPI_Receive_byte();
	sprintf(IDB,"%02x %02x",ID1,ID2);
	GUI_Text(100,10,(uint8_t*)IDB,Black,White);
	AT45DBXX_Disable;
}
