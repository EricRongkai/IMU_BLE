/***************************************************
*Task.c
*通用任务
****************************************************/

#include "LED.h"
#include "Task.h"
#include "BLE103.h"
#include "delay.h"
#include "mpu9250.h"
#include "includes.h"
OS_TCB BLED_Blink3_TaskTCB;
CPU_STK BLED_Blink3_TASK_STK[BLED_Blink3_STK_SIZE];

OS_TCB GLED_Blink3_TaskTCB;
CPU_STK GLED_Blink3_TASK_STK[GLED_Blink3_STK_SIZE];

OS_TCB RLED_Blink3_TaskTCB;
CPU_STK RLED_Blink3_TASK_STK[RLED_Blink3_STK_SIZE];

OS_TCB Get_MPU_Data_TaskTCB;
CPU_STK Get_MPU_Data_TASK_STK[Get_MPU_Data_STK_SIZE];

OS_TCB CheckBLE1_LINKsta_TaskTCB;
CPU_STK CheckBLE1_LINKsta_TASK_STK[CheckBLE1_LINKsta_STK_SIZE];

OS_TCB CheckBLE2_LINKsta_TaskTCB;
CPU_STK CheckBLE2_LINKsta_TASK_STK[CheckBLE2_LINKsta_STK_SIZE];

void Creat_EventFlag(void)
{
	OS_ERR err;
	OSFlagCreate((OS_FLAG_GRP*) &MeasureEvent,
							 (CPU_CHAR*) "MeasureEvent",
							 (OS_FLAGS) 0x00,
							 (OS_ERR*) err);
}

void Create_LED_Task(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	//创建BLED闪烁任务
	OSTaskCreate((OS_TCB 	* )&BLED_Blink3_TaskTCB,		
				       (CPU_CHAR	* )"BLED Blink task", 		
               (OS_TASK_PTR ) BLED_Blink3_task, 			
               (void		* )0,					
               (OS_PRIO	  )BLED_Blink3_TASK_PRIO,     
               (CPU_STK   * )&BLED_Blink3_TASK_STK[0],	
               (CPU_STK_SIZE)BLED_Blink3_STK_SIZE/10,	
               (CPU_STK_SIZE)BLED_Blink3_STK_SIZE,		
               (OS_MSG_QTY  )0,					
               (OS_TICK	  )0,					
               (void   	* )0,					
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
               (OS_ERR 	* )&err);				
				 
	//创建GLED闪烁任务
	OSTaskCreate((OS_TCB 	* )&GLED_Blink3_TaskTCB,		
							 (CPU_CHAR	* )"GLED Blink task", 		
               (OS_TASK_PTR )GLED_Blink3_task, 			
               (void		* )0,					
               (OS_PRIO	  )GLED_Blink3_TASK_PRIO,     	
               (CPU_STK   * )&GLED_Blink3_TASK_STK[0],	
               (CPU_STK_SIZE)GLED_Blink3_STK_SIZE/10,	
               (CPU_STK_SIZE)GLED_Blink3_STK_SIZE,		
               (OS_MSG_QTY  )0,					
               (OS_TICK	  )0,					
               (void   	* )0,				
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
               (OS_ERR 	* )&err);
							 
	//创建RLED闪烁任务
	OSTaskCreate((OS_TCB 	* )&RLED_Blink3_TaskTCB,		
							 (CPU_CHAR	* )"RLED Blink task", 		
               (OS_TASK_PTR )RLED_Blink3_task, 			
               (void		* )0,					
               (OS_PRIO	  )RLED_Blink3_TASK_PRIO,     	
               (CPU_STK   * )&RLED_Blink3_TASK_STK[0],	
               (CPU_STK_SIZE)RLED_Blink3_STK_SIZE/10,	
               (CPU_STK_SIZE)RLED_Blink3_STK_SIZE,		
               (OS_MSG_QTY  )0,					
               (OS_TICK	  )0,					
               (void   	* )0,				
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
               (OS_ERR 	* )&err);
	OSTaskSuspend(&BLED_Blink3_TaskTCB,&err);
	OSTaskSuspend(&GLED_Blink3_TaskTCB,&err);
	OSTaskSuspend(&RLED_Blink3_TaskTCB,&err);
}

//BLED闪烁3次 周期3秒
void BLED_Blink3_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	while(1)
	{
		Bled_on;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Bled_off;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Bled_on;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Bled_off;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Bled_on;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Bled_off;
		OSTimeDlyHMSM(0,0,3,0,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}
//GLED闪烁3次 周期3秒
void GLED_Blink3_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	while(1)
	{
		Gled_on;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Gled_off;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Gled_on;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Gled_off;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Gled_on;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Gled_off;
		OSTimeDlyHMSM(0,0,3,0,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}
//RLED闪烁3次 周期3秒
void RLED_Blink3_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	while(1)
	{
		Rled_on;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Rled_off;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Rled_on;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Rled_off;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Rled_on;
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err);
		Rled_off;
		OSTimeDlyHMSM(0,0,3,0,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

void Create_Get_MPU_Data_Task(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OSTaskCreate((OS_TCB 	* )&Get_MPU_Data_TaskTCB,		
						 (CPU_CHAR	* )"Get_MPU_Data task", 		
						 (OS_TASK_PTR ) Get_MPU_Data_task, 			
						 (void		* )0,					
						 (OS_PRIO	  )  Get_MPU_Data_TASK_PRIO,     
						 (CPU_STK   * )&Get_MPU_Data_TASK_STK[0],	
						 (CPU_STK_SIZE)Get_MPU_Data_STK_SIZE/10,	
						 (CPU_STK_SIZE)Get_MPU_Data_STK_SIZE,		
						 (OS_MSG_QTY  )0,					
						 (OS_TICK	  )0,					
						 (void   	* )0,					
						 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
						 (OS_ERR 	* )&err);	
//	OSTaskSuspend(&Get_MPU_Data_TaskTCB,&err);
}

OS_FLAG_GRP MeasureEvent;        //测量数据事件
void Get_MPU_Data_task(void *p_arg)
{	
	OS_ERR err;
	unsigned short TS = 0;
	unsigned char count1 = 0,count2 = 0;
	while(1)
	{	
		OS_FLAGS flags_num = OSFlagPend(&MeasureEvent,MeasureFlag,0,OS_OPT_PEND_FLAG_SET_ALL + OS_OPT_PEND_BLOCKING,0,&err);
		//读取惯性传感器数据
		if (TS<65535){TS ++;}else TS =0;
    READ_MPU9250_ACCEL();	
		READ_MPU9250_GYRO();
		char aaa = BLE1.DataReceive[0];
		if(BLE1.DataReceive[0]==0xCC && BLE1.DataReceive[1] ==0xCC)
		{
			BLE2_Reload_Data(BLE1.DataReceive,20);
			count1++;
		}
		memset(BLE1.DataReceive, 0, BLE1.DataAmount);  //清空缓存
		BLE2_Send_RawData(TS,mpu_acce,mpu_gyro);
		count2++;
		if(count1 == 200){Bled_on;}else if(count1 == 210){Bled_off; count2=0;}
		if(count2 == 200){Gled_on;}else if(count2 == 210){Gled_off; count2=0;}
		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

void Create_BLE1_Check_Task(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OSTaskCreate((OS_TCB 	* )&CheckBLE1_LINKsta_TaskTCB,		
						 (CPU_CHAR	* )"CheckBLE1_LINKsta task", 		
						 (OS_TASK_PTR ) CheckBLE1_LINKsta_task, 			
						 (void		* )0,					
						 (OS_PRIO	  )  CheckBLE1_LINKsta_TASK_PRIO,     
						 (CPU_STK   * )&CheckBLE1_LINKsta_TASK_STK[0],	
						 (CPU_STK_SIZE)CheckBLE1_LINKsta_STK_SIZE/10,	
						 (CPU_STK_SIZE)CheckBLE1_LINKsta_STK_SIZE,		
						 (OS_MSG_QTY  )0,					
						 (OS_TICK	  )0,					
						 (void   	* )0,					
						 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
						 (OS_ERR 	* )&err);
}

//BLE1 连接状态检查
void CheckBLE1_LINKsta_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	char sta;
	while(1)
	{
		while(!BLE1_ENCM()){}
		sta = BLE1_LINK_STA();
		while(!BLE1_ENTM()){}
		if(sta==1)
		{
			OSTaskResume(&BLED_Blink3_TaskTCB,&err);
			OSTaskSuspend(&Get_MPU_Data_TaskTCB,&err);
			BLE1.LINK_Sta = 1;
		}
		else
		{
			OSTaskSuspend(&BLED_Blink3_TaskTCB,&err);
			OSTaskResume(&Get_MPU_Data_TaskTCB,&err);
			BLE1.LINK_Sta = 0;
		}
		OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

void Create_BLE2_Check_Task(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OSTaskCreate((OS_TCB 	* )&CheckBLE2_LINKsta_TaskTCB,		
						 (CPU_CHAR	* )"CheckBLE2_LINKsta task", 		
						 (OS_TASK_PTR ) CheckBLE2_LINKsta_task, 			
						 (void		* )0,					
						 (OS_PRIO	  )  CheckBLE2_LINKsta_TASK_PRIO,     
						 (CPU_STK   * )&CheckBLE2_LINKsta_TASK_STK[0],	
						 (CPU_STK_SIZE)CheckBLE2_LINKsta_STK_SIZE/10,	
						 (CPU_STK_SIZE)CheckBLE2_LINKsta_STK_SIZE,		
						 (OS_MSG_QTY  )0,					
						 (OS_TICK	  )0,					
						 (void   	* )0,					
						 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
						 (OS_ERR 	* )&err);
}

//BLE2 连接状态检查
void CheckBLE2_LINKsta_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	char sta;
	while(1)
	{
		while(!BLE2_ENCM()){}
		sta = BLE2_LINK_STA();
		while(!BLE2_ENTM()){}
		if(sta==1)
		{
			OSTaskResume(&GLED_Blink3_TaskTCB,&err);
			OSTaskResume(&Get_MPU_Data_TaskTCB,&err);
			BLE2.LINK_Sta = 1;
		}
		else
		{
			OSTaskSuspend(&GLED_Blink3_TaskTCB,&err);
			OSTaskSuspend(&Get_MPU_Data_TaskTCB,&err);
			BLE2.LINK_Sta = 0;
		}
		OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}






