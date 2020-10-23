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

//UCOSIII���������ȼ��û�������ʹ��
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()

//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		128
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//������
int main(void)
{
	RCC_ClocksTypeDef RCC_clock; 	
	OS_ERR err;
	CPU_SR_ALLOC();
	LED_Config(); 
	KEY_Init();																			//������ʼ��
	delay_init();  																	//ʱ�ӳ�ʼ��	//LED��ʼ��
	WKUP_Init();																		//���ػ����������ģʽ��
	/***********************************�ϵ������ʼ��***************************************/
	__set_PRIMASK(1);  															//�����ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�жϷ�������
	BLE103_Init();																	//BLEģ���ʼ��
	i2cInit();      							  								//IIC���ߵĳ�ʼ��
	if(Mpu9250_Work_Mode_Init()) while(1);					//MPU9250��ʼ��
	RCC_GetClocksFreq(&RCC_clock);  								//��ȡ��ǰ��ʱ��Ƶ��
	__set_PRIMASK(0);  						  								//�����ж�
	
	/***********************************����ģ���ʼ��***************************************/
	BLE1_RESET();
	BLE2_RESET();
	BLE1_WAKE(1);
	BLE2_WAKE(1);
	//����BLE1Ϊ��ģʽ
	while(!BLE1_ENCM()){}
	while(!BLE1_SET_MODE('M')){}
	while(!BLE1_ENCM()){}
	while(!BLE1_CONNADD()){}
	while(!BLE1_ENCM()){}
	while(!BLE1_AUTOCONN()){}
	//��ѯ״̬
	while(!BLE1_MODE()){}
	while(!BLE1_NAME()){}
	while(!BLE1_MAC()){}
	while(!BLE1_CIVER()){}
	while(!BLE1_MAXPUT(0)){}			
	while(!BLE1_ENTM()){}
	//����BLE2Ϊ��ģʽ
	while(!BLE2_ENCM()){}
	while(!BLE2_SET_MODE('S')){}
	//��ѯ״̬
	while(!BLE2_ENCM()){}
	while(!BLE2_MODE()){}
	while(!BLE2_NAME()){}
	while(!BLE2_MAC()){}
	while(!BLE2_CIVER()){}
	while(!BLE2_MAXPUT(0)){}			
	while(!BLE2_ENTM()){}
	Rled_off;	
	EnableLINK();                                   //��������״ָ̬ʾ��
	
	/***********************************����ϵͳ��ʼ��***************************************/
	OSInit(&err);		    														//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();														//�����ٽ���			 
	//������ʼ����
	OSTaskCreate(  (OS_TCB 	* )&StartTaskTCB,				//������ƿ�
								 (CPU_CHAR	* )"start task", 			//��������
                 (OS_TASK_PTR )start_task, 				//������
                 (void		* )0,										//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,									//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,										//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,										//�û�����Ĵ洢��
                 (OS_OPT    )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);								//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();															//�˳��ٽ���	 
	OSStart(&err);      														//����UCOSIII
  /***************************************************************************************/
}

//��ʼ����������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);  //ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN	  //���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN   //��ʹ��ʱ��Ƭ��ת��ʱ��
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
#endif
	
	OS_CRITICAL_ENTER();				//�����ٽ���

//	Create_LED_Task();
	Creat_EventFlag();
	Create_Get_MPU_Data_Task();
	OS_Start_Flag = 1;
	
	OS_CRITICAL_EXIT();					//�˳��ٽ���
	
	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
}



