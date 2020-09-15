#ifndef __GETANALYSE_H
#define __GETANALYSE_H

#include "includes.h"

void GetAnalyse(uint8_t *ptRecData);
extern EventGroupHandle_t xGetCmdEventGroup;	
void GetCmdEventGroupCreat (void);


//用于指示GET标志组中需要处理的信息位
#define GET_CMD_STUP  (1<<0)

#endif 