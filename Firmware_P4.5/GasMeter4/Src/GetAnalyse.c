#include "includes.h"
//#include "cJSON.h"
enum {WEINA=2,WEISHENG=1}sensor_type;

#define REC_COM_LEN 40  //GET���ݵ������

EventGroupHandle_t xGetCmdEventGroup = NULL;

void GetCmdEventGroupCreat (void)
{
		/* �����¼���־�� */
	xGetCmdEventGroup = xEventGroupCreate();
	
	if(xGetCmdEventGroup == NULL)
	{
			/* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
	}
}

//����GET����������
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
		if(ptFindResult != NULL)   //GetMeterSetting �������
		{
			xEventGroupSetBits(xGetCmdEventGroup, GET_CMD_STUP);
			json=cJSON_Parse(ptJson); //��ȡ����Json��ľ��
			if(json != NULL)
			{
				test_arr = cJSON_GetObjectItem(json,"message");
				
				item = cJSON_GetObjectItem(test_arr,"serverIpAddress"); 					//IP��ַ
				memcpy(Value,item->valuestring,strlen(item->valuestring));
				strncpy(CONFIG_GPRS.Server_IP,Value,strlen(Value));
				memset(Value,0,REC_COM_LEN);
				
				item = cJSON_GetObjectItem(test_arr,"serverPort"); 					//�˿ں�
				JsonValue = item->valueint;
				sprintf(CONFIG_GPRS.Socket_Port,"%d",(uint32_t)JsonValue);	
				IsSaveCONFIG_GPRS = true;		
				CONFIG_GPRS_Write();				
				
				item = cJSON_GetObjectItem(test_arr,"dataUploadPeriod"); 					//�ϴ�����
				JsonValue = item->valueint;
				CONFIG_Meter.UpDuty = (uint32_t) JsonValue;
				
				item = cJSON_GetObjectItem(test_arr,"warningLowBatteryVoltage");	//������ѹ��ֵ
				memcpy(Value,item->valuestring,strlen(item->valuestring));
				sscanf(Value,"%f",&CONFIG_Meter.LowBattery);
				memset(Value,0,REC_COM_LEN);
				
				item = cJSON_GetObjectItem(test_arr,"warningLowCreditBalance"); 	//����������ֵ
				JsonValue = item->valuedouble;
				CONFIG_Meter.LowCredit = JsonValue;
				
				item = cJSON_GetObjectItem(test_arr,"warningLowGasVolumeAlarm"); 	//��������������ֵ
				memcpy(Value,item->valuestring,strlen(item->valuestring));
				sscanf(Value,"%f",&CONFIG_Meter.LowGasVolume);
				memset(Value,0,REC_COM_LEN);
				
				item = cJSON_GetObjectItem(test_arr,"meterCurrency"); 						//���� ��λ
				memcpy(Value,item->valuestring,strlen(item->valuestring));
				strncpy(CONFIG_Meter.CURRENCY,Value,3);
				memset(Value,0,REC_COM_LEN);
				
				IsSaveCONFIG_Meter = true;
				CONFIG_Meter_Write();
//				CONFIG_Meter_Write(); //�������Ϣ 
				item = cJSON_GetObjectItem(test_arr,"sensorType"); 						//����������
				JsonValue = item->valueint;
				if(JsonValue == WEINA)
				{
					item = cJSON_GetObjectItem(test_arr,"sensorSlope");	//б��
					memcpy(Value,item->valuestring,strlen(item->valuestring));
					sscanf(Value,"%f",&REAL_DATA_CALIBRATION.slope);
					memset(Value,0,REC_COM_LEN);
					
					item = cJSON_GetObjectItem(test_arr,"sensorIntercept"); 	//ԭ��
					JsonValue = item->valueint;
					REAL_DATA_CALIBRATION.zero = JsonValue;
					
					IsSaveCALIBRATION = true;
					CALIBRATION_Voltage_Write();
				}
				else if(JsonValue == WEISHENG)
				{
					item = cJSON_GetObjectItem(test_arr,"sensorSlope");	//б��
					memcpy(Value,item->valuestring,strlen(item->valuestring));
					sscanf(Value,"%f",REAL_Flow_CALIBRATION.ABCDEF);
					memset(Value,0,REC_COM_LEN);
					
					item = cJSON_GetObjectItem(test_arr,"sensorIntercept"); 	//ԭ��
					JsonValue = item->valueint;
					REAL_Flow_CALIBRATION.ParamNumber = JsonValue;		
					IsSaveFlowCALIBRATION = true;
					CALIBRATION_Flow_Write();					
				}
			
				
//				CALIBRATION_Voltage_Write();  //����б����ԭ��
				
				
				cJSON_Delete(json);
			}
		}		
		
	}
	
}