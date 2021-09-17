#ifndef  __FUNCTION__H__
#define  __FUNCTION__H__

#include "NUC029XAN.h"
/**************************************************************/
/*
		分离型传感器输出规格/温度传感器
---------------------------------------------------------
	标识       OUT1					OUT2          温度传感器
						 流量					温度
---------------------------------------------------------
	1					1~5V					 -							NONE
---------------------------------------------------------
	2					4~20MA				 -							NONE
---------------------------------------------------------
	1T				1~5V					1~5V						YES
---------------------------------------------------------
*/
/**************************************************************/
/*SMC流量/模拟输出关系*/

/*
---------------------------------------------------------
								|				|			  B				 |		     
								|	  A   ------------------				C
								|				|4/16/40 |100|250|
----------------------------------------------------------
电压输出/V					  1   		1.5   1.4 1.8         5   
----------------------------------------------------------
电流输出/MA					4        6    5.6 7.2         20  
-----------------------------------------------------------
注：
			A：代表0L/min时的输出
			B：代表为最小额定流量
			C：代表最大额定流量
------------------------------------------------------------	
	 型号									最小额定流量						 最大额定流量
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
/*SMC流体温度/模拟输出
PF3W7/5系列
---------------------------------------------------------
							A					B         C	        D
---------------------------------------------------------
电压输出	/V		0.6       1         5         5.4
---------------------------------------------------------
电流输出/MA		2.4       4         20        21.6	
---------------------------------------------------------

A=-10C
B=0C
C=100C
D=110C
*/
/***************************************************************/
/*
	量程公式:
		(数字量上限-数字量下限)/(量程上限-量程下限)=(数字量值-数字量下限)/(量程值-量程下限)，
*/

/***************************************************************/


#define LED_MAXCOUNT    150ul						//LED闪烁最大值-10ms定时器中断
#define LED_MINCOUNT    20ul						//LED闪烁最小值-10ms定时器中断
#define TEMP_UPPER      110ul						//温度采集最大值
#define TEMP_LOWER      -10L						//温度采集最小值

/*功放输出=DAC*1.66*/
#define VOLTAGE_UPPER   305ul						//DAC3.05输出-流量输出OUT1电压/电流最大值5V(20MA
#define VOLTAGE_LOWER   61ul						//DAC0.61输出-流量输出OUT1电压/电流最小值1V(4MA)

#define VOLTAGE_TEMP_UPPER   329ul			//DAC3.29输出-流量输出OUT2电压/电流最大值5.4V
#define VOLTAGE_TEMP_LOWER   37ul				//DAC0.37输出-流量输出OUT2电压/电流最小值0.6V

/*温度输出量程*/
#define TEMP_OUTOFFSET    		(float)((float)(VOLTAGE_TEMP_UPPER-VOLTAGE_TEMP_LOWER)/(float)(TEMP_UPPER-TEMP_LOWER))
#define TEMP_OUTRANGE(TEMP)		(float)(((TEMP-TEMP_LOWER)*TEMP_OUTOFFSET )+VOLTAGE_TEMP_LOWER)
	
#define 	__PF3W504__
//#define 	__PF3W520__
//#define 	__PF3W540__
//#define 	__PF3W511__
//#define 	__PF3W521__


/*PF3W504
			在低流量时0.4~1L/min时脉冲计数频率在1HZ左右;可能更低;
			
*/
#ifdef 		__PF3W504__
/*流量*100*/
#define FLOW_UPPER      	400ul				  	//额定流量最大值
#define FLOW_LOWER      	50ul						//额定流量最小值

/*LED闪烁量程*/

#define LED_OFFSET						(float)((float)(LED_MAXCOUNT-LED_MINCOUNT)/(float)(FLOW_UPPER-FLOW_LOWER))
#define LED_RANGE(FLOW)				(float)(((FLOW-FLOW_LOWER)*LED_OFFSET)+LED_MINCOUNT)

/*流量输出量程*/
#define FLOW_OUTOFFSET    		(float)((float)(VOLTAGE_UPPER-VOLTAGE_LOWER)/(float)(FLOW_UPPER-0ul))
#define FLOW_OUTRANGE(FLOW)		(float)(((FLOW-0ul)*FLOW_OUTOFFSET)+VOLTAGE_LOWER)

#endif





/*PF3W520*/
#ifdef 		__PF3W520__
/*流量*100*/
#define FLOW_UPPER      1600ul		//额定流量最大值
#define FLOW_LOWER      200ul		//额定流量最小值

/*LED闪烁量程*/

#define LED_OFFSET						(float)((float)(LED_MAXCOUNT-LED_MINCOUNT)/(float)(FLOW_UPPER-FLOW_LOWER))
#define LED_RANGE(FLOW)				(float)(((FLOW-FLOW_LOWER)*LED_OFFSET)+LED_MINCOUNT)

/*流量输出量程*/
#define FLOW_OUTOFFSET    		(float)((float)(VOLTAGE_UPPER-VOLTAGE_LOWER)/(float)(FLOW_UPPER-0ul))
#define FLOW_OUTRANGE(FLOW)		(float)(((FLOW-0ul)*FLOW_OUTOFFSET)+VOLTAGE_LOWER)
#endif


/*PF3W540*/

#ifdef 		__PF3W540__
/*流量*100*/
#define FLOW_UPPER      4000ul		//额定流量最大值
#define FLOW_LOWER      500ul		//额定流量最小值

/*LED闪烁量程*/

#define LED_OFFSET						(float)((float)(LED_MAXCOUNT-LED_MINCOUNT)/(float)(FLOW_UPPER-FLOW_LOWER))
#define LED_RANGE(FLOW)				(float)(((FLOW-FLOW_LOWER)*LED_OFFSET)+LED_MINCOUNT)

/*流量输出量程*/
#define FLOW_OUTOFFSET    		(float)((float)(VOLTAGE_UPPER-VOLTAGE_LOWER)/(float)(FLOW_UPPER-0ul))
#define FLOW_OUTRANGE(FLOW)		(float)(((FLOW-0ul)*FLOW_OUTOFFSET)+VOLTAGE_LOWER)
#endif



/*PF3W511*/

#ifdef 		__PF3W511__
/*流量*100*/
#define FLOW_UPPER      10000ul		//额定流量最大值
#define FLOW_LOWER      1000ul			//额定流量最小值
/*LED闪烁量程*/

#define LED_OFFSET						(float)((float)(LED_MAXCOUNT-LED_MINCOUNT)/(float)(FLOW_UPPER-FLOW_LOWER))
#define LED_RANGE(FLOW)				(float)(((FLOW-FLOW_LOWER)*LED_OFFSET)+LED_MINCOUNT)

/*流量输出量程*/
#define FLOW_OUTOFFSET    		(float)((float)(VOLTAGE_UPPER-VOLTAGE_LOWER)/(float)(FLOW_UPPER-0ul))
#define FLOW_OUTRANGE(FLOW)		(float)(((FLOW-0ul)*FLOW_OUTOFFSET)+VOLTAGE_LOWER)
#endif



/*PF3W521*/
#ifdef 		__PF3W521__
/*流量*100*/
#define FLOW_UPPER      25000ul		//额定流量最大值
#define FLOW_LOWER      500ul			//额定流量最小值
/*LED闪烁量程*/

#define LED_OFFSET						(float)((float)(LED_MAXCOUNT-LED_MINCOUNT)/(float)(FLOW_UPPER-FLOW_LOWER))
#define LED_RANGE(FLOW)				(float)(((FLOW-FLOW_LOWER)*LED_OFFSET)+LED_MINCOUNT)

/*流量输出量程*/
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





























