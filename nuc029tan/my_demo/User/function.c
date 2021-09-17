#include "function.h" 
#include "Interior_ADC.h"
#include "pwmcapture.h"
#include "systick.h"
#include <stdio.h>


/**
* @brief  : ADC��λֵƽ���˲���
* @param[in] ��Array:uint16_t����ָ��
							MemberNumber�������Ա����
* @retval :-1��ʧ��,�ɹ���ADC�˲����ֵ
	@note:
*/
static int32_t Data_Smoothing(int32_t *Array,int8_t MemberNumber)
{
    int32_t MaxData=Array[0];
	  int32_t MinData=Array[0];
	  int32_t revalue=0;
	  uint8_t i;
	  /*��Ա�������٣�*/
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
* @brief  :  �ɼ�5��������������ת����Ӧ����
* @param[in] ��None					
* @retval :-1��ʧ��,�ɹ�������ֵL/min
	@note:
*/
int32_t FlowData_Cacl(void)
{
			int32_t flow[5]={0,0,0,0,0};
	    int32_t flowData=0;
			uint8_t error_flag=0;
			uint8_t i=0;
			uint8_t j=0;
			/*�ɼ�5�β�������*/
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
								/*����4�βɼ��Ķ�Ϊ����ģ�����Ϊ��������������������*/
								if((++j)>4)
									 return -1;
						}
						delay_ms(5);
			
			}
			flowData=Data_Smoothing(flow,(sizeof(flow)/4));
			//printf("flowData:%d\r\n",flowData);
			/*������������ʽ����*/
			return flowData;	
}

/**
* @brief  :  �ɼ�5���¶ȴ�������ת����Ӧ�¶�
* @param[in] ��None					
* @retval :-1��ʧ��,�ɹ����¶�ֵC
	@note:
*/
int32_t  TempData_Cacl(void)
{

			int32_t temp[10]={0,0,0,0,0,0,0,0,0,0};
	    int32_t tempData=0;
			uint8_t error_flag=0;
			uint8_t i=0;
			uint8_t j=0;
			/*�ɼ�10���¶�����*/
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
								/*����4�βɼ��Ķ�Ϊ����ģ�����Ϊ�¶ȴ���������������*/
								if((++j)>4)
									 return -1;
						}
						delay_ms(5);
			}
			tempData=Data_Smoothing(temp,sizeof(temp)/4);
		//	printf("tempData:%d\r\n",tempData);
		//	printf("ADC_Voltage=%f\r\n",(3.3*tempData/4095));
			/*�������¶ȹ�ʽ����*/
			return tempData;	
}
	











