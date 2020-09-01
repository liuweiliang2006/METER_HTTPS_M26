/**
  ******************************************************************************
  * File Name          : ReplyPacket.h
  * Description        :
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ReplyPacket_H
#define __ReplyPacket_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"
	 
typedef struct
{
	char keyWord[5];     					//关键
	char meterNumer[20]; 					//仪表号
	char identifier[5];  					//流水号
	char datetime[13];    				//日期            
	char verification[4]; 				//校验值
}ReplyPacket_t; //设置的结构体

extern ReplyPacket_t ReplyPacket;
extern bool IsNeedReply;

void refreshReplyPacket(ReplyPacket_t *sPacket);
void encodeReplyPacket(char *sendMeagess,ReplyPacket_t *sPacket);
//void ResetToMeter(UsedResetPacket_t *sPacket);
void SendReplyPacket(void);


#ifdef __cplusplus
}
#endif
#endif /*__ ReplyPacket_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
