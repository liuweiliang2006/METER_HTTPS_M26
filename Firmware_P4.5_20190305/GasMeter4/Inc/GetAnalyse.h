#ifndef __GETANALYSE_H
#define __GETANALYSE_H

#include "includes.h"

void GetAnalyse(uint8_t *ptRecData);
extern EventGroupHandle_t xGetCmdEventGroup;	
void GetCmdEventGroupCreat (void);


//����ָʾGET��־������Ҫ�������Ϣλ
#define GET_CMD_STUP  (1<<0)

#endif 