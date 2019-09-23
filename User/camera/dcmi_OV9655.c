/**
  ******************************************************************************
  * @file    DCMI/OV9655_Camera/dcmi_OV9655.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   This file includes the driver for OV9655 Camera module mounted on 
  *          STM322xG-EVAL board RevA and RevB.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "camera/dcmi_OV9655.h"

/** @addtogroup DCMI_OV9655_Camera
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  TIMEOUT  2

static unsigned char OV9655_QQVGA[][2]=
{
  0x00, 0x00,
  0x01, 0x80,
  0x02, 0x80,
  0x03, 0x02,
  0x04, 0x03,
  0x09, 0x01,
  0x0b, 0x57,
  0x0e, 0x61,
  0x0f, 0x40,
  0x11, 0x01,
  0x12, 0x62,
  0x13, 0xc7,
  0x14, 0x3a,
  0x16, 0x24,
  0x17, 0x18,
  0x18, 0x04,
  0x19, 0x01,
  0x1a, 0x81,
  0x1e, 0x00,//0x20
  0x24, 0x3c,
  0x25, 0x36,
  0x26, 0x72,
  0x27, 0x08,
  0x28, 0x08,
  0x29, 0x15,
  0x2a, 0x00,
  0x2b, 0x00,
  0x2c, 0x08,
  0x32, 0xa4,
  0x33, 0x00,
  0x34, 0x3f,
  0x35, 0x00,
  0x36, 0x3a,
  0x38, 0x72,
  0x39, 0x57,
  0x3a, 0xcc,
  0x3b, 0x04,
  0x3d, 0x99,
  0x3e, 0x0e,
  0x3f, 0xc1,

  0x40, 0x1f,

  0x41, 0x41,
  0x42, 0xc0,
  0x43, 0x0a,
  0x44, 0xf0,
  0x45, 0x46,
  0x46, 0x62,
  0x47, 0x2a,
  0x48, 0x3c,
  0x4a, 0xfc,
  0x4b, 0xfc,
  0x4c, 0x7f,
  0x4d, 0x7f,
  0x4e, 0x7f,
  0x4f, 0x98,
  0x50, 0x98,
  0x51, 0x00,
  0x52, 0x28,
  0x53, 0x70,
  0x54, 0x98,
  0x58, 0x1a,
  0x59, 0x85,
  0x5a, 0xa9,
  0x5b, 0x64,
  0x5c, 0x84,
  0x5d, 0x53,
  0x5e, 0x0e,
  0x5f, 0xf0,
  0x60, 0xf0,
  0x61, 0xf0,
  0x62, 0x00,
  0x63, 0x00,
  0x64, 0x02,
  0x65, 0x20,
  0x66, 0x00,
  0x69, 0x0a,
  0x6b, 0x5a,
  0x6c, 0x04,
  0x6d, 0x55,
  0x6e, 0x00,
  0x6f, 0x9d,
  0x70, 0x21,
  0x71, 0x78,
  0x72, 0x22,
  0x73, 0x02,
  0x74, 0x10,
  0x75, 0x10,
  0x76, 0x01,
  0x77, 0x02,
  0x7A, 0x12,
  0x7B, 0x08,
  0x7C, 0x16,
  0x7D, 0x30,
  0x7E, 0x5e,
  0x7F, 0x72,
  0x80, 0x82,
  0x81, 0x8e,
  0x82, 0x9a,
  0x83, 0xa4,
  0x84, 0xac,
  0x85, 0xb8,
  0x86, 0xc3,
  0x87, 0xd6,
  0x88, 0xe6,
  0x89, 0xf2,
  0x8a, 0x24,
  0x8c, 0x80,
  0x90, 0x7d,
  0x91, 0x7b,
  0x9d, 0x02,
  0x9e, 0x02,
  0x9f, 0x7a,
  0xa0, 0x79,
  0xa1, 0x40,
  0xa4, 0x50,
  0xa5, 0x68,
  0xa6, 0x4a,
  0xa8, 0xc1,
  0xa9, 0xef,
  0xaa, 0x92,
  0xab, 0x04,
  0xac, 0x80,
  0xad, 0x80,
  0xae, 0x80,
  0xaf, 0x80,
  0xb2, 0xf2,
  0xb3, 0x20,
  0xb4, 0x20,
  0xb5, 0x00,
  0xb6, 0xaf,
  0xbb, 0xae,
  0xbc, 0x7f,
  0xbd, 0x7f,
  0xbe, 0x7f,
  0xbf, 0x7f,
  0xbf, 0x7f,
  0xc0, 0xaa,
  0xc1, 0xc0,
  0xc2, 0x01,
  0xc3, 0x4e,
  0xc6, 0x05,
  0xc7, 0x82,
  0xc9, 0xe0,
  0xca, 0xe8,
  0xcb, 0xf0,
  0xcc, 0xd8,
  0xcd, 0x93,

  0x12, 0x63,
  0x40, 0x10,
  0x15, 0x08,
};

/* QVGA 360x240 */
static unsigned char OV9655_QVGA[][2]=
{
  0x12, 0x80,
  0x00, 0x00,
  0x01, 0x80,
  0x02, 0x80,
  0x03, 0x02,
  0x04, 0x03,
  0x09, 0x01,
  0x0b, 0x57,
  0x0e, 0x61,
  0x0f, 0x40,
  0x11, 0x01,
  0x12, 0x62,
  0x13, 0xc7,
  0x14, 0x3a,
  0x16, 0x24,
  0x17, 0x18,
  0x18, 0x04,
  0x19, 0x01,
  0x1a, 0x81,
  0x1e, 0x00,
  0x24, 0x3c,
  0x25, 0x36,
  0x26, 0x72,
  0x27, 0x08,
  0x28, 0x08,
  0x29, 0x15,
  0x2a, 0x00,
  0x2b, 0x00,
  0x2c, 0x08,
  0x32, 0xa4,       //Q vs QQ 0x32, 0xa4
  0x33, 0x00,
  0x34, 0x3f,
  0x35, 0x00,
  0x36, 0x3a,
  0x38, 0x72,
  0x39, 0x57,
  0x3a, 0xcc,
  0x3b, 0x04,
  0x3d, 0x99,
  0x3e, 0x02,		 //Q vs QQ  0x3e, 0x0e,
  0x3f, 0xc1,
  0x40, 0xc0,
  0x41, 0x41,
  0x42, 0xc0,
  0x43, 0x0a,
  0x44, 0xf0,
  0x45, 0x46,
  0x46, 0x62,
  0x47, 0x2a,
  0x48, 0x3c,
  0x4a, 0xfc,
  0x4b, 0xfc,
  0x4c, 0x7f,
  0x4d, 0x7f,
  0x4e, 0x7f,
  0x4f, 0x98,
  0x50, 0x98,
  0x51, 0x00,
  0x52, 0x28,
  0x53, 0x70,
  0x54, 0x98,
  0x58, 0x1a,
  0x59, 0x85,
  0x5a, 0xa9,
  0x5b, 0x64,
  0x5c, 0x84,
  0x5d, 0x53,
  0x5e, 0x0e,
  0x5f, 0xf0,
  0x60, 0xf0,
  0x61, 0xf0,
  0x62, 0x00,
  0x63, 0x00,
  0x64, 0x02,
  0x65, 0x20,
  0x66, 0x00,
  0x69, 0x0a,
  0x6b, 0x5a,
  0x6c, 0x04,
  0x6d, 0x55,
  0x6e, 0x00,
  0x6f, 0x9d,
  0x70, 0x21,
  0x71, 0x78,
  0x72, 0x11,		//Q vs QQ 0x72, 0x22
  0x73, 0x01,		//Q vs QQ  0x73, 0x02
  0x74, 0x10,
  0x75, 0x10,
  0x76, 0x01,
  0x77, 0x02,
  0x7A, 0x12,
  0x7B, 0x08,
  0x7C, 0x16,
  0x7D, 0x30,
  0x7E, 0x5e,
  0x7F, 0x72,
  0x80, 0x82,
  0x81, 0x8e,
  0x82, 0x9a,
  0x83, 0xa4,
  0x84, 0xac,
  0x85, 0xb8,
  0x86, 0xc3,
  0x87, 0xd6,
  0x88, 0xe6,
  0x89, 0xf2,
  0x8a, 0x24,
  0x8c, 0x80,
  0x90, 0x7d,
  0x91, 0x7b,
  0x9d, 0x02,
  0x9e, 0x02,
  0x9f, 0x7a,
  0xa0, 0x79,
  0xa1, 0x40,
  0xa4, 0x50,
  0xa5, 0x68,
  0xa6, 0x4a,
  0xa8, 0xc1,
  0xa9, 0xef,
  0xaa, 0x92,
  0xab, 0x04,
  0xac, 0x80,
  0xad, 0x80,
  0xae, 0x80,
  0xaf, 0x80,
  0xb2, 0xf2,
  0xb3, 0x20,
  0xb4, 0x20,
  0xb5, 0x00,
  0xb6, 0xaf,
  0xb6, 0xaf,
  0xbb, 0xae,
  0xbc, 0x7f,
  0xbd, 0x7f,
  0xbe, 0x7f,
  0xbf, 0x7f,
  0xbf, 0x7f,
  0xc0, 0xaa,
  0xc1, 0xc0,
  0xc2, 0x01,
  0xc3, 0x4e,
  0xc6, 0x05,
  0xc7, 0x81,		 //Q vs QQ  0xc7, 0x82,
  0xc9, 0xe0,
  0xca, 0xe8,
  0xcb, 0xf0,
  0xcc, 0xd8,
  0xcd, 0x93,

  0x12, 0x63,
  0x40, 0x10,
  0x15, 0x08,

};

