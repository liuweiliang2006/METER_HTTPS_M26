/**
  ******************************************************************************
  * File Name          : OpenLockPacket.h
  * Description        :
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OpenLockPacket_H
#define __OpenLockPacket_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"
	 
typedef struct
{
	char keyWord[5];     					//关键字CONT
	char meterNumer[20]; 					//仪表号
	char identifier[5];  					//流水号
	char CARD_ID[19];	            //开锁卡号
	char datetime[13];    				//日期            
	char verification[4]; 				//校验值
}OpenLockPacket_t; //充值的结构体

extern OpenLockPacket_t openLockPacket;
extern bool IsNeedReportOpenLock;

void refreshOpenLockPacket(OpenLockPacket_t *cPacket);
void encodeOpenLockPacket(char *sendMeagess,OpenLockPacket_t *cPacket);
void SendOpenLockPacket(void);

#ifdef __cplusplus
}
#endif
#endif /*__ RechargePacket_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
