#ifndef __GETANALYSE_H
#define __GETANALYSE_H

#include "includes.h"

void GetCmdEventGroupCreat (void);
void GetAnalyse(uint8_t *ptRecData);
extern EventGroupHandle_t xGetCmdEventGroup;	
extern REAL_DATA_CALIBRATION_t REAL_DATA_CALIBRATION;
extern CONFIG_GPRS_t CONFIG_GPRS; 


//����ָʾGET��־������Ҫ�������Ϣλ
#define GET_CMD_STUP  (1<<0)

#endif 