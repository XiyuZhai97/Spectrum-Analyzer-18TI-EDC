#include "tim.h"
#include "led.h"
#include "adc.h"
#include "AD7176Z.h"
#include "LCD/LCD.h"
//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
u16 k;u16 DAT1;u16 DAT2;u16 DAT3;
extern u32 adcd[4096];extern u32 Volt;

long long tlow,thigh,ttotal;
double temp=0,totaltemp=0,ftemp;
char strf[32];

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		AT45DBXX_Enable;	
		while(PCin(2));
		Volt=0;
		SPI_Send_byte(0X44); //�����ݼĴ���
		DAT1=SPI_Receive_byte();
		DAT2=SPI_Receive_byte();
		DAT3=SPI_Receive_byte();
		Volt=(Volt|DAT1)&0x0000ff;
		Volt=(((Volt<<8)&0x00ff00)|DAT2)&0x00ffff;
		Volt=(((Volt<<8)&0xffff00)|DAT3)&0xffffff;
		adcd[k%4096]=Volt;//VoltΪ��ȡ����24λ������
		k++;
		AT45DBXX_Disable;
	}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}


TIM_ICInitTypeDef  TIM5_ICInitStructure;

//��ʱ��5ͨ��1���벶������
//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
//psc��ʱ��Ԥ��Ƶ��
void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����-----------------
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //PA0����λ��ʱ��5
  
	//��ʼ�� TIM5,���� TIM5 �� ARR �� PSC��  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	

	//3������ TIM5 �����벶��������������벶��
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//�����ز���
  TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5

 
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	
}
//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
//u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u32 TIM5CH1_CAPTURE_STA_LOW=0;
u32 TIM5CH1_CAPTURE_STA_HIGH=0;
u32	TIM5CH1_CAPTURE_VAL_LOW;	//���벶��ֵ(TIM2/TIM5��32λ)
u32	TIM5CH1_CAPTURE_VAL_HIGH;	//���벶��ֵ(TIM2/TIM5��32λ)
u32 tem;
//��ʱ��5�жϷ������	 
void TIM5_IRQHandler(void)
{ 	  
	 	if((TIM5CH1_CAPTURE_STA_HIGH&0x80000000)==0)//��δ�ɹ�����һ������
        {
	        if((TIM5CH1_CAPTURE_STA_LOW&0x80000000)==0)////��δ�ɹ�����һ���͵�ƽ
	        {
					//if1,��ʵ��̫���ܷ�����
					if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//���
					{	     
						
						if(TIM5CH1_CAPTURE_STA_LOW&0x40000000)//�Ѿ����񵽸ߵ�ƽ��
						{
							if((TIM5CH1_CAPTURE_STA_LOW&0x3FFFFFFF)==0x3FFFFFFF)//�ߵ�ƽ̫����
							{
								TIM5CH1_CAPTURE_STA_LOW|=0x80000000;		//��ǳɹ�������һ��
 								TIM5CH1_CAPTURE_VAL_LOW=0xFFFFFFFF;
	                             TIM5CH1_CAPTURE_STA_HIGH=0;			//���
							    TIM5CH1_CAPTURE_VAL_HIGH=0;
							    TIM5CH1_CAPTURE_STA_HIGH|=0x40000000;		//��ǲ�����������  ��D6=1
							    TIM_SetCounter(TIM5,0);
							    TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
							    TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
							}else TIM5CH1_CAPTURE_STA_LOW++;
						}	 
					}//have been checked
					//if2
					if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼�
					{	
 						if(TIM5CH1_CAPTURE_STA_LOW&0x40000000) 	//�������½��أ��ɹ�����һ�θߵ�ƽ����
						{	  			
							TIM5CH1_CAPTURE_STA_LOW|=0x80000000;		//��ǳɹ�����һ�θߵ�ƽ����
						    TIM5CH1_CAPTURE_VAL_LOW=TIM_GetCapture1(TIM5);//��ȡ��ǰ�Ĳ���ֵ.
 							TIM5CH1_CAPTURE_STA_HIGH=0;			//���
							TIM5CH1_CAPTURE_VAL_HIGH=0;
							TIM5CH1_CAPTURE_STA_HIGH|=0x40000000;		//��ǲ�����������  ��D6=1
							TIM_SetCounter(TIM5,0);
							TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
							TIM_Cmd(TIM5,ENABLE); 	//ʹ�ܶ�ʱ��5
						}else  //(D6==0)	������1xia�أ�һ���¼����Ŀ�ʼ
						{
							TIM5CH1_CAPTURE_STA_LOW=0;			//���
							TIM5CH1_CAPTURE_VAL_LOW=0;
							TIM5CH1_CAPTURE_STA_LOW|=0x40000000;		//��ǲ�����������  ��D6=1
							TIM_Cmd(TIM5,DISABLE ); 	//�رն�ʱ��5
							TIM_SetCounter(TIM5,0);
							TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);		//CC1P=1 ����Ϊ�½��ز���
							TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
						}		    
					}
	        }
			else //�Ѿ��ڸߵ�ƽ���λ�ã����Σ�����
			{
			     //if1 
					if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//���
					{	  
							if((TIM5CH1_CAPTURE_STA_HIGH&0x3FFFFFFF)==0x3FFFFFFF)//�ߵ�ƽ̫����
							{
								TIM5CH1_CAPTURE_STA_HIGH|=0x80000000;		//��ǳɹ�������һ��
 								TIM5CH1_CAPTURE_VAL_HIGH=0xFFFFFFFF;
							}else TIM5CH1_CAPTURE_STA_HIGH++;
					}
					//if2
					if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����1���������¼�
					{	
						   			
							TIM5CH1_CAPTURE_STA_HIGH|=0x80000000;		//��ǳɹ�����һ�θߵ�ƽ����
 						  TIM5CH1_CAPTURE_VAL_HIGH=TIM_GetCapture1(TIM5);//��ȡ��ǰ�Ĳ���ֵ.
							TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling ); //CC1P=0 ����Ϊ�����ز���
					 	    
					}
			}
	    }
		else        //�ɹ�������һ������
		{
			//��ʾʱ�� ��λus
			totaltemp=0;ftemp=0;
			temp=TIM5CH1_CAPTURE_STA_LOW&0X3FFFFFFF; 
			temp*=0XFFFFFFFF;		 		         //���cishu�ܺ�
			temp+=TIM5CH1_CAPTURE_VAL_LOW;	//�õ��ܵļ������� ÿ����250ns
			temp/=84;totaltemp+=temp; //us
			tlow=(long long)temp; 
			temp*=10000;
			
			//��HIGH
			temp=TIM5CH1_CAPTURE_STA_HIGH&0X3FFFFFFF; 
			temp*=0XFFFFFFFF;		 		         //���cishu�ܺ�
			temp+=TIM5CH1_CAPTURE_VAL_HIGH;	//�õ��ܵĸߵ�ƽʱ��
			temp/=84;totaltemp+=temp;
			ftemp=1/totaltemp*1000000;//us
			temp*=10000;
			totaltemp*=10000;
			//��ʾƵ�� ��λHz

//		//	thigh=(long long)ftemp;
//			sprintf(strf,"%-04f",ftemp);
//			GUI_Text(164,110,strf,Black,White);  
			
			TIM5CH1_CAPTURE_STA_HIGH=0;			     //������һ�β���
			TIM5CH1_CAPTURE_STA_LOW=0;			     //������һ�β���
		}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
}



//double Measure_Frequency(void)
//{
//	int CaptureNumber = 0;
//	int counter;
//	double freq;
//	u32 Time;
//while(1)
//{
//    if(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC1)==SET)
//    {
//       TIM_ClearFlag(TIM1,TIM_FLAG_CC1);
//       if(CaptureNumber == 0)
//       {
//         counter = TIM_GetCapture1(TIM1);  //��һ�β���
//         CaptureNumber = 1;
//         
//       }
//       else if(CaptureNumber == 1)  //����ڶ��β���
//       {
//         if(TIM_GetFlagStatus(TIM1,TIM_FLAG_Update) != SET)//���β����û�з�������Ĵ���
//         {
//           Time = TIM_GetCapture1(TIM1);
//           Time = Time - counter;
//         }
//         else
//         {
//           TIM_ClearFlag(TIM1,TIM_FLAG_Update);   //�����˸����¼�
//           Time = 0xFFFFFFFF - counter + TIM_GetCapture1(TIM1)+1;  //����и����¼�����ʱ��ļ��㷽ʽ
//         }
//         CaptureNumber = 0;
//         if(Time!=0)
//         {
//           freq= 2000000.0/Time;  //����Ƶ��
//					 
//					 break;
//         }
//				 
//         //freq = freq;  //�������freq���������Ż���
//				 
//       }
//    }
//}
//return freq;
//}

//void TIM1_Int_Init(u16 arr,u16 psc)		//�ⷽ��Ƶ����
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  ///ʹ��TIM3ʱ��
//	
//	
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_Init(GPIOE,&GPIO_InitStructure);

//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //���������á�
//  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //����������
//  TIM_TimeBaseInitStructure.TIM_Prescaler = 84-1;   //Timer clock = sysclock /(TIM_Prescaler+1) = 2M  ʱ��Ƶ��
//  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
//  TIM_TimeBaseInitStructure.TIM_Period = 0xFFFFFFFF;    //Period = (TIM counter clock / TIM output clock) - 1 = 40Hz  
//  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);

//  TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
//  TIM_ICInitStructure.TIM_ICFilter = 0;
//  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//  TIM_ICInit(TIM1,&TIM_ICInitStructure);
//	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
//	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);
//  TIM_Cmd(TIM1,ENABLE);
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM1_TRG_COM_TIM11_IRQn; //��ʱ��3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}