/* Bits definitions ----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void Delay(uint32_t nTime); 
static void Delay_ms(uint32_t nTime);
/**
  * @brief  Configures the DCMI to interface with the OV9655 camera module.
  * @param  None
  * @retval None
  */

uint8_t DCMI_OV9655Config(void)
{
  
  MCO1_Init();
  /* I2C1 will be used for OV9655 camera configuration */
  SCCB_GPIO_Config();

  Delay_ms(0xfff);
  /* Reset and check the presence of the OV9655 camera module */
  if(DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x12, 0x80))
  {
     return (0xFF);
  }
  Delay_ms(0xfff);

  /* OV9655 Camera size setup */    
#if defined (QQVGA_SIZE)
  DCMI_OV9655_QQVGASizeSetup();
#elif defined (QVGA_SIZE)
  DCMI_OV9655_QVGASizeSetup();
#endif 

  /* Set the RGB565 mode */
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, 0x63);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM15, 0x10);

  /* Invert the HRef signal*/
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM10, 0x08);
  
  
  /* Configure the DCMI to interface with the OV9655 camera module */
  DCMI_Config();

  return (0x00);
}

void DCMI_Config(void)
{
  DCMI_InitTypeDef DCMI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  DMA_InitTypeDef  DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable DCMI GPIOs clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE); 

  /* Enable DCMI clock */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);

  /* Connect DCMI pins to AF13 ************************************************/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI);//HSYNC 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);//PCLK

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_DCMI);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI);//VSYNC
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_DCMI);

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_DCMI);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_DCMI);
  
  /* DCMI GPIO configuration **************************************************/
    /*
  D0 -- PC6
  D1 -- PC7
  D2 -- PE0
  D3 -- PE1
  D4 -- PE4
  D5 -- PB6
  D6 -- PB8
  D7 -- PB9
  PCK - PA6
  HS -- PA4
  VS -- PB7
  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4  /* | GPIO_Pin_5 | GPIO_Pin_6*/;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  /* DCMI configuration *******************************************************/ 
  DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
  DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
  DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Falling;
  DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;
  DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_High;
  DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
  DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
  
  DCMI_Init(&DCMI_InitStructure);

  /* DCMI Interrupts config ***************************************************/
  DCMI_ITConfig(DCMI_IT_VSYNC, ENABLE);
  //DCMI_ITConfig(DCMI_IT_LINE, ENABLE);
  //DCMI_ITConfig(DCMI_IT_FRAME, ENABLE);
  //DCMI_ITConfig(DCMI_IT_ERR, ENABLE);
      
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);
  /* Configures the DMA2 to transfer Data from DCMI to the LCD ****************/
  /* Enable DMA2 clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  
  
  /* DMA2 Stream1 Configuration */  
  DMA_DeInit(DMA2_Stream1);

  DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;	
  DMA_InitStructure.DMA_Memory0BaseAddr = FSMC_LCD_ADDRESS;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 320;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
     
  DMA_Init(DMA2_Stream1, &DMA_InitStructure); 
}

