#ifndef __SIGNAL_H
#define __SIGNAL_H

#include "includes.h"

/*ATָ������־λ λ�ö��壬ָʾ��ͬ��ATָ���Ӧ���¼���־��λ��*/
#define AT_BIT 					(1<<0) 	//AT
#define CSQ_BIT 				(1<<1)	//AT+CSQ
#define QIREGAPP_BIT 		(1<<2)	//AT+QIREGAPP
#define QIACT_BIT 			(1<<3)	//AT+QIACT
#define QILOCIP_BIT 		(1<<4)	//AT+QILOCIP
#define SNI_BIT 				(1<<5)	//AT+SNI
#define QSSLCFG_BIT 		(1<<6)	//AT+QSSLCFG
#define QHTTPURL_BIT 		(1<<7)	//AT+QHTTPURL
#define SEVERADDR_BIT 	(1<<8)	//URL address
#define QHTTPGET_BIT 		(1<<9)	//AT+QHTTPGET
#define QHTTPREAD_BIT 	(1<<10)	//AT+QHTTPREAD
#define QIDEACT_BIT 		(1<<11)	//AT+QIDEACT
#define QHTTPPOST_BIT 	(1<<12)	//AT+QHTTPPOST
#define POSTDATA_BIT 		(1<<13)	//AT+POSTDATA
#define QHTTPCFG_BIT		(1<<14) //AT+QHTTPCFG

#define ALL_AT_BIT			(1<<23) //������һATָ����󣬸�λ��λ

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
extern EventGroupHandle_t xCreatedEventGroup;  //�¼���־�飬����ָʾATָ��Ĵ�������������Ӧλ
	
void AppObjCreate (void);
void M26_Sni_Init(void );
void  PostCookingSecsion(void);
void  PostMeterStatus(void);
void  PostMeterWarning(void);
void  PostMeterHardware(void);
void  PostMeterSettings(void);
void  GetMeterSettings(void);

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
uint8_t Analysis_QHTTPCFG_Cmd(char *pdata);

#endif