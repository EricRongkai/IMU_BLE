#include "stm32f10x.h"
#include "includes.h"
void BLE103_Init(void);
void BLE1_WAKE(u8 sta);
void BLE2_WAKE(u8 sta);
void BLE1_RESET(void);
void BLE2_RESET(void);
u8 Get_BLE1_LINK(void);
u8 Get_BLE2_LINK(void);
void BLE1_Send_AT_Command(char *Command);
void BLE2_Send_AT_Command(char *Command);
void BLE2_Reload_Data(unsigned char *Data,char LEN);
void BLE1_Send_RawData(unsigned short TS, short *Adt, short *Gdt);
void BLE2_Send_RawData(unsigned short TS, short *Adt, short *Gdt);
void EnableLINK(void);
u8 BLE1_ENCM(void);
u8 BLE2_ENCM(void);
u8 BLE1_ENTM(void);
u8 BLE2_ENTM(void);
u8 BLE1_NAME(void);
u8 BLE2_NAME(void);
u8 BLE1_MODE(void);
u8 BLE2_MODE(void);
u8 BLE1_MAC(void);
u8 BLE2_MAC(void);
u8 BLE1_CIVER(void);
u8 BLE2_CIVER(void);
u8 BLE1_MAXPUT(char EN);
u8 BLE2_MAXPUT(char EN);
u8 BLE1_SET_MODE(char Mode);
u8 BLE2_SET_MODE(char Mode);
u8 BLE1_SCAN(void);
u8 BLE2_SCAN(void);
u8 BLE1_CONNADD(void);
u8 BLE2_CONNADD(void);
u8 BLE1_Z(void);
u8 BLE2_Z(void);
u8 BLE1_LINK_STA(void);
u8 BLE2_LINK_STA(void);
u8 BLE1_AUTOCONN(void);
u8 BLE2_AUTOCONN(void);
u8 BLE1_HIBERNATE(void);
u8 BLE2_HIBERNATE(void);

extern struct BLE103 BLE1;
extern struct BLE103 BLE2;
extern OS_SEM MY_SEM; 

struct BLE103
{
	char ModelNAME[32];
	char ModelMODE[32];
	char ModelMAC[32];
	char ModelCIVER[32];
	char ModelTPL[32];
	char ModelPASS[32];
	char ModelPASSEN[32];
	char ModelUART[32];
	char ModelUARTTM[32];
	char ModelHELLO[32];
	char ModelSCAN[32];
	char ModelMAXPUT[32];
	char ModelCONNADD[32];
	char CM_Sta;
	char LP_Sta;
	char LINK;
  char LINK_Sta;
	char NewATReturn;
	char TempData;
	short DataAmount;
	unsigned char *DataReceive;
	char *DataTransmit;
};