#ifndef __SIGNAL_H
#define __SIGNAL_H
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"

typedef uint8_t (*Cmd_Analysis)(char *pCmd);
//add by lwl
typedef struct SeverInfo
{
	char *Sendsever; //http://virtserver.swaggerhub.com/KopaTechnology/KTMeterControl/
	char *MeterId;	//meter/{meterId}/
	char *SeverVer;//1.0.0/
	char *CardID;//card/{cardId}/
//	char Type[]
} Stru_Sever_Info_t;
//add by lwl
typedef struct ATInfo{
	uint8_t  u8CmdNum;
	char     *SendCommand;
//	char     ReceiveAnswer[128];
	Cmd_Analysis pFun;
}stru_P4_command_t;

extern stru_P4_command_t Send_AT_cmd[];
extern QueueHandle_t SendATQueue;
extern SemaphoreHandle_t  Semaphore_Uart_Rec;

void AppObjCreate (void);

void  PostCookingSecsion(void);
void  PostMeterStatus(void);

uint8_t Analysis_AT_Cmd(char *pdata);
uint8_t Analysis_CSQ_Cmd(char *pdata);
uint8_t Analysis_QIREGAPP_Cmd(char *pdata);
uint8_t Analysis_QIACT_Cmd(char *pdata);
uint8_t Analysis_QILOCIP_Cmd(char *pdata);
uint8_t Analysis_SNI_Cmd(char *pdata);
uint8_t Analysis_QSSLCFG_Cmd(char *pdata);
uint8_t Analysis_QHTTPURL_Cmd(char *pdata);
uint8_t Analysis_QHTTPGET_Cmd(char *pdata);
uint8_t Analysis_QHTTPREAD_Cmd(char *pdata);
uint8_t Analysis_QIDEACT_Cmd(char *pdata);
uint8_t Analysis_SEVER_Addr_Cmd(char *pdata);
uint8_t Analysis_QHTTPPOST_Cmd(char *pdata);
uint8_t Analysis_POSTDATA_Cmd(char *pdata);
uint8_t Analysis_POSTDATA_Cmd(char *pdata);

#endif