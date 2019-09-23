#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
//////////////////////////////////////////////////////////////////////////////////	 
extern float fft_outputbuf[4096];
extern int xiebo;
extern float kk;
//Õ‚≤ø÷–∂œ∑˛ŒÒ≥Ã–Ú

//Õ‚≤ø÷–∂œ2∑˛ŒÒ≥Ã–Ú
void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);	//œ˚∂∂
	if(B==0)	  
	{		
		if(fft_outputbuf[xiebo]!=0) 
		{kk=50/(fft_outputbuf[xiebo]/2048);}
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line6);//«Â≥˝LINE2…œµƒ÷–∂œ±Í÷æŒª 
	delay_ms(100);
}
//Õ‚≤ø÷–∂œ3∑˛ŒÒ≥Ã–Ú
void EXTI4_IRQHandler(void)
{
	delay_ms(10);	//œ˚∂∂
	if(C==0)	 
	{
				if(fft_outputbuf[xiebo]!=0) 
		{kk=50/(fft_outputbuf[xiebo]/2048);}
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line4);  //«Â≥˝LINE3…œµƒ÷–∂œ±Í÷æŒ
	 delay_ms(100);
}
//Õ‚≤ø÷–∂œ4∑˛ŒÒ≥Ã–Ú
void EXTI2_IRQHandler(void)
{
	delay_ms(10);	//œ˚∂∂
	if(D==0)	 
	{	if(fft_outputbuf[xiebo]!=0) 
		{kk=50/(fft_outputbuf[xiebo]/2048);} 
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line2);//«Â≥˝LINE4…œµƒ÷–∂œ±Í÷æŒª 
	 delay_ms(100);	
}
	   
//Õ‚≤ø÷–∂œ≥ı ºªØ≥Ã–Ú
//≥ı ºªØPE2~4,PA0Œ™÷–∂œ ‰»Î.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);// πƒ‹SYSCFG ±÷”
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 ¡¨Ω”µΩ÷–∂œœﬂ2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);//PE3 ¡¨Ω”µΩ÷–∂œœﬂ3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource6);//PE4 ¡¨Ω”µΩ÷–∂œœﬂ4
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);//PC13 ¡¨Ω”µΩ÷–∂œœﬂ0
	
	/* ≈‰÷√EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line13 | EXTI_Line6 | EXTI_Line4 | EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode	 = EXTI_Mode_Interrupt;//÷–∂œ ¬º˛
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //œ¬Ωµ—ÿ¥•∑¢
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//÷–∂œœﬂ πƒ‹
  EXTI_Init(&EXTI_InitStructure);//≈‰÷√
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //Õ‚≤ø÷–∂œ0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//«¿’º”≈œ»º∂0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//◊””≈œ»º∂2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;// πƒ‹Õ‚≤ø÷–∂œÕ®µ¿
  NVIC_Init(&NVIC_InitStructure);//≈‰÷√
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//Õ‚≤ø÷–∂œ2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//«¿’º”≈œ»º∂3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//◊””≈œ»º∂2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;// πƒ‹Õ‚≤ø÷–∂œÕ®µ¿
  NVIC_Init(&NVIC_InitStructure);//≈‰÷√
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//Õ‚≤ø÷–∂œ3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//«¿’º”≈œ»º∂2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//◊””≈œ»º∂2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;// πƒ‹Õ‚≤ø÷–∂œÕ®µ¿
  NVIC_Init(&NVIC_InitStructure);//≈‰÷√
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//Õ‚≤ø÷–∂œ4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//«¿’º”≈œ»º∂1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//◊””≈œ»º∂2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;// πƒ‹Õ‚≤ø÷–∂œÕ®µ¿
  NVIC_Init(&NVIC_InitStructure);//≈‰÷√
	   
}
