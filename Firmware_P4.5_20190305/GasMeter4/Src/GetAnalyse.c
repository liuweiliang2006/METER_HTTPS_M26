#include "includes.h"
//#include "cJSON.h"

#define REC_COM_LEN 40
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
			json=cJSON_Parse(ptJson); //获取整个大的句柄
			if(json != NULL)
			{
				test_arr = cJSON_GetObjectItem(json,"message");
				
				item = cJSON_GetObjectItem(test_arr,"serverPort");
				JsonValue = item->valueint;
				
				item = cJSON_GetObjectItem(test_arr,"command");
				memcpy(Value,item->valuestring,strlen(item->valuestring));
				memset(Value,0,REC_COM_LEN);
				
				
				
				item = cJSON_GetObjectItem(test_arr,"sensorIntercept");
				JsonValue = item->valuedouble;
				
				cJSON_Delete(json);
			}
		}		
		
	}
	
}