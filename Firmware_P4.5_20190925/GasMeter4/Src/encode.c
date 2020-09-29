#include "includes.h"


//时间格式转化
void DataTimeFormat(char **destination,char *source)
{
	*destination = (char *) malloc(strlen("xxxx-xx-xxTxx:xx:xx.xxxZ")+2);
	memset(*destination,0,strlen("xxxx-xx-xxTxx:xx:xx.xxxZ")+2);
	
	strncat(*destination,source,4); //年
	strcat(*destination,"-");
	strncat(*destination,source+4,2);//月
	strcat(*destination,"-");
	strncat(*destination,source+6,2);//日
	strcat(*destination,"T");
	strncat(*destination,source+8,2);//时
	strcat(*destination,":");
	strncat(*destination,source+10,2);//分
	strcat(*destination,":");
	strncat(*destination,source+12,2);//秒
	strcat(*destination,".001Z\"");
}


void encodeCookingPacket(char **sendMeagess,CookingSessionReport_t *rPacket)
{
	char *cDataTime ;
	strcat(*sendMeagess,"{\"cookingSessionId\":\"220erbdsbudwofjewo4234fdwb\",");
	
	strcat(*sendMeagess,"\"startTime\":\"");
	DataTimeFormat(&cDataTime,rPacket->SESSION_START_TIME);
	strcat(*sendMeagess,cDataTime);
	strcat(*sendMeagess,",");
	free(cDataTime);
	
	strcat(*sendMeagess,"\"endTime\":\"");
	DataTimeFormat(&cDataTime,rPacket->SESSION_END_TIME);
	strcat(*sendMeagess,cDataTime);
	strcat(*sendMeagess,",");
	free(cDataTime);
	
	strcat(*sendMeagess,"\"endReason\":");
	strcat(*sendMeagess,rPacket->SESSION_END_TYPE);
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"endCumulativeMass\":");
	strcat(*sendMeagess,rPacket->END_CUMULATIVE_VOLUME);
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"startCumulativeMass\":");
	strcat(*sendMeagess,rPacket->START_CUMULATIVE_VOLUME);
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"startCredit\":");
	strcat(*sendMeagess,rPacket->CREDIT_SESSION_START);
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"endCredit\":");
	strcat(*sendMeagess,rPacket->CREDIT_SESSION_END);
	strcat(*sendMeagess,",");
	
	
	strcat(*sendMeagess,"\"gasRemaining\":");
	strcat(*sendMeagess,rPacket->GAS_REMAINING);
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"cardId\":");
//	strcat(*sendMeagess,rPacket->CARD_ID);
	strcat(*sendMeagess,"3943534095");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"csrpTimestamp\":\"");
	DataTimeFormat(&cDataTime,rPacket->datetime);
	strcat(*sendMeagess,cDataTime);
	free(cDataTime);
	strcat(*sendMeagess,"}");			
}


void encodeMeterStatusPacket(char **sendMeagess,reportStatePacket_t *rPacket)
{
	char *cDataTime ;
	strcat(*sendMeagess,"{\"batteryVoltage\":");
	strcat(*sendMeagess,rPacket->BatteryLevel);
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"gasTemperature\":");
	strcat(*sendMeagess,rPacket->GAS_TEMPERATURE);
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"tankLockStatus\":");
	if(rPacket->tank_Lock_status[0] == '1')
	{
		strcat(*sendMeagess,"true");
	}
	else if((rPacket->tank_Lock_status[0] == '0') || (rPacket->tank_Lock_status[0] == 'X'))
	{
		strcat(*sendMeagess,"false");
	}
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"tankSensorStatus\":");
	if(rPacket->tank_Sensor_status[0] == '1')
	{
		strcat(*sendMeagess,"true");
	}
	else if((rPacket->tank_Sensor_status[0] == '0') || (rPacket->tank_Sensor_status[0] == 'X'))
	{
		strcat(*sendMeagess,"false");
	}
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"gsmSignalIntensity\":");
	strcat(*sendMeagess,rPacket->GSM_signal_intensity);
	strcat(*sendMeagess,",");
	
	
	strcat(*sendMeagess,"\"needleSensorStatus\":");
	if(rPacket->NEEDLE_Sensor_status[0] == '1')
	{
		strcat(*sendMeagess,"true");
	}
	else if((rPacket->NEEDLE_Sensor_status[0] == '0') || (rPacket->NEEDLE_Sensor_status[0] == 'X'))
	{
		strcat(*sendMeagess,"false");
	}
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"lidLightSensorStatus\":");
	if(rPacket->Lid_Sensor_status[0] == '1')
	{
		strcat(*sendMeagess,"true");
	}
	else if((rPacket->Lid_Sensor_status[0] == '0') || (rPacket->Lid_Sensor_status[0] == 'X'))
	{
		strcat(*sendMeagess,"false");
	}
	strcat(*sendMeagess,",");		
	
	strcat(*sendMeagess,"\"electronicValveStatus\":");
	if(rPacket->Electric_Valve_status[0] == '1')
	{
		strcat(*sendMeagess,"true");
	}
	else if((rPacket->Electric_Valve_status[0] == '0') || (rPacket->Electric_Valve_status[0] == 'X'))
	{
		strcat(*sendMeagess,"false");
	}
	strcat(*sendMeagess,",");	
	
	strcat(*sendMeagess,"\"lidElectricLockStatus\":");
	if(rPacket->Lid_Lock_status[0] == '1')
	{
		strcat(*sendMeagess,"true");
	}
	else if((rPacket->Lid_Lock_status[0] == '0') || (rPacket->Lid_Lock_status[0] == 'X'))
	{
		strcat(*sendMeagess,"false");
	}
	strcat(*sendMeagess,",");

	strcat(*sendMeagess,"\"meterStatusDatestamp\":\"");
	DataTimeFormat(&cDataTime,rPacket->datetime);
	strcat(*sendMeagess,cDataTime);
	free(cDataTime);
	strcat(*sendMeagess,"}");		
}

void encodeWarningsPacket(char **sendMeagess,waringPacket_t *rPacket)
{
	char *cDataTime ;
	char lowbattery[5];
	strcat(*sendMeagess,"{\"warningName\":\"Low Gas Warning\"");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"warningDateTimestamp\":\"");
	DataTimeFormat(&cDataTime,rPacket->datetime);
	strcat(*sendMeagess,cDataTime);
	strcat(*sendMeagess,",");
	free(cDataTime);
	
	strcat(*sendMeagess,"\"warningId\":\"");
	strcat(*sendMeagess,rPacket->meterNumer);
	strcat(*sendMeagess,"\"");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"lowGas\":");
	if(rPacket->Low_gas_amount[0] == '1')
	{
		strcat(*sendMeagess,"true");
	}
	else if((rPacket->Low_gas_amount[0] == '0') || (rPacket->Low_gas_amount[0] == 'X'))
	{
		strcat(*sendMeagess,"false");
	}
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"lidOpen\":");
	if(rPacket->Illegal_lid_opening[0] == '1')
	{
		strcat(*sendMeagess,"true");
	}
	else if((rPacket->Illegal_lid_opening[0] == '0') || (rPacket->Illegal_lid_opening[0] == 'X'))
	{
		strcat(*sendMeagess,"false");
	}
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"lowCredit\":");
	if(rPacket->low_amount_of_prepaid_credit[0] == '1')
	{
		strcat(*sendMeagess,"true");
	}
	else if((rPacket->low_amount_of_prepaid_credit[0] == '0') || (rPacket->low_amount_of_prepaid_credit[0] == 'X'))
	{
		strcat(*sendMeagess,"false");
	}
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"lowBattery\":");
//	strcat(*sendMeagess,rPacket->Low_battery);
	sscanf(lowbattery,"%.2f",CONFIG_Meter.LowBattery);
	strcat(*sendMeagess,lowbattery);
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"gasTemperature\":");
	strcat(*sendMeagess,rPacket->GAS_TEMPERATURE);
	strcat(*sendMeagess,",");
	

	strcat(*sendMeagess,"\"tankSensorStatus\":");
	if(rPacket->tank_Sensor_status[0] == '1')
	{
		strcat(*sendMeagess,"true");
	}
	else if((rPacket->tank_Sensor_status[0] == '0') || (rPacket->tank_Sensor_status[0] == 'X'))
	{
		strcat(*sendMeagess,"false");
	}
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"tankSensorStatus2\":");
	if(rPacket->tank_Sensor_status[0] == '1')
	{
		strcat(*sendMeagess,"true");
	}
	else if((rPacket->tank_Sensor_status[0] == '0') || (rPacket->tank_Sensor_status[0] == 'X'))
	{
		strcat(*sendMeagess,"false");
	}
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"electricLockStatus\":");
	if(rPacket->Electric_Lock_status[0] == '1')
	{
		strcat(*sendMeagess,"true");
	}
	else if((rPacket->Electric_Lock_status[0] == '0') || (rPacket->Electric_Lock_status[0] == 'X'))
	{
		strcat(*sendMeagess,"false");
	}
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"gsmSignalIntensity\":");
	strcat(*sendMeagess,rPacket->GSM_signal_intensity);
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"needleSensorStatus\":");
	if(rPacket->NEEDLE_Sensor_status[0] == '1')
	{
		strcat(*sendMeagess,"true");
	}
	else if((rPacket->NEEDLE_Sensor_status[0] == '0') || (rPacket->NEEDLE_Sensor_status[0] == 'X'))
	{
		strcat(*sendMeagess,"false");
	}
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"electricValveStatus\":");
	if(rPacket->Electric_Valve_status[0] == '1')
	{
		strcat(*sendMeagess,"true");
	}
	else if((rPacket->Electric_Valve_status[0] == '0') || (rPacket->Electric_Valve_status[0] == 'X'))
	{
		strcat(*sendMeagess,"false");
	}	
	strcat(*sendMeagess,"}");		
}

void encodeHardwarePacket(char **sendMeagess,InformationPacket_t *rPacket)
{
	char *cDataTime ;
	strcat(*sendMeagess,"{\"pcbVersion\":\"");
	strcat(*sendMeagess,rPacket->PCB_V);
	strcat(*sendMeagess,"\"");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"mcuFirmwareVersion\":\"");
	strcat(*sendMeagess,rPacket->FIRMWARE_V);
	strcat(*sendMeagess,"\"");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"mcuFirmwareCompileDate\":\"");
	strcat(*sendMeagess,"2016-08-29T09:12:33.001Z");
	strcat(*sendMeagess,"\"");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"modemFirmwareVersion\":\"");
	strcat(*sendMeagess,"1752B13SIM7029E");
	strcat(*sendMeagess,"\"");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"batteryModel\":\"");
	strcat(*sendMeagess,rPacket->BATTERY_MODEL);
	strcat(*sendMeagess,"\"");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"batterySerialNumberList\":[\"");
	strcat(*sendMeagess,"BAT 18650-00208\", \"BAT 18650-001223\"]");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"iflowSerialNumber\":\"");
	strcat(*sendMeagess,"aaa22");
	strcat(*sendMeagess,"\"");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"hardwareDatestamp\":\"");
	DataTimeFormat(&cDataTime,rPacket->datetime);
	strcat(*sendMeagess,cDataTime);
	free(cDataTime);
	
	strcat(*sendMeagess,"}");		
}


void encodeSettingsPacket(char **sendMeagess,SetupPacket_t *rPacket)
{
	char *cDataTime ;
	char slope[10];
	char Intercept[10];
	CONFIG_Meter_t stReadMeterConfig;
	REAL_DATA_CALIBRATION_t stReadRealDataCalibration;
	REAL_Flow_CALIBRATION_t stReadRealFlowCalibration;
	CONFIG_GPRS_t stReadConfigGprs;
	
	MB85RS16A_READ(CONFIG_Meter_Address,(uint8_t*)(&stReadMeterConfig),sizeof(CONFIG_Meter_t));
	MB85RS16A_READ(CONFIG_GPRS_Address,(uint8_t*)(&stReadConfigGprs),sizeof(CONFIG_GPRS_t));
	
	strcat(*sendMeagess,"{\"command\":\"STUP\"");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"serverIPaddress\":\"");
	strcat(*sendMeagess,stReadConfigGprs.Server_IP);
//	strcat(*sendMeagess,"198.51.100.42");
	strcat(*sendMeagess,"\"");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"serverPort\":");
	strcat(*sendMeagess,stReadConfigGprs.Socket_Port);
//	strcat(*sendMeagess,"5070");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"serverURL\":\"");
	strcat(*sendMeagess,"https//xxxxx/xxxx");
	strcat(*sendMeagess,"\"");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"gasLevel\":");
	strcat(*sendMeagess,"9800");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"dataUploadPeriod\":");
//	strcat(*sendMeagess,"360");
	sprintf(rPacket->UpdatePeriod,"%d",stReadMeterConfig.UpDuty);
	strcat(*sendMeagess,rPacket->UpdatePeriod);
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"warningLowBatteryVoltage\":");
//	strcat(*sendMeagess,"4.5");
	sprintf(rPacket->LowBattery,"%f",stReadMeterConfig.LowBattery);
	strcat(*sendMeagess,rPacket->LowBattery);
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"warningLowCreditBalance\":");
//	strcat(*sendMeagess,"80");
	sprintf(rPacket->LowCredit,"%f",stReadMeterConfig.LowCredit);
	strcat(*sendMeagess,rPacket->LowCredit);
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"warningLowGasVolumeAlarm\":");
//	strcat(*sendMeagess,"2000");
	sprintf(rPacket->LowGasVolume,"%f",stReadMeterConfig.LowGasVolume);
	strcat(*sendMeagess,rPacket->LowGasVolume);
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"metercurrency\":\"");
//	strcat(*sendMeagess,"KSH");
	strcat(*sendMeagess,stReadMeterConfig.CURRENCY);
	strcat(*sendMeagess,"\"");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"uploadFrequency\":");
	strcat(*sendMeagess,"360");
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"uploadTime\":");
//	strcat(*sendMeagess,"0");
	sprintf(rPacket->StartPeriod,"%d",stReadMeterConfig.StartDuty);
	strcat(*sendMeagess,rPacket->StartPeriod);
	strcat(*sendMeagess,",");
	
	if(stReadMeterConfig.USE_SENSOR[0] == '1') //WEISHENG
	{
		SPI_FLASH_BufferRead((uint8_t*)(&stReadRealFlowCalibration),4096,sizeof(REAL_Flow_CALIBRATION_t));
		sprintf(slope,"%f",stReadRealFlowCalibration.ABCDEF[0]);
		sprintf(Intercept,"%d",stReadRealFlowCalibration.ParamNumber);
	}
	else if(stReadMeterConfig.USE_SENSOR[0] =='2') //WEINA
	{
		SPI_FLASH_BufferRead((uint8_t*)(&stReadRealDataCalibration),0,sizeof(REAL_DATA_CALIBRATION_t));
		sprintf(slope,"%f",stReadRealDataCalibration.slope);
		sprintf(Intercept,"%f",stReadRealDataCalibration.zero);
	}
	strcat(*sendMeagess,"\"sensorSlope\":");
	strcat(*sendMeagess,slope);
	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"sensorIntercept\":");
	strcat(*sendMeagess,Intercept);
	strcat(*sendMeagess,",");

//	strcat(*sendMeagess,"\"sensorSlope\":");
//	strcat(*sendMeagess,"2.3");
//	strcat(*sendMeagess,",");
//	
//	strcat(*sendMeagess,"\"sensorIntercept\":");
//	strcat(*sendMeagess,"0.1");
//	strcat(*sendMeagess,",");
	
	strcat(*sendMeagess,"\"settingDatestamp\":\"");
	DataTimeFormat(&cDataTime,rPacket->datetime);
	strcat(*sendMeagess,cDataTime);
	free(cDataTime);
	
	strcat(*sendMeagess,"}");		
}



