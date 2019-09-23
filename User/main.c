/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "LCD/LCD.h"
#include <stdio.h>
#include "led.h" 
#include "tim.h" 
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "math.h" 
#include "arm_math.h"  
#include "AD7176Z.h"
#include "key.h"
#include "exti.h"
#define FFT_LENGTH		4096 		//FFT长度
float fft_inputbuf[FFT_LENGTH*2];	//FFT输入数组
float fft_outputbuf[FFT_LENGTH];	//FFT输出数组
extern double ftemp;
int xiebo;
u32 adcd[4096];
u32 Volt;
float kk=1;
char stri[32];
float dc;
extern char strf[32];
static int cheng10=1; //=1不乘10，=10乘10
void Delay_SPI(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
int main(void)
{
	u16 i;u16 adcx;
	double temp;//模拟量
	arm_cfft_radix4_instance_f32 scfft;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	KEY_Init(); 	//初始化各IO口 
	EXTIX_Init();
	//Adc_Init(); 
	delay_init(168); 
	LED_Init();		           //初始化LED端口
	//uart_init(115200);
	LCD_Initializtion();
	LCD_Clear(White);
	GUI_Text(10,10,"ADC7176_ID:",Black,White);
	//GUI_Text(30,30,"ADC7176_VOL:",Black,White); 
	GUI_Text(10,30,"Harmony freq:",Black,White);
	GUI_Text(10,50,"Amplitude",Black,White);
	GUI_Text(10,70,"Spectrum 1",Black,White);
	GUI_Text(10,90,"Spectrum 2",Black,White);
	GUI_Text(10,110,"Spectrum 3",Black,White);
	GUI_Text(10,130,"Spectrum 4",Black,White);
	GUI_Text(10,150,"Spectrum 5",Black,White);
	GUI_Text(10,170,"Spectrum 6",Black,White);
	GUI_Text(10,190,"Spectrum 7",Black,White);
	GUI_Text(10,210,"Spectrum 8",Black,White);
	GUI_Text(10,230,"Spectrum 9",Black,White);
	arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1);//初始化scfft结构体，设定FFT相关参数
	///////////////////////AD7176初始化////////////////////////////
	AT45DBXX_Init();
	AT45DBXX_Enable;
	SPI_I2S_ReceiveData(Open_SPIx);
	SPI_Send_byte(0xFF);SPI_Send_byte(0xFF);SPI_Send_byte(0xFF);SPI_Send_byte(0xFF);SPI_Send_byte(0xFF);SPI_Send_byte(0xFF);SPI_Send_byte(0xFF);SPI_Send_byte(0xFF);
	delay_us(500);
	AT45DBXX_Disable;		
	AD7176_Read_ID();AD7176_Read_ID();AD7176_Read_ID();AD7176_Read_ID();AD7176_Read_ID();
	AT45DBXX_Enable;
	SPI_Send_byte(0X20); //设置配置寄存器
	SPI_Send_byte(0X10);
	SPI_Send_byte(0X00);
	delay_us(50);
	AT45DBXX_Disable;		
	
//	AT45DBXX_Enable;
//	SPI_Send_byte(0X10); //打开 次通道 ×10通道
//	SPI_Send_byte(0X80);
//	SPI_Send_byte(0X01);
//	delay_us(50);
//	SPI_Send_byte(0X12); //关 主通道
//	SPI_Send_byte(0X00);
//	SPI_Send_byte(0X43);
//	delay_us(50);
//	AT45DBXX_Disable;

	AT45DBXX_Enable;
	SPI_Send_byte(0X10); //关闭 次通道 ×10通道
	SPI_Send_byte(0X00);
	SPI_Send_byte(0X00);
	delay_us(50);
	SPI_Send_byte(0X12); //开 主通道
	SPI_Send_byte(0X80);
	SPI_Send_byte(0X43);
	delay_us(50);
	AT45DBXX_Disable;

	///////////////////////AD7176初始化结束////////////////////////////	

	/* Infinite loop */
	while (1)
	{		
		if(cheng10==1){
			AT45DBXX_Enable;
			SPI_Send_byte(0X10); //关闭 次通道 ×10通道
			SPI_Send_byte(0X00);
			SPI_Send_byte(0X00);
			delay_us(50);
			SPI_Send_byte(0X12); //开 主通道
			SPI_Send_byte(0X80);
			SPI_Send_byte(0X43);
			delay_us(50);
			AT45DBXX_Disable;

		TIM5_CH1_Cap_Init(0xFFFFFFFF,1-1); //以1Mhz的频率计数 （TIM5为32位计数器）
		delay_ms(1000);
		TIM_Cmd(TIM5,DISABLE);
		//====================================================
		sprintf(strf,"%f",ftemp);
		xiebo=(int)(ftemp/5);
		GUI_Text(120,30,strf,Black,White);
		GUI_Text(200,30,"Hz",Black,White);
		TIM3_Int_Init(14-1,293-1);//设置采样频率
		delay_ms(2000);
		TIM_Cmd(TIM3,DISABLE); 
				//====================================================
		
//		sprintf(stri,"%04f",(float)(Volt/1677.7216)-5000.0);
//		//GUI_Num(134,150,(float)(Volt/6710.886-1250),4,Red,White);
//		GUI_Text(134,90,stri,Black,White); 
		//GUI_Text(166,150,"mV",Black,White); 
		////////////////////////////////对adc转化的序列进行FFT变换（输入为adcd[4096]）/////////////////////////////////////
		for(i=0;i<FFT_LENGTH;i++)//生成信号序列
		{
				fft_inputbuf[2*i]= (float)(adcd[i]/1677.7216)-5000.0;   //生成输入信号实部
				fft_inputbuf[2*i+1]=0;//虚部全部为0
		}
		dc=0;
		for(i=0;i<FFT_LENGTH;i++){dc=dc+fft_inputbuf[2*i]/FFT_LENGTH;}
		for(i=0;i<FFT_LENGTH;i++){fft_inputbuf[2*i]=fft_inputbuf[2*i]-dc;}
		//delay_ms(10); 
		arm_cfft_radix4_f32(&scfft,fft_inputbuf);
		//delay_ms(10);
		arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);
		//全频谱
		temp=0;
		for(i=0;i<FFT_LENGTH;i++)
		{
		temp=sqrt(temp*temp+(fft_outputbuf[i]/(FFT_LENGTH/2))*(fft_outputbuf[i]/(FFT_LENGTH/2)));
		}
		adcx=temp/cheng10*10.177-526.13;
		//adcx=20*(int)(kk*temp);
		GUI_Text(150,50,"mV",Black,White); 
		GUI_Num(100,50,adcx,5,Red,White);
		GUI_Text(220,50,"mA",Black,White); 
		GUI_Num(185,50,adcx/10,4,Red,White);
		//基波
		temp=0;
		for(i=0;i<11;i++)
		{
		temp=sqrt(temp*temp+(fft_outputbuf[xiebo-5+i]/(FFT_LENGTH/2))*(fft_outputbuf[xiebo-5+i]/(FFT_LENGTH/2)));
		}
		temp=fft_outputbuf[xiebo]/(FFT_LENGTH/2);
		adcx=temp/cheng10*13.793+ 38.7;
		//adcx=20*(int)(kk*temp);
		GUI_Text(150,70,"mV",Black,White); 
		GUI_Num(100,70,adcx,5,Red,White);
		GUI_Text(220,70,"mA",Black,White); 
		GUI_Num(185,70,adcx/10,4,Red,White);
		//2次
		temp=fft_outputbuf[xiebo*2]/(FFT_LENGTH/2);
		adcx=temp/cheng10*13.793+ 38.7;
		//adcx=20*(int)(kk*temp);
		GUI_Text(150,90,"mV",Black,White); 
		GUI_Num(100,90,adcx,5,Red,White);
		GUI_Text(220,90,"mA",Black,White); 
		GUI_Num(185,90,adcx/10,4,Red,White);
		//3次
		//temp=fft_outputbuf[xiebo*3]/(FFT_LENGTH/2);
		temp=0;
		for(i=0;i<11;i++)
		{
		temp=sqrt(temp*temp+(fft_outputbuf[3*xiebo-5+i]/(FFT_LENGTH/2))*(fft_outputbuf[3*xiebo-5+i]/(FFT_LENGTH/2)));
		}
		adcx=temp/cheng10*13.793+ 38.7;
		//adcx=20*(int)(kk*temp);
		GUI_Text(150,110,"mV",Black,White); 
		GUI_Num(100,110,adcx,5,Red,White);    
				GUI_Text(220,110,"mA",Black,White); 
		GUI_Num(185,110,adcx/10,4,Red,White);
		//4次
		temp=fft_outputbuf[xiebo*4]/(FFT_LENGTH/2);
		adcx=temp/cheng10*13.793+ 38.7;
		//adcx=20*(int)(kk*temp);
		GUI_Text(150,130,"mV",Black,White); 
		GUI_Num(100,130,adcx,5,Red,White);    
				GUI_Text(220,130,"mA",Black,White); 
		GUI_Num(185,130,adcx/10,4,Red,White);
	//5次
		temp=0;
		for(i=0;i<11;i++)
		{
		temp=sqrt(temp*temp+(fft_outputbuf[5*xiebo-5+i]/(FFT_LENGTH/2))*(fft_outputbuf[5*xiebo-5+i]/(FFT_LENGTH/2)));
		}
		adcx=temp/cheng10*13.793+ 38.7;
		//adcx=20*(int)(kk*temp);
		GUI_Text(150,150,"mV",Black,White); 
		GUI_Num(100,150,adcx,5,Red,White);    
		GUI_Text(220,150,"mA",Black,White); 
		GUI_Num(185,150,adcx/10,4,Red,White);
		//6次
		temp=fft_outputbuf[xiebo*6]/(FFT_LENGTH/2);
		adcx=temp/cheng10*13.793+ 38.7;
		//adcx=20*(int)(kk*temp);
		GUI_Text(150,170,"mV",Black,White); 
		GUI_Num(100,170,adcx,5,Red,White);    
		GUI_Text(220,170,"mA",Black,White); 
		GUI_Num(185,170,adcx/10,4,Red,White);
		//7次
		temp=0;
		for(i=0;i<11;i++)
		{
		temp=sqrt(temp*temp+(fft_outputbuf[7*xiebo-5+i]/(FFT_LENGTH/2))*(fft_outputbuf[7*xiebo-5+i]/(FFT_LENGTH/2)));
		}
		adcx=temp/cheng10*13.793+ 38.7;
		//adcx=20*(int)(kk*temp);
		GUI_Text(150,190,"mV",Black,White); 
		GUI_Num(100,190,adcx,5,Red,White);    
				GUI_Text(220,190,"mA",Black,White); 
		GUI_Num(185,190,adcx/10,4,Red,White);
		//8次
		temp=fft_outputbuf[xiebo*8]/(FFT_LENGTH/2);
		adcx=temp/cheng10*13.793+ 38.7;
		//adcx=20*(int)(kk*temp);
		GUI_Text(150,210,"mV",Black,White); 
		GUI_Num(100,210,adcx,5,Red,White);    
		GUI_Text(220,210,"mA",Black,White); 
		GUI_Num(185,210,adcx/10,4,Red,White);
		//9次
		temp=0;
		for(i=0;i<11;i++)
		{
		temp=sqrt(temp*temp+(fft_outputbuf[9*xiebo-5+i]/(FFT_LENGTH/2))*(fft_outputbuf[9*xiebo-5+i]/(FFT_LENGTH/2)));
		}
		adcx=temp/cheng10*13.793+ 38.7;
		//adcx=20*(int)(kk*temp);
		GUI_Text(150,230,"mV",Black,White);
		GUI_Num(100,230,adcx,5,Red,White);
				GUI_Text(220,230,"mA",Black,White); 
		GUI_Num(185,230,adcx/10,4,Red,White);
		
		GUI_Num(250,10,cheng10,2,Red,White);
		temp=0;
		for(i=0;i<11;i++)
		{
		temp=sqrt(temp*temp+(fft_outputbuf[xiebo-5+i]/(FFT_LENGTH/2))*(fft_outputbuf[xiebo-5+i]/(FFT_LENGTH/2)));
		}
		//adcx=temp/cheng10;
		adcx=20*(int)(kk*temp);
		if(adcx<1000){
			cheng10=12;
		}
	}
//==============判断进入B
		if(cheng10==12){
//				AT45DBXX_Enable;
//				SPI_Send_byte(0X10); //打开 次通道 ×10通道
//				SPI_Send_byte(0X80);
//				SPI_Send_byte(0X01);
//				delay_us(50);
//				SPI_Send_byte(0X12); //关 主通道
//				SPI_Send_byte(0X00);
//				SPI_Send_byte(0X43);
//				delay_us(50);
//				AT45DBXX_Disable;
//				cheng10=12;
			
		TIM5_CH1_Cap_Init(0xFFFFFFFF,1-1); //以1Mhz的频率计数 （TIM5为32位计数器）
		delay_ms(1000);
		TIM_Cmd(TIM5,DISABLE );
		//====================================================
		sprintf(strf,"%f",ftemp);
		xiebo=(int)(ftemp/5);
		//xiebo*=2;
		GUI_Text(120,30,strf,Black,White);
		GUI_Text(200,30,"Hz",Black,White);
		TIM3_Int_Init(14-1,293-1);//设置采样频率
		delay_ms(2000);
		TIM_Cmd(TIM3,DISABLE); 
//====================================================

		for(i=0;i<FFT_LENGTH;i++)//生成信号序列
		{
				fft_inputbuf[2*i]= ((float)(adcd[i]/1677.7216)-5000.0);   //生成输入信号实部
				fft_inputbuf[2*i+1]=0;//虚部全部为0
		}
		dc=0;
		for(i=0;i<FFT_LENGTH;i++){dc=dc+fft_inputbuf[2*i]/FFT_LENGTH;}
		for(i=0;i<FFT_LENGTH;i++){fft_inputbuf[2*i]=fft_inputbuf[2*i]-dc;}
		//delay_ms(10); 
		arm_cfft_radix4_f32(&scfft,fft_inputbuf);
		//delay_ms(10);
		arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,FFT_LENGTH);

		//全频谱
				temp=0;
		for(i=0;i<FFT_LENGTH;i++)
		{
		temp=sqrt(temp*temp+(fft_outputbuf[i]/(FFT_LENGTH/2))*(fft_outputbuf[i]/(FFT_LENGTH/2)));
		}
		//adcx=temp/cheng10*13.793+38.621;
		adcx=20*(int)(kk*temp);
		GUI_Text(150,50,"mV",Black,White); 
		GUI_Num(100,50,adcx,5,Red,White);
				GUI_Text(220,50,"mA",Black,White); 
		GUI_Num(185,50,adcx/10,4,Red,White);
		//1次
		temp=0;
		for(i=0;i<11;i++)
		{
		temp=sqrt(temp*temp+(fft_outputbuf[xiebo-5+i]/(FFT_LENGTH/2))*(fft_outputbuf[xiebo-5+i]/(FFT_LENGTH/2)));
		}
		//adcx=temp/cheng10*13.793+38.621;
		adcx=20*(int)(kk*temp);
		GUI_Text(150,70,"mV",Black,White); 
		GUI_Num(100,70,adcx,5,Red,White);    
				GUI_Text(220,70,"mA",Black,White); 
		GUI_Num(185,70,adcx/10,4,Red,White);
		
		//2次
		temp=fft_outputbuf[xiebo*2]/(FFT_LENGTH/2);
		//adcx=temp/cheng10*13.793+38.621;
		adcx=20*(int)(kk*temp);
		GUI_Text(150,90,"mV",Black,White); 
		GUI_Num(100,90,adcx,5,Red,White);
		GUI_Text(220,90,"mA",Black,White); 
		GUI_Num(185,90,adcx/10,4,Red,White);
		//3次
		temp=0;
		for(i=0;i<11;i++)
		{
		temp=sqrt(temp*temp+(fft_outputbuf[3*xiebo-5+i]/(FFT_LENGTH/2))*(fft_outputbuf[3*xiebo-5+i]/(FFT_LENGTH/2)));
		}
		//adcx=temp/cheng10*13.793+38.621;
		adcx=20*(int)(kk*temp);
		GUI_Text(150,110,"mV",Black,White); 
		GUI_Num(100,110,adcx,5,Red,White);    
				GUI_Text(220,110,"mA",Black,White); 
		GUI_Num(185,110,adcx/10,4,Red,White);
		//4次
		temp=fft_outputbuf[xiebo*4]/(FFT_LENGTH/2);
		//adcx=temp/cheng10*13.793+38.621;
		adcx=20*(int)(kk*temp);
		GUI_Text(150,130,"mV",Black,White); 
		GUI_Num(100,130,adcx,5,Red,White);    
				GUI_Text(220,130,"mA",Black,White); 
		GUI_Num(185,130,adcx/10,4,Red,White);
		//5次
		temp=0;
		for(i=0;i<11;i++)
		{
		temp=sqrt(temp*temp+(fft_outputbuf[5*xiebo-5+i]/(FFT_LENGTH/2))*(fft_outputbuf[5*xiebo-5+i]/(FFT_LENGTH/2)));
		}
		//adcx=temp/cheng10*13.793+38.621;
		adcx=20*(int)(kk*temp);
		GUI_Text(150,150,"mV",Black,White); 
		GUI_Num(100,150,adcx,5,Red,White);    
		GUI_Text(220,150,"mA",Black,White); 
		GUI_Num(185,150,adcx/10,4,Red,White);
		//6次
		temp=fft_outputbuf[xiebo*6]/(FFT_LENGTH/2);
		//adcx=temp/cheng10*13.793+38.621;
		adcx=20*(int)(kk*temp);
		GUI_Text(150,170,"mV",Black,White); 
		GUI_Num(100,170,adcx,5,Red,White);    
				GUI_Text(220,170,"mA",Black,White); 
		GUI_Num(185,170,adcx/10,4,Red,White);
//7次
		temp=0;
		for(i=0;i<11;i++)
		{
		temp=sqrt(temp*temp+(fft_outputbuf[7*xiebo-5+i]/(FFT_LENGTH/2))*(fft_outputbuf[7*xiebo-5+i]/(FFT_LENGTH/2)));
		}
		//adcx=temp/cheng10*13.793+38.621;
		adcx=20*(int)(kk*temp);
		GUI_Text(150,190,"mV",Black,White); 
		GUI_Num(100,190,adcx,5,Red,White);    
				GUI_Text(220,190,"mA",Black,White); 
		GUI_Num(185,190,adcx/10,4,Red,White);
		//8次
		temp=fft_outputbuf[xiebo*8]/(FFT_LENGTH/2);
		//adcx=temp/cheng10*13.793+38.621;
		adcx=20*(int)(kk*temp);
		GUI_Text(150,210,"mV",Black,White); 
		GUI_Num(100,210,adcx,5,Red,White);    
		GUI_Text(220,210,"mA",Black,White); 
		GUI_Num(185,210,adcx/10,4,Red,White);
		//9次
		temp=0;
		for(i=0;i<11;i++)
		{
		temp=sqrt(temp*temp+(fft_outputbuf[9*xiebo-5+i]/(FFT_LENGTH/2))*(fft_outputbuf[9*xiebo-5+i]/(FFT_LENGTH/2)));
		}
		//adcx=temp/cheng10*13.793+38.621;
		adcx=20*(int)(kk*temp);
		GUI_Text(150,230,"mV",Black,White); 
		GUI_Num(100,230,adcx,5,Red,White);
				GUI_Text(220,230,"mA",Black,White); 
		GUI_Num(185,230,adcx/10,4,Red,White);
		
		GUI_Num(250,10,cheng10,2,Red,White);
		
		temp=0;
		for(i=0;i<11;i++)
		{
			temp=sqrt(temp*temp+(fft_outputbuf[xiebo-5+i]/(FFT_LENGTH/2))*(fft_outputbuf[xiebo-5+i]/(FFT_LENGTH/2)));
		}
		//adcx=temp/cheng10;
		adcx=20*(int)(kk*temp);
		if(adcx>1200){
			cheng10=1;
		}
		}	
	
	}
}
