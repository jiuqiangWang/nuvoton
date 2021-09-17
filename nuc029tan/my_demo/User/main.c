#include "NUC029XAN.h"
#include "systick.h"
#include "Flash.h"
#include <stdio.h>
#include <string.h>
#include "Usart0.h"


#define PLL_CLOCK        50000000					//PLL时钟频率：50MHZ


 


/**
* @brief  : 系统时钟初始化
* @param[in] ：none						
* @retval :none
	@note:使能内部HIRC时钟：22MHZ
				使能外部晶振:16MHZ
				系统工作频率（PLL）：50MHZ
*/
static void System_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Enable Internal RC 22.1184MHz clock */
    CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

    /* Waiting for Internal RC clock ready */
    //CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

    /* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

    /* Enable external XTAL 16MHz clock */
	  /*
	     产品外部晶振为16M，在System_NUC029xAN.h头文件修改 
	  */
    CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);

    /* Waiting for external XTAL clock ready */
  //  CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);

    /* Set core clock as PLL_CLOCK from PLL 
			 PLL_CLOCK时钟为50MHZ
		*/
    CLK_SetCoreClock(PLL_CLOCK);
}

/**
* @brief  : 独立看门狗初始化
* @param[in] ：none						
* @retval :none
	@note:none
*/
void WDT_Init(void)
{
	  /* Enable WDT module clock */
    CLK_EnableModuleClock(WDT_MODULE);    
        
    /* Select WDT module clock source */
    CLK_SetModuleClock(WDT_MODULE, CLK_CLKSEL1_WDT_S_LIRC, NULL);
		
    /* Enable WDT time-out reset function and select time-out interval to 2^14 * WDT clock then start WDT counting */
		/*
			 在10KHZ时钟下，可选择的超时间间隔为2^4~2^18(104ms~26.316s)
		*/
    WDT_Open(WDT_TIMEOUT_2POW16, WDT_RESET_DELAY_1026CLK, TRUE, FALSE);
	
		WDT_RESET_COUNTER();

	  WDT_CLEAR_TIMEOUT_INT_FLAG();
}


/**
* @brief  : 系统驱动初始化
* @param[in] ：none						
* @retval :none
	@note: 
*/
static void Device_Init(void)
{
 
		SysTick_Init(16);						  //系统滴答定时器初始化,采用外部晶振16MHZ作为时钟源
		System_Init();								//系统时钟初始化
 
		USART1_Init(115200);				//USART1初始化,波特率115200
  
}

 
 struct EEPROM
 {
	 uint8_t test1;
	 uint8_t test2;
 }EEPROM_dat1;



int  main(void)
{
		uint8_t Data,a[]={"新塘单片机 \r\n"};
		uint8_t i=0;
		uint8_t *j=&a[0];
	
		System_Init();								//系统时钟初始化
		SYS_UnlockReg();							//系统寄存器解锁
		Device_Init();								//硬件驱动初始化
 
 

 
		printf("EPPROM Test\r\n");
		//printf("temp=%x\r\n",0xffffffff);
		
	  FMC_Open();
	  SYS_UnlockReg();
		FMC_EnableAPUpdate();
	  Init_EEPROM(100,6);
		Search_Valid_Page();
		
		
		printf("**************Test EEPROM ***************\r\n");
 
		Read_StructFullData(&EEPROM_dat1,sizeof(EEPROM_dat1));
		printf("读出 EEPROM_dat1.test1 : %d \r\n",EEPROM_dat1.test1);
		if(EEPROM_dat1.test1 != 0x15)
		{
			EEPROM_dat1.test1 = 0x15;
			printf("写入 EEPROM_dat1.test1 : %d \r\n",EEPROM_dat1.test1);
			Write_StructFullData(&EEPROM_dat1,sizeof(EEPROM_dat1));
	 
		}
		  
		printf("**************Test finish ***************\r\n");
		
		while(1);
}
 



















