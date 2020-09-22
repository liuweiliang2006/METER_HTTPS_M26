#include "includes.h"
//#include "cJSON.h"

#define REC_COM_LEN 40  //GET数据的命令长度

EventGroupHandle_t xGetCmdEventGroup = NULL;

void GetCmdEventGroupCreat (void)
{
		/* 创建事件标志组 */
	xGetCmdEventGroup = xEventGroupCreate();
	
	if(xGetCmdEventGroup == NULL)
	{
			/* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
	}
}

//分析GET的数据内容
void GetAnalyse(uint8_t *ptRecData)
{
	char *ptFindResult ;
	char *ptJson;
	char *ptStrStart;
	cJSON *json;
	cJSON *item;
	cJSON* test_arr ;
	cJSON *object;
	char Value[REC_COM_LEN];
	float JsonValue;
	memset(Value,0,REC_COM_LEN);
//	int test_arr; 
	ptStrStart = (char *)ptRecData;
	ptFindResult = strstr(ptStrStart,"200");
	if(ptFindResult != NULL)
	{
		ptJson = strstr(ptStrStart,"{");
		
		ptFindResult = NULL;
		ptFindResult = strstr(ptStrStart,"STUP");
		if(ptFindResult != NULL)   //GetMeterSetting 命令解析
		{
			xEventGroupSetBits(xGetCmdEventGroup, GET_CMD_STUP);
			json=cJSON_Parse(ptJson); //获取整个Json大的句柄
			if(json != NULL)
			{
				test_arr = cJSON_GetObjectItem(json,"message");
				
				item = cJSON_GetObjectItem(test_arr,"serverIpAddress"); 					//IP地址
				memcpy(Value,item->valuestring,strlen(item->valuestring));
				strncpy(CONFIG_GPRS.Server_IP,Value,strlen(Value));
				memset(Value,0,REC_COM_LEN);
				
				item = cJSON_GetObjectItem(test_arr,"serverPort"); 					//端口号
				JsonValue = item->valueint;
				sprintf(CONFIG_GPRS.Socket_Port,"%d",(uint32_t)JsonValue);
				
				
				item = cJSON_GetObjectItem(test_arr,"dataUploadPeriod"); 					//上传周期
				JsonValue = item->valueint;
				CONFIG_Meter.UpDuty = (uint32_t) JsonValue;
				
				item = cJSON_GetObjectItem(test_arr,"warningLowBatteryVoltage");	//报警电压阈值
				memcpy(Value,item->valuestring,strlen(item->valuestring));
				sscanf(Value,"%f",&CONFIG_Meter.LowBattery);
				memset(Value,0,REC_COM_LEN);
				
				item = cJSON_GetObjectItem(test_arr,"warningLowCreditBalance"); 	//卡内余额报敬阈值
				JsonValue = item->valuedouble;
				CONFIG_Meter.LowCredit = JsonValue;
				
				item = cJSON_GetObjectItem(test_arr,"warningLowGasVolumeAlarm"); 	//罐内气体余量阈值
				memcpy(Value,item->valuestring,strlen(item->valuestring));
				sscanf(Value,"%f",&CONFIG_Meter.LowGasVolume);
				memset(Value,0,REC_COM_LEN);
				
				item = cJSON_GetObjectItem(test_arr,"meterCurrency"); 						//币种 单位
				memcpy(Value,item->valuestring,strlen(item->valuestring));
				strncpy(CONFIG_Meter.CURRENCY,Value,3);
				memset(Value,0,REC_COM_LEN);
				
				CONFIG_Meter_Write(); //保存表信息 
				
				
				item = cJSON_GetObjectItem(test_arr,"sensorSlope");	//斜率
				memcpy(Value,item->valuestring,strlen(item->valuestring));
				sscanf(Value,"%f",&REAL_DATA_CALIBRATION.slope);
				memset(Value,0,REC_COM_LEN);
				
				item = cJSON_GetObjectItem(test_arr,"sensorIntercept"); 	//原点
				JsonValue = item->valueint;
				REAL_DATA_CALIBRATION.zero = JsonValue;
				
//				CALIBRATION_Voltage_Write();  //保存斜率与原点
				
				
				cJSON_Delete(json);
			}
		}		
		
	}
	
}