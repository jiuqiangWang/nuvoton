#ifndef __SYSTICK__H__
#define __SYSTICK__H__

#include "NUC029XAN.h"
extern void SysTick_Init(uint8_t SYSCLK);
extern void delay_us(uint32_t nus);
extern void delay_ms(uint32_t nms);


#endif



 

