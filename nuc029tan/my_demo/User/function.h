#ifndef  __FUNCTION__H__
#define  __FUNCTION__H__

#include "NUC029XAN.h"
/**************************************************************/
/*
		�����ʹ�����������/�¶ȴ�����
---------------------------------------------------------
	��ʶ       OUT1					OUT2          �¶ȴ�����
						 ����					�¶�
---------------------------------------------------------
	1					1~5V					 -							NONE
---------------------------------------------------------
	2					4~20MA				 -							NONE
---------------------------------------------------------
	1T				1~5V					1~5V						YES
---------------------------------------------------------
*/
/**************************************************************/
/*SMC����/ģ�������ϵ*/

/*
---------------------------------------------------------
								|				|			  B				 |		     
								|	  A   ------------------				C
								|				|4/16/40 |100|250|
----------------------------------------------------------
��ѹ���/V					  1   		1.5   1.4 1.8         5   
----------------------------------------------------------
�������/MA					4        6    5.6 7.2         20  
-----------------------------------------------------------
ע��
			A������0L/minʱ�����
			B������Ϊ��С�����
			C�������������
------------------------------------------------------------	
	 �ͺ�									��С�����						 �������
------------------------------------------------------------
PF3W704/504									0.5											4
------------------------------------------------------------
PF3W720/520									2												16		
------------------------------------------------------------
PF3W740/540									5												40
------------------------------------------------------------
PF3W711/511									10											100		
------------------------------------------------------------
PF3W721/521									50											250	
------------------------------------------------------------

*/

/***************************************************************/
/*SMC�����¶�/ģ�����
PF3W7/5ϵ��
---------------------------------------------------------
							A					B         C	        D
---------------------------------------------------------
��ѹ���	/V		0.6       1         5         5.4
---------------------------------------------------------
�������/MA		2.4       4         20        21.6	
---------------------------------------------------------

A=-10C
B=0C
C=100C
D=110C
*/
/***************************************************************/
/*
	���̹�ʽ:
		(����������-����������)/(��������-��������)=(������ֵ-����������)/(����ֵ-��������)��
*/

/***************************************************************/


#define LED_MAXCOUNT    150ul						//LED��˸���ֵ-10ms��ʱ���ж�
#define LED_MINCOUNT    20ul						//LED��˸��Сֵ-10ms��ʱ���ж�
#define TEMP_UPPER      110ul						//�¶Ȳɼ����ֵ
#define TEMP_LOWER      -10L						//�¶Ȳɼ���Сֵ

/*�������=DAC*1.66*/
#define VOLTAGE_UPPER   305ul						//DAC3.05���-�������OUT1��ѹ/�������ֵ5V(20MA
#define VOLTAGE_LOWER   61ul						//DAC0.61���-�������OUT1��ѹ/������Сֵ1V(4MA)

#define VOLTAGE_TEMP_UPPER   329ul			//DAC3.29���-�������OUT2��ѹ/�������ֵ5.4V
#define VOLTAGE_TEMP_LOWER   37ul				//DAC0.37���-�������OUT2��ѹ/������Сֵ0.6V

/*�¶��������*/
#define TEMP_OUTOFFSET    		(float)((float)(VOLTAGE_TEMP_UPPER-VOLTAGE_TEMP_LOWER)/(float)(TEMP_UPPER-TEMP_LOWER))
#define TEMP_OUTRANGE(TEMP)		(float)(((TEMP-TEMP_LOWER)*TEMP_OUTOFFSET )+VOLTAGE_TEMP_LOWER)
	
#define 	__PF3W504__
//#define 	__PF3W520__
//#define 	__PF3W540__
//#define 	__PF3W511__
//#define 	__PF3W521__


/*PF3W504
			�ڵ�����ʱ0.4~1L/minʱ�������Ƶ����1HZ����;���ܸ���;
			
*/
#ifdef 		__PF3W504__
/*����*100*/
#define FLOW_UPPER      	400ul				  	//��������ֵ
#define FLOW_LOWER      	50ul						//�������Сֵ

/*LED��˸����*/

#define LED_OFFSET						(float)((float)(LED_MAXCOUNT-LED_MINCOUNT)/(float)(FLOW_UPPER-FLOW_LOWER))
#define LED_RANGE(FLOW)				(float)(((FLOW-FLOW_LOWER)*LED_OFFSET)+LED_MINCOUNT)

