#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
//////////////////////////////////////////////////////////////////////////////////	 
extern float fft_outputbuf[4096];
extern int xiebo;
extern float kk;
//�ⲿ�жϷ������

//�ⲿ�ж�2�������
void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);	//����
	if(B==0)	  
	{		
		if(fft_outputbuf[xiebo]!=0) 
		{kk=50/(fft_outputbuf[xiebo]/2048);}
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line6);//���LINE2�ϵ��жϱ�־λ 
	delay_ms(100);
}
//�ⲿ�ж�3�������
void EXTI4_IRQHandler(void)
{
	delay_ms(10);	//����
	if(C==0)	 
	{
				if(fft_outputbuf[xiebo]!=0) 
		{kk=50/(fft_outputbuf[xiebo]/2048);}
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE3�ϵ��жϱ�־�
	 delay_ms(100);
}
//�ⲿ�ж�4�������
void EXTI2_IRQHandler(void)
{
	delay_ms(10);	//����
	if(D==0)	 
	{	if(fft_outputbuf[xiebo]!=0) 
		{kk=50/(fft_outputbuf[xiebo]/2048);} 
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line2);//���LINE4�ϵ��жϱ�־λ 
	 delay_ms(100);	
}
	   
//�ⲿ�жϳ�ʼ������
//��ʼ��PE2~4,PA0Ϊ�ж�����.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 ���ӵ��ж���2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);//PE3 ���ӵ��ж���3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource6);//PE4 ���ӵ��ж���4
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);//PC13 ���ӵ��ж���0
	
	/* ����EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line13 | EXTI_Line6 | EXTI_Line4 | EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode	 = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ⲿ�ж�2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//�ⲿ�ж�3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ⲿ�ж�4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	   
}