/**
  * @brief  Set PA8 Output SYSCLK/2.
  * @param  None
  * @retval None
  */
void MCO1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOs clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		
	/* Configure MCO (PA8) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_MCO1Config(RCC_MCO1Source_HSI, RCC_MCO1Div_1);
}

/**
  * @brief  Reset the OV9655 SCCB registers.
  * @param  None
  * @retval None
  */
void DCMI_OV9655_Reset(void)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, SCCB_REG_RESET);    
}

/**
  * @brief  Set the QVGA size(240*320).
  * @param  None
  * @retval None
  */
void DCMI_OV9655_QVGASizeSetup(void)
{  
  uint16_t  i;
  DCMI_OV9655_Reset();    //RESET
  Delay(TIMEOUT);
    for(i=0; i<(sizeof(OV9655_QVGA)/2); i++)
  {
    DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,OV9655_QVGA[i][0], OV9655_QVGA[i][1]);
    Delay(TIMEOUT);
  }
}

/**
  * @brief  Set the QQVGA size(120*160).
  * @param  None
  * @retval None
  */
void DCMI_OV9655_QQVGASizeSetup(void)
{
  uint16_t  i;
  DCMI_OV9655_Reset();    //RESET
  Delay(TIMEOUT);
    for(i=0; i<(sizeof(OV9655_QQVGA)/2); i++)
  {
    DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,OV9655_QQVGA[i][0], OV9655_QQVGA[i][1]);
    Delay(TIMEOUT);
  }

}

/**
  * @brief  Read the OV9655 Manufacturer identifier.
  * @param  OV9655ID: pointer to the OV9655 Manufacturer identifier. 
  * @retval None
  */
void DCMI_OV9655_ReadID(OV9655_IDTypeDef* OV9655ID)
{
	uint8_t temp;
	DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_MIDH , &temp);
	OV9655ID->Manufacturer_ID1 = temp;
	DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_MIDL , &temp);
	OV9655ID->Manufacturer_ID2 = temp;
	DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_VER , &temp);
	OV9655ID->Version = temp;
	DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_PID , &temp);
	OV9655ID->PID = temp;
}

/**
  * @brief  Set the Internal Clock Prescaler.
  * @param  OV9655_Prescaler: the new value of the prescaler. 
  *         This parameter can be a value between 0x0 and 0x1F
  * @retval None
  */
void DCMI_OV9655_SetPrescaler(uint8_t OV9655_Prescaler)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_CLKRC, OV9655_Prescaler);
}

/**
  * @brief  Select the Output Format.
  * @param  OV9655_OuputFormat: the Format of the ouput Data.  
  *         This parameter can be one of the following values:
  *           @arg OUTPUT_FORMAT_RAWRGB_DATA 
  *           @arg OUTPUT_FORMAT_RAWRGB_INTERP    
  *           @arg OUTPUT_FORMAT_YUV              
  *           @arg OUTPUT_FORMAT_RGB    
  * @retval None
  */
void DCMI_OV9655_SelectOutputFormat(uint8_t OV9655_OuputFormat)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, OV9655_OuputFormat);
}


/**
  * @brief  Select the Output Format Resolution.
  * @param  OV9655_FormatResolution: the Resolution of the ouput Data. 
  *         This parameter can be one of the following values:
  *           @arg FORMAT_CTRL_15fpsVGA 
  *           @arg FORMAT_CTRL_30fpsVGA_NoVArioPixel    
  *           @arg FORMAT_CTRL_30fpsVGA_VArioPixel     
  * @retval None
  */
void DCMI_OV9655_SelectFormatResolution(uint8_t OV9655_FormatResolution)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, OV9655_FormatResolution);
}

/**
  * @brief  Set the new value of OV9655 registers
  * @param  OV9655_Register: the OV9655 Register to be configured. 
  * @param  Register_Val: The new value to be set 
  * @retval None
  */
void DCMI_OV9655_SetRegister(uint8_t OV9655_Register, uint8_t Register_Val)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_Register, Register_Val);
}

/**
  * @brief  Select the HREF Control signal option
  * @param  OV9665_HREFControl: the HREF Control signal option.
  *         This parameter can be one of the following value:
  *           @arg OV9665_HREFControl_Opt1: HREF edge offset to data output. 
  *           @arg OV9665_HREFControl_Opt2: HREF end 3 LSB    
  *           @arg OV9665_HREFControl_Opt3: HREF start 3 LSB      
  * @retval None
  */
void DCMI_OV9655_HREFControl(uint8_t OV9665_HREFControl)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_HREF, OV9665_HREFControl);
}

/**
  * @brief  Select the RGB format option
  * @param  OV9665_RGBOption: the RGB Format option.
  *         This parameter can be one of the following value:
  *           @arg RGB_NORMAL
  *           @arg RGB_565  
  *           @arg RGB_555    
  * @retval None
  */
void DCMI_OV9655_SelectRGBOption(uint8_t OV9665_RGBOption)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM15, OV9665_RGBOption);
}

 			 			  
/**
  * @}
  */
static void Delay(uint32_t nTime)
{
  while(nTime--);
} 

static void Delay_ms(uint32_t nTime)
{
  while(nTime--)
  	{Delay(1000);}

}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
