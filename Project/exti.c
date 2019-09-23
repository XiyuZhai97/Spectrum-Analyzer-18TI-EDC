#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
//////////////////////////////////////////////////////////////////////////////////	 
extern float fft_outputbuf[4096];
extern int xiebo;
extern float kk;
//外部中断服务程序

//外部中断2服务程序
void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(B==0)	  
	{		
		if(fft_outputbuf[xiebo]!=0) 
		{kk=50/(fft_outputbuf[xiebo]/2048);}
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line6);//清除LINE2上的中断标志位 
	delay_ms(100);
}
//外部中断3服务程序
void EXTI4_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(C==0)	 
	{
				if(fft_outputbuf[xiebo]!=0) 
		{kk=50/(fft_outputbuf[xiebo]/2048);}
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE3上的中断标志?
	 delay_ms(100);
}
//外部中断4服务程序
void EXTI2_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(D==0)	 
	{	if(fft_outputbuf[xiebo]!=0) 
		{kk=50/(fft_outputbuf[xiebo]/2048);}
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line2);//清除LINE4上的中断标志位 
	 delay_ms(100);	
}
	   
//外部中断初始化程序
//初始化PE2~4,PA0为中断输入.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);//PE3 连接到中断线3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource6);//PE4 连接到中断线4
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);//PC13 连接到中断线0
	
	/* 配置EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line13 | EXTI_Line6 | EXTI_Line4 | EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode	 = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //外部中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//外部中断3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	   
}
