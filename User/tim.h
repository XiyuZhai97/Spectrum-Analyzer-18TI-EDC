#ifndef _TIM_H
#define _TIM_H
#include "sys.h"

void TIM5_CH1_Cap_Init(u32 arr,u16 psc);
void TIM3_IRQHandler(void);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM5_IRQHandler(void);
#endif
