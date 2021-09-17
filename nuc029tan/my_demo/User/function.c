#include "function.h" 
#include "Interior_ADC.h"
#include "pwmcapture.h"
#include "systick.h"
#include <stdio.h>


/**
* @brief  : ADC中位值平均滤波法
* @param[in] ：Array:uint16_t类型指针
							MemberNumber：数组成员个数
* @retval :-1：失败,成功：ADC滤波后的值
	@note:
*/
static int32_t Data_Smoothing(int32_t *Array,int8_t MemberNumber)
{
    int32_t MaxData=Array[0];
	  int32_t MinData=Array[0];
	  int32_t revalue=0;
	  uint8_t i;
	  /*成员个数过少，*/
	  if(MemberNumber<3)
				return -1;
		for(i=0;i<MemberNumber;i++)
	  {
				if(MinData>Array[i])	
						MinData=Array[i];	
			 
			  if(MaxData<Array[i])	
						MaxData=Array[i];
				revalue+=Array[i];
		}
		revalue=(revalue-MaxData-MinData)/(MemberNumber-2);
		return revalue;
}




/**
* @brief  :  采集5次流量传感器并转换对应流量
* @param[in] ：None					
* @retval :-1：失败,成功：流量值L/min
	@note:
*/
int32_t FlowData_Cacl(void)
{
			int32_t flow[5]={0,0,0,0,0};
	    int32_t flowData=0;
			uint8_t error_flag=0;
			uint8_t i=0;
			uint8_t j=0;
			/*采集5次捕获数据*/
			for(i=0;i<(sizeof(flow)/5);i++)
		  {		
						if(1==error_flag)
						{
								--i;
								error_flag=0;
						}
						flow[i]= CalPeriodTime(PWMA,PWM_CH2);
				    if(-1==flow[i])
						{
							  error_flag=1;
								/*连续4次采集的都为错误的，可视为流量传感器可能已破损*/
								if((++j)>4)
									 return -1;
						}
						delay_ms(5);
			
			}
			flowData=Data_Smoothing(flow,(sizeof(flow)/4));
			//printf("flowData:%d\r\n",flowData);
			/*这里有流量公式换算*/
			return flowData;	
}

/**
* @brief  :  采集5次温度传感器并转换对应温度
* @param[in] ：None					
* @retval :-1：失败,成功：温度值C
	@note:
*/
int32_t  TempData_Cacl(void)
{

			int32_t temp[10]={0,0,0,0,0,0,0,0,0,0};
	    int32_t tempData=0;
			uint8_t error_flag=0;
			uint8_t i=0;
			uint8_t j=0;
			/*采集10次温度数据*/
			for(i=0;i<(sizeof(temp)/4);i++)
		  {
						
				    if(1==error_flag)
						{
								--i;
								error_flag=0;
						}
						temp[i]=Read_ADCValue(4);
				    if(-1==temp[i])
						{
								error_flag=1;
								/*连续4次采集的都为错误的，可视为温度传感器可能已破损*/
								if((++j)>4)
									 return -1;
						}
						delay_ms(5);
			}
			tempData=Data_Smoothing(temp,sizeof(temp)/4);
		//	printf("tempData:%d\r\n",tempData);
		//	printf("ADC_Voltage=%f\r\n",(3.3*tempData/4095));
			/*这里有温度公式换算*/
			return tempData;	
}
	











