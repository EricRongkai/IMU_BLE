#include "includes.h"
//BLED
#define BLED_Blink3_TASK_PRIO		7
#define BLED_Blink3_STK_SIZE 		128
extern OS_TCB BLED_Blink3_TaskTCB;
extern CPU_STK BLED_Blink3_TASK_STK[BLED_Blink3_STK_SIZE];
void BLED_Blink3_task(void *p_arg);
//GLED
#define GLED_Blink3_TASK_PRIO		7
#define GLED_Blink3_STK_SIZE 		128
extern OS_TCB GLED_Blink3_TaskTCB;
extern CPU_STK GLED_Blink3_TASK_STK[GLED_Blink3_STK_SIZE];
void GLED_Blink3_task(void *p_arg);
//RLED
#define RLED_Blink3_TASK_PRIO		7	
#define RLED_Blink3_STK_SIZE 		128
extern OS_TCB RLED_Blink3_TaskTCB;
extern CPU_STK RLED_Blink3_TASK_STK[RLED_Blink3_STK_SIZE];
void RLED_Blink3_task(void *p_arg);
//获取传感器数据任务
#define Get_MPU_Data_TASK_PRIO		6	
#define Get_MPU_Data_STK_SIZE 		256
extern OS_TCB Get_MPU_Data_TaskTCB;
extern CPU_STK Get_MPU_Data_TASK_STK[Get_MPU_Data_STK_SIZE];
void Get_MPU_Data_task(void *p_arg);
//检查BLE1连接状态
#define CheckBLE1_LINKsta_TASK_PRIO		5	
#define CheckBLE1_LINKsta_STK_SIZE 		128
extern OS_TCB CheckBLE1_LINKsta_TaskTCB;
extern CPU_STK CheckBLE1_LINKsta_TASK_STK[CheckBLE1_LINKsta_STK_SIZE];
void CheckBLE1_LINKsta_task(void *p_arg);
//检查BLE2连接状态
#define CheckBLE2_LINKsta_TASK_PRIO		5	
#define CheckBLE2_LINKsta_STK_SIZE 		128
extern OS_TCB CheckBLE2_LINKsta_TaskTCB;
extern CPU_STK CheckBLE2_LINKsta_TASK_STK[CheckBLE2_LINKsta_STK_SIZE];
void CheckBLE2_LINKsta_task(void *p_arg);

void Create_LED_Task(void);
void Create_Get_MPU_Data_Task(void);
void Create_BLE1_Check_Task(void);
void Create_BLE2_Check_Task(void);

/********************************事件标志**********************************/
void Creat_EventFlag(void);

extern OS_FLAG_GRP MeasureEvent;        //测量数据事件
#define MeasureFlag   0x01





