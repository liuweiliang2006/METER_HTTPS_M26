20200930
Note:
Solve PostMeterSettings ERROR:3822  
Solution:
Changes integer data to character data.location encode.c  ,void encodeSettingsPacket()

20200924 
长按与定时逻辑分支合并

20200908 1812
新建文件 encode.c encode.h,并集成函数PostMeterStatus

20200924 
加入APN，建立一次通道，实现POST GET

20200922 
字段对接，国内版本可连接AWS，国外版本需要增加APN PIN等设置

20200909 0942
集成PostMeterWarning功能，累积共集成PostMeterStatus、PostMeterWarning、PostCookingSecsion

20200915 1013
加入cJSON.c cJSON.h文件，并测试成功
问题：跑机老化时，会自动重启。

20200914 0914
加入看门狗和休眠功能

20200910 0911
POST指令全部集成完成
下面代码，为参考老刘原始的逻辑，后需应添加到代码 2167行，并删除这版当中的2173-2222行
if(HAL_GetTick()-TimeForSlowRunGPRSFree > 1500)
{
	HAL_GPIO_WritePin(RUN_LED_GPIO_Port,RUN_LED_Pin,GPIO_PIN_SET);
	osDelay(500);
	HAL_GPIO_WritePin(RUN_LED_GPIO_Port,RUN_LED_Pin,GPIO_PIN_RESET);

	//GPRS_GetCurrentConnectionStatus();

	if(Sim80x.GPRS.Connection[0] != GPRSConnection_ConnectOK && Sim80x.GPRS.Connection[0] != GPRSConnection_AlreadyConnect)
	{
			printf("Sim80x.GPRS.Connection[0] %02X\r\n",Sim80x.GPRS.Connection[0]);
			connectStep = 6;//联网过程中失败,断开上下文重新连接
			//Sim80x.GPRS.Connection[0] = GPRSConnection_ConnectFail;
			//Sim80x_GPRSClose(99);
			break;
	}
	else
	{
		if(HAL_GetTick() - TimeForCurrStart >= 2000)
		{
			if(IsNeedSendCook == true)  //PostCookingSecsion
			{
				PostMeterStatus();
				LL_VCC(1);
				printf("Long press!\r\n");
				PostMeterSettings();
				PostMeterHardware();
				PostMeterWarning();
				PostCookingSecsion();
			}
			else if(IsNeedTimeing ==  true) //PostCookingSecsion
			{
				IsNeedTimeing = false;
				if(IsSendHalfTime == false)
				{
					PostCookingSecsion();
					IsSendHalfTime = true;
					TimeForCurrStart = HAL_GetTick();		
				}
				else
				{
					if(HAL_GetTick()-TimeForCurrStart > 1000 * 5)
					{
						PostMeterStatus(); //PostMeterStatus
						IsSendHalfTime = false;
						IsNeedTimeing = false;
						TimeForCurrStart = HAL_GetTick();
					}
				}
				HearRetryNumber = 0;
			}
			else if(IsNeedWarning == true)//报警信息 PostMeterWarning
			{
				if(HAL_GetTick()-TimeForCurrStart > 1000 * 5)
				{
					PostMeterWarning(); 
					IsNeedWarning = false;
					TimeForCurrStart = HAL_GetTick();
				}
				HearRetryNumber = 0;
			}
			else if(IsNeedInformationResponse == true) //PostMeterHardware
			{
				IsNeedInformationResponse = false;
				PostMeterHardware();
				commandType = 0;
				TimeForCurrStart = HAL_GetTick();
				HearRetryNumber = 0;
			}
		}
	}
}





V1.1 修改 PostCookingSecsion函数中 下面三条指令的位置 ，放在while循环中，分配-释放配对。
Send_AT_cmd[8].SendCommand =(char *)malloc(20);
Send_AT_cmd[14].SendCommand =(char *)malloc(20);
struSeverInfo = (struct SeverInfo *) malloc(sizeof(struct SeverInfo));

V1.0 完成长按功能的上传，本版本较老版本修改内容：
1、加入TIM14的使用，用于完成串口接收完成的判断；
2、增加SIGNAL文件，本文件实现了与M26的通信功能；
3、发送的长度由128改为512，真实通讯指令超过128字节。SIM80x.h