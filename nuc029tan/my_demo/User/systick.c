#include "systick.h"



static uint8_t fac_us=0;								   
static uint16_t fac_ms=0;
/**
* @brief  :ϵͳ�δ�ʱ����ʼ��
					 24λ���µݼ���ʱ��
* @param[in] ��SYSCLK:�ⲿ����
* @retval 
*/
void SysTick_Init(uint8_t SYSCLK)
{
		/*ѡ��δ�ʱ����ʱ��Դ
			������-16M
		*/
		CLK_SetSysTickClockSrc(CLK_CLKSEL0_STCLK_S_HXT); 
		fac_us=SYSCLK;					
		fac_ms=(uint16_t)fac_us*1000;				   
}

 /**
* @brief  :ϵͳ�δ�ʱ��΢����ʱ
* @param[in] ��nus:΢����ʱ����
              ��󲻳���  (1/16000000)*0xffffff=1.04s
* @retval 
*/ 								   
void delay_us(uint32_t nus)
{		
		uint32_t temp;	    	 
		SysTick->LOAD=nus*fac_us; 				  		 
		SysTick->VAL=0x00;        				
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	
		do
		{
			temp=SysTick->CTRL;
		}while((temp&0x01)&&!(temp&(1<<16)));	
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
		SysTick->VAL =0X00;      		
}

 /**
* @brief  :ϵͳ�δ�ʱ��������ʱ
* @param[in] ��mus:������ʱ����
              ��󲻳���  (1/16000000)*0xffffff=1.04s
* @retval 
*/ 
void delay_ms(uint32_t nms)
{	 		  	  
		uint32_t temp;		   
		SysTick->LOAD=(uint32_t)nms*fac_ms;			
		SysTick->VAL =0x00;	
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;
		do
		{
				temp=SysTick->CTRL;

		}while((temp&0x01)&&!(temp&(1<<16)));		
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	
		SysTick->VAL =0X00; 	
} 


