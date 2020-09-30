#ifndef __GETANALYSE_H
#define __GETANALYSE_H

#include "includes.h"

void GetCmdEventGroupCreat (void);
void GetAnalyse(uint8_t *ptRecData);
extern EventGroupHandle_t xGetCmdEventGroup;	
extern REAL_DATA_CALIBRATION_t REAL_DATA_CALIBRATION;
extern REAL_Flow_CALIBRATION_t REAL_Flow_CALIBRATION;
extern CONFIG_GPRS_t CONFIG_GPRS; 

extern bool IsSaveCONFIG_Meter;
extern bool IsSaveCONFIG_GPRS;
extern bool IsSaveCALIBRATION;
extern bool IsSaveFlowCALIBRATION;
//用于指示GET标志组中需要处理的信息位
#define GET_CMD_STUP  (1<<0)

#endif 