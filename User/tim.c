#include "tim.h"
#include "led.h"
#include "adc.h"
#include "AD7176Z.h"
#include "LCD/LCD.h"
//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
u16 k;u16 DAT1;u16 DAT2;u16 DAT3;
extern u32 adcd[4096];extern u32 Volt;

long long tlow,thigh,ttotal;
double temp=0,totaltemp=0,ftemp;
char strf[32];

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		AT45DBXX_Enable;	
		while(PCin(2));
		Volt=0;
		SPI_Send_byte(0X44); //读数据寄存器
		DAT1=SPI_Receive_byte();
		DAT2=SPI_Receive_byte();
		DAT3=SPI_Receive_byte();
		Volt=(Volt|DAT1)&0x0000ff;
		Volt=(((Volt<<8)&0x00ff00)|DAT2)&0x00ffff;
		Volt=(((Volt<<8)&0xffff00)|DAT3)&0xffffff;
		adcd[k%4096]=Volt;//Volt为读取到的24位数字量
		k++;
		AT45DBXX_Disable;
	}
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}


TIM_ICInitTypeDef  TIM5_ICInitStructure;

//定时器5通道1输入捕获配置
//arr：自动重装值(TIM2,TIM5是32位的!!)
//psc：时钟预分频数
void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //下拉-----------------
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //PA0复用位定时器5
  
	//初始化 TIM5,设置 TIM5 的 ARR 和 PSC。  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	

	//3）设置 TIM5 的输入捕获参数，开启输入捕获。
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//上升沿捕获
  TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5

 
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	
}
//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
//u8  TIM5CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u32 TIM5CH1_CAPTURE_STA_LOW=0;
u32 TIM5CH1_CAPTURE_STA_HIGH=0;
u32	TIM5CH1_CAPTURE_VAL_LOW;	//输入捕获值(TIM2/TIM5是32位)
u32	TIM5CH1_CAPTURE_VAL_HIGH;	//输入捕获值(TIM2/TIM5是32位)
u32 tem;
//定时器5中断服务程序	 
void TIM5_IRQHandler(void)
{ 	  
	 	if((TIM5CH1_CAPTURE_STA_HIGH&0x80000000)==0)//还未成功捕获到一个周期
        {
	        if((TIM5CH1_CAPTURE_STA_LOW&0x80000000)==0)////还未成功捕获到一个低电平
	        {
					//if1,其实不太可能发生的
					if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//溢出
					{	     
						
						if(TIM5CH1_CAPTURE_STA_LOW&0x40000000)//已经捕获到高电平了
						{
							if((TIM5CH1_CAPTURE_STA_LOW&0x3FFFFFFF)==0x3FFFFFFF)//高电平太长了
							{
								TIM5CH1_CAPTURE_STA_LOW|=0x80000000;		//标记成功捕获了一次
 								TIM5CH1_CAPTURE_VAL_LOW=0xFFFFFFFF;
	                             TIM5CH1_CAPTURE_STA_HIGH=0;			//清空
							    TIM5CH1_CAPTURE_VAL_HIGH=0;
							    TIM5CH1_CAPTURE_STA_HIGH|=0x40000000;		//标记捕获到了上升沿  ，D6=1
							    TIM_SetCounter(TIM5,0);
							    TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
							    TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
							}else TIM5CH1_CAPTURE_STA_LOW++;
						}	 
					}//have been checked
					//if2
					if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
					{	
 						if(TIM5CH1_CAPTURE_STA_LOW&0x40000000) 	//捕获到了下降沿，成功捕获到一次高电平脉宽
						{	  			
							TIM5CH1_CAPTURE_STA_LOW|=0x80000000;		//标记成功捕获到一次高电平脉宽
						    TIM5CH1_CAPTURE_VAL_LOW=TIM_GetCapture1(TIM5);//获取当前的捕获值.
 							TIM5CH1_CAPTURE_STA_HIGH=0;			//清空
							TIM5CH1_CAPTURE_VAL_HIGH=0;
							TIM5CH1_CAPTURE_STA_HIGH|=0x40000000;		//标记捕获到了上升沿  ，D6=1
							TIM_SetCounter(TIM5,0);
							TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
							TIM_Cmd(TIM5,ENABLE); 	//使能定时器5
						}else  //(D6==0)	捕获到了1xia沿，一次新计数的开始
						{
							TIM5CH1_CAPTURE_STA_LOW=0;			//清空
							TIM5CH1_CAPTURE_VAL_LOW=0;
							TIM5CH1_CAPTURE_STA_LOW|=0x40000000;		//标记捕获到了上升沿  ，D6=1
							TIM_Cmd(TIM5,DISABLE ); 	//关闭定时器5
							TIM_SetCounter(TIM5,0);
							TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);		//CC1P=1 设置为下降沿捕获
							TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
						}		    
					}
	        }
			else //已经在高电平这个位置（后半段）上了
			{
			     //if1 
					if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//溢出
					{	  
							if((TIM5CH1_CAPTURE_STA_HIGH&0x3FFFFFFF)==0x3FFFFFFF)//高电平太长了
							{
								TIM5CH1_CAPTURE_STA_HIGH|=0x80000000;		//标记成功捕获了一次
 								TIM5CH1_CAPTURE_VAL_HIGH=0xFFFFFFFF;
							}else TIM5CH1_CAPTURE_STA_HIGH++;
					}
					//if2
					if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
					{	
						   			
							TIM5CH1_CAPTURE_STA_HIGH|=0x80000000;		//标记成功捕获到一次高电平脉宽
 						  TIM5CH1_CAPTURE_VAL_HIGH=TIM_GetCapture1(TIM5);//获取当前的捕获值.
							TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling ); //CC1P=0 设置为上升沿捕获
					 	    
					}
			}
	    }
		else        //成功捕获到了一个周期
		{
			//显示时间 单位us
			totaltemp=0;ftemp=0;
			temp=TIM5CH1_CAPTURE_STA_LOW&0X3FFFFFFF; 
			temp*=0XFFFFFFFF;		 		         //溢出cishu总和
			temp+=TIM5CH1_CAPTURE_VAL_LOW;	//得到总的计数次数 每次是250ns
			temp/=84;totaltemp+=temp; //us
			tlow=(long long)temp; 
			temp*=10000;
			
			//算HIGH
			temp=TIM5CH1_CAPTURE_STA_HIGH&0X3FFFFFFF; 
			temp*=0XFFFFFFFF;		 		         //溢出cishu总和
			temp+=TIM5CH1_CAPTURE_VAL_HIGH;	//得到总的高电平时间
			temp/=84;totaltemp+=temp;
			ftemp=1/totaltemp*1000000;//us
			temp*=10000;
			totaltemp*=10000;
			//显示频率 单位Hz

//		//	thigh=(long long)ftemp;
//			sprintf(strf,"%-04f",ftemp);
//			GUI_Text(164,110,strf,Black,White);  
			
			TIM5CH1_CAPTURE_STA_HIGH=0;			     //开启下一次捕获
			TIM5CH1_CAPTURE_STA_LOW=0;			     //开启下一次捕获
		}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
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
//         counter = TIM_GetCapture1(TIM1);  //第一次捕获
//         CaptureNumber = 1;
//         
//       }
//       else if(CaptureNumber == 1)  //处理第二次捕获
//       {
//         if(TIM_GetFlagStatus(TIM1,TIM_FLAG_Update) != SET)//两次捕获间没有发生溢出的处理
//         {
//           Time = TIM_GetCapture1(TIM1);
//           Time = Time - counter;
//         }
//         else
//         {
//           TIM_ClearFlag(TIM1,TIM_FLAG_Update);   //产生了更新事件
//           Time = 0xFFFFFFFF - counter + TIM_GetCapture1(TIM1)+1;  //如果有更新事件产生时候的计算方式
//         }
//         CaptureNumber = 0;
//         if(Time!=0)
//         {
//           freq= 2000000.0/Time;  //计算频率
//					 
//					 break;
//         }
//				 
//         //freq = freq;  //避免变量freq被编译器优化掉
//				 
//       }
//    }
//}
//return freq;
//}

//void TIM1_Int_Init(u16 arr,u16 psc)		//测方波频率用
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  ///使能TIM3时钟
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

//	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //死区控制用。
//  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  //计数器方向
//  TIM_TimeBaseInitStructure.TIM_Prescaler = 84-1;   //Timer clock = sysclock /(TIM_Prescaler+1) = 2M  时钟频率
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
//	NVIC_InitStructure.NVIC_IRQChannel=TIM1_TRG_COM_TIM11_IRQn; //定时器3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}