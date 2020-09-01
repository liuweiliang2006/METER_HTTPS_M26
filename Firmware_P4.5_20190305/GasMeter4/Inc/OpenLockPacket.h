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
	char keyWord[5];     					//�ؼ���CONT
	char meterNumer[20]; 					//�Ǳ��
	char identifier[5];  					//��ˮ��
	char CARD_ID[19];	            //��������
	char datetime[13];    				//����            
	char verification[4]; 				//У��ֵ
}OpenLockPacket_t; //��ֵ�Ľṹ��

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
