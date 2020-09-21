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
				
				item = cJSON_GetObjectItem(test_arr,"dataUploadPeriod"); 					//上传周期
				JsonValue = item->valueint;
				CONFIG_Meter.UpDuty = (uint32_t) JsonValue;
				CONFIG_Meter.UpDuty = 20;
				
				item = cJSON_GetObjectItem(test_arr,"warningLowBatteryVoltage");	//报警电压阈值
				memcpy(Value,item->valuestring,strlen(item->valuestring));
				sscanf(Value,"%f",&CONFIG_Meter.LowBattery);
				memset(Value,0,REC_COM_LEN);
				
				item = cJSON_GetObjectItem(test_arr,"warningLowCreditBalance"); 	//卡内余额报敬阈值
				JsonValue = item->valueint;
				CONFIG_Meter.LowCredit = JsonValue;
				
				item = cJSON_GetObjectItem(test_arr,"warningLowGasVolumeAlarm"); 	//罐内气体余量阈值
				memcpy(Value,item->valuestring,strlen(item->valuestring));
				sscanf(Value,"%f",&CONFIG_Meter.LowGasVolume);
				memset(Value,0,REC_COM_LEN);
				
				item = cJSON_GetObjectItem(test_arr,"meterCurrency"); 						//币种 单位
				memcpy(Value,item->valuestring,strlen(item->valuestring));
				strncpy(CONFIG_Meter.CURRENCY,Value,3);
				memset(Value,0,REC_COM_LEN);
				
				CONFIG_Meter_Write();
				
				cJSON_Delete(json);
			}
		}		
		
	}
	
}