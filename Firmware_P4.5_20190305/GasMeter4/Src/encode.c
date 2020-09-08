#include "encode.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

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
	DataTimeFormat(&cDataTime,rPacket->SESSION_END_TIME);
	strcat(*sendMeagess,cDataTime);
	strcat(*sendMeagess,",");
	free(cDataTime);
	
	strcat(*sendMeagess,"\"endTime\":\"");
	DataTimeFormat(&cDataTime,rPacket->SESSION_START_TIME);
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



