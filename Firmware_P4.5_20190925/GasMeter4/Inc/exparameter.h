#ifndef __EXPARAMETER_H__
#define __EXPARAMETER_H__


#include "gas.h"
#include "CookingSessionReport.h"
#include "W25Q64.h"
#include "ReportStatePacket.h"

extern REAL_DATA_Credit_t REAL_DATA_Credit;
extern CookingSessionReport_t CookingSessionReport;
extern reportStatePacket_t reportStatePacket;
//extern bool IsNeedRestart;
extern CONFIG_Meter_t CONFIG_Meter;

//enum { ERROR=0,CORRECT!=false};

#endif