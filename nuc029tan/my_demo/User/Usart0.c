#include "Usart0.h"


/**
* @brief  :USART0��ʼ��
* @param[in] ��Baudrate:������
* @retval :None
*/
void USART1_Init(uint32_t Baudrate)
{
    /* Enable UART module clock */
    CLK_EnableModuleClock(UART1_MODULE);//UART0_MODULE

    /* Select UART module clock source */
    CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_HXT, CLK_CLKDIV_UART(1));
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set GPB multi-function pins for UART0 RXD and TXD, and Clock Output */
		 /* Set P3 multi-function pins for UART0 RXD, TXD and ACMP0 output */
    SYS->P1_MFP &= ~(SYS_MFP_P12_Msk | SYS_MFP_P13_Msk);
    SYS->P1_MFP |= (SYS_MFP_P12_RXD1 | SYS_MFP_P13_TXD1);
	
		SYS_ResetModule(UART1_RST);		//��λUSART0����
		/* Init UART0 to 115200-8n1 for print message */
		UART_Open(UART1, Baudrate);			//����USART0�Ĳ�����Ϊ115200
}




 
 
 