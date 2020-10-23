#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "LED.h"
#include "includes.h"
#include "BLE103.h"
#include "mpu9250.h"
#include "Task.h"
#include "stm32_iic.h"
#include "KEY.h"
#include "Power.h"

//UCOSIII中以下优先级用户程序不能使用
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()

//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		128
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//主函数
int main(void)
{
	RCC_ClocksTypeDef RCC_clock; 	
	OS_ERR err;
	CPU_SR_ALLOC();
	LED_Config(); 
	KEY_Init();																			//按键初始化
	delay_init();  																	//时钟初始化	//LED初始化
	WKUP_Init();																		//开关机（进入待机模式）
	/***********************************上电外设初始化***************************************/
	__set_PRIMASK(1);  															//关总中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组配置
	BLE103_Init();																	//BLE模块初始化
	i2cInit();      							  								//IIC总线的初始化
	if(Mpu9250_Work_Mode_Init()) while(1);					//MPU9250初始化
	RCC_GetClocksFreq(&RCC_clock);  								//获取当前各时钟频率
	__set_PRIMASK(0);  						  								//开总中断
	
	/***********************************蓝牙模块初始化***************************************/
	BLE1_RESET();
	BLE2_RESET();
	BLE1_WAKE(1);
	BLE2_WAKE(1);
	//设置BLE1为主模式
	while(!BLE1_ENCM()){}
	while(!BLE1_SET_MODE('M')){}
	while(!BLE1_ENCM()){}
	while(!BLE1_CONNADD()){}
	while(!BLE1_ENCM()){}
	while(!BLE1_AUTOCONN()){}
	//查询状态
	while(!BLE1_MODE()){}
	while(!BLE1_NAME()){}
	while(!BLE1_MAC()){}
	while(!BLE1_CIVER()){}
	while(!BLE1_MAXPUT(0)){}			
	while(!BLE1_ENTM()){}
	//设置BLE2为从模式
	while(!BLE2_ENCM()){}
	while(!BLE2_SET_MODE('S')){}
	//查询状态
	while(!BLE2_ENCM()){}
	while(!BLE2_MODE()){}
	while(!BLE2_NAME()){}
	while(!BLE2_MAC()){}
	while(!BLE2_CIVER()){}
	while(!BLE2_MAXPUT(0)){}			
	while(!BLE2_ENTM()){}
	Rled_off;	
	EnableLINK();                                   //开启连接状态指示灯
	
	/***********************************操作系统初始化***************************************/
	OSInit(&err);		    														//初始化UCOSIII
	OS_CRITICAL_ENTER();														//进入临界区			 
	//创建开始任务
	OSTaskCreate(  (OS_TCB 	* )&StartTaskTCB,				//任务控制块
								 (CPU_CHAR	* )"start task", 			//任务名字
                 (OS_TASK_PTR )start_task, 				//任务函数
                 (void		* )0,										//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,									//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,										//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,										//用户补充的存储区
                 (OS_OPT    )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);								//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();															//退出临界区	 
	OSStart(&err);      														//开启UCOSIII
  /***************************************************************************************/
}

//开始任务任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);  //统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN	  //如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN   //当使用时间片轮转的时候
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
#endif
	
	OS_CRITICAL_ENTER();				//进入临界区

//	Create_LED_Task();
	Creat_EventFlag();
	Create_Get_MPU_Data_Task();
	OS_Start_Flag = 1;
	
	OS_CRITICAL_EXIT();					//退出临界区
	
	OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身
}