/*�����������*/
#define FLOW_OUTOFFSET    		(float)((float)(VOLTAGE_UPPER-VOLTAGE_LOWER)/(float)(FLOW_UPPER-0ul))
#define FLOW_OUTRANGE(FLOW)		(float)(((FLOW-0ul)*FLOW_OUTOFFSET)+VOLTAGE_LOWER)

#endif





/*PF3W520*/
#ifdef 		__PF3W520__
/*����*100*/
#define FLOW_UPPER      1600ul		//��������ֵ
#define FLOW_LOWER      200ul		//�������Сֵ

/*LED��˸����*/

#define LED_OFFSET						(float)((float)(LED_MAXCOUNT-LED_MINCOUNT)/(float)(FLOW_UPPER-FLOW_LOWER))
#define LED_RANGE(FLOW)				(float)(((FLOW-FLOW_LOWER)*LED_OFFSET)+LED_MINCOUNT)

/*�����������*/
#define FLOW_OUTOFFSET    		(float)((float)(VOLTAGE_UPPER-VOLTAGE_LOWER)/(float)(FLOW_UPPER-0ul))
#define FLOW_OUTRANGE(FLOW)		(float)(((FLOW-0ul)*FLOW_OUTOFFSET)+VOLTAGE_LOWER)
#endif


/*PF3W540*/

#ifdef 		__PF3W540__
/*����*100*/
#define FLOW_UPPER      4000ul		//��������ֵ
#define FLOW_LOWER      500ul		//�������Сֵ

/*LED��˸����*/

#define LED_OFFSET						(float)((float)(LED_MAXCOUNT-LED_MINCOUNT)/(float)(FLOW_UPPER-FLOW_LOWER))
#define LED_RANGE(FLOW)				(float)(((FLOW-FLOW_LOWER)*LED_OFFSET)+LED_MINCOUNT)

/*�����������*/
#define FLOW_OUTOFFSET    		(float)((float)(VOLTAGE_UPPER-VOLTAGE_LOWER)/(float)(FLOW_UPPER-0ul))
#define FLOW_OUTRANGE(FLOW)		(float)(((FLOW-0ul)*FLOW_OUTOFFSET)+VOLTAGE_LOWER)
#endif



/*PF3W511*/

#ifdef 		__PF3W511__
/*����*100*/
#define FLOW_UPPER      10000ul		//��������ֵ
#define FLOW_LOWER      1000ul			//�������Сֵ
/*LED��˸����*/

#define LED_OFFSET						(float)((float)(LED_MAXCOUNT-LED_MINCOUNT)/(float)(FLOW_UPPER-FLOW_LOWER))
#define LED_RANGE(FLOW)				(float)(((FLOW-FLOW_LOWER)*LED_OFFSET)+LED_MINCOUNT)

/*�����������*/
#define FLOW_OUTOFFSET    		(float)((float)(VOLTAGE_UPPER-VOLTAGE_LOWER)/(float)(FLOW_UPPER-0ul))
#define FLOW_OUTRANGE(FLOW)		(float)(((FLOW-0ul)*FLOW_OUTOFFSET)+VOLTAGE_LOWER)
#endif



/*PF3W521*/
#ifdef 		__PF3W521__
/*����*100*/
#define FLOW_UPPER      25000ul		//��������ֵ
#define FLOW_LOWER      500ul			//�������Сֵ
/*LED��˸����*/

#define LED_OFFSET						(float)((float)(LED_MAXCOUNT-LED_MINCOUNT)/(float)(FLOW_UPPER-FLOW_LOWER))
#define LED_RANGE(FLOW)				(float)(((FLOW-FLOW_LOWER)*LED_OFFSET)+LED_MINCOUNT)

/*�����������*/
#define FLOW_OUTOFFSET    		(float)((float)(VOLTAGE_UPPER-VOLTAGE_LOWER)/(float)(FLOW_UPPER-0ul))
#define FLOW_OUTRANGE(FLOW)		(float)(((FLOW-0ul)*FLOW_OUTOFFSET)+VOLTAGE_LOWER)
#endif


/**
  * @brief    	clear Counter value
  *
  * @param[in]  timer  The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  *
  * @return    
  *
  * @details    
  */
static __INLINE void TIMER_ClearCounter(TIMER_T *timer)
{
    timer->TCSR|=TIMER_TCSR_CRST_Msk;
}
extern int32_t  FlowData_Cacl(void);
extern int32_t  TempData_Cacl(void);

#endif





























