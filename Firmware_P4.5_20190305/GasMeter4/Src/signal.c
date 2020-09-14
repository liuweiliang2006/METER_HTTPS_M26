#include "includes.h"

extern bool IsNeedRestart;
QueueHandle_t SendATQueue = NULL; 			//用于表示当前正在与无线通讯模组
SemaphoreHandle_t  Semaphore_Uart_Rec = NULL; //该信号量用于指示串口是否接收到完整数据帧
EventGroupHandle_t xCreatedEventGroup = NULL;

#define SEVER_URL "https://ateei9d448.execute-api.eu-west-1.amazonaws.com/"
#define SEVER_VERSION "testing/"

#define M26GETCOMMANDLEN 13
#define M26POSTCOMMANDLEN 15



stru_P4_command_t Send_AT_cmd[]={
	          //  u8CmdNum  	SendCommand																													pFun
/*0*/			{     	1,			 "AT\r\n",																												Analysis_AT_Cmd					},
/*1*/			{     	2,			 "AT+CSQ\r\n",																										Analysis_CSQ_Cmd				},
/*2*/			{     	3,			 "AT+QIREGAPP\r\n",																								Analysis_QIREGAPP_Cmd		},//设置APN 用户名密码 初始化TCP/IP任务
/*3*/			{     	4,			 "AT+QIACT\r\n",																									Analysis_QIACT_Cmd			},//激活GPRS PDP连接
/*4*/			{     	5,			 "AT+QILOCIP\r\n",																								Analysis_QILOCIP_Cmd		},//获取本地IP地址
/*5*/			{     	6,			 "AT+QSSLCFG=\"sni\",0,1\r\n",																		Analysis_SNI_Cmd				},
/*6*/			{     	7,			 "AT+QSSLCFG=\"https\",1\r\n",																		Analysis_QSSLCFG_Cmd		},//启用HTTPS功能 
/*7*/			{     	8,			 "AT+QSSLCFG=\"httpsctxi\",0\r\n",																Analysis_QSSLCFG_Cmd		},//为HTPS配置SSL上下文索引
/*8*/			{     	9,			 NULL											,																				Analysis_QHTTPURL_Cmd		}, //AT+QHTTPURL=88,60
/*9*/			{     	10,			 NULL,																														Analysis_SEVER_Addr_Cmd	},
/*10*/		{     	11,			 "AT+QHTTPGET=60,120\r\n",																				Analysis_QHTTPGET_Cmd		},  //GET请求
/*11*/		{     	12,			 "AT+QHTTPREAD\r\n",																							Analysis_QHTTPREAD_Cmd	},//读取HTTPS服务器的响应
/*12*/		{     	13,			 "AT+QIDEACT\r\n",																								Analysis_QIDEACT_Cmd		},//关闭当前GPRS/CSD场景
/*13*/		{     	14,			 "at+qhttpcfg=\"CONTENT-TYPE\",\"application/json\"\r\n",					Analysis_QSSLCFG_Cmd		},//设置JSON格式
/*14*/		{     	15,			 NULL,																														Analysis_QHTTPPOST_Cmd	},//AT+QHTTPPOST=272\r\n
/*15*/		{     	16,			 NULL,																														Analysis_POSTDATA_Cmd		}, //POST 指令携带的数据
/*16*/		{     	17,			 "AT+QHTTPCFG==\"requestheader\",1",															Analysis_QHTTPCFG_Cmd		}, //开启自定义头部功能
/*17*/		{     	18,			 "AT+QHTTPCFG==\"requestheader\",0",															Analysis_QHTTPCFG_Cmd		}, //关闭自定义头部功能
};


uint8_t u8GetNum[M26GETCOMMANDLEN]= {0,1,2,3,4,5,6,7,8,9,10,11,12};
uint8_t u8PostNum[M26POSTCOMMANDLEN] = {0,1,2,3,4,5,6,7,8,9,13,14,15,11,12};
uint8_t u8SniNum[3] = {0,1,5};

/* 实现itoa函数的源码 */ 
static char *myitoa(int num,char *str,int radix) 
{  
	/* 索引表 */ 
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	unsigned unum; /* 中间变量 */ 
	int i=0,j,k; 
	char temp;
	/* 确定unum的值 */ 
	if(radix==10&&num<0) /* 十进制负数 */ 
	{ 
		unum=(unsigned)-num; 
		str[i++]='-'; 
	} 
	else unum=(unsigned)num; /* 其它情况 */ 
	/* 逆序 */ 
	do  
	{ 
		str[i++]=index[unum%(unsigned)radix]; 
		unum/=radix; 
	}while(unum); 
	str[i]='\0'; 
	/* 转换 */ 
	if(str[0]=='-') k=1; /* 十进制负数 */ 
	else k=0; 
	/* 将原来的“/2”改为“/2.0”，保证当num在16~255之间，radix等于16时，也能得到正确结果 */ 
	 
	for(j=k;j<=(i-k-1)/2.0;j++) 
	{ 
		temp=str[j]; 
		str[j]=str[i-j-1]; 
		str[i-j-1]=temp; 
	} 
	return str; 
}

void AppObjCreate (void)
{
	/* 创建二值信号量，首次创建信号量计数值是0 */
	Semaphore_Uart_Rec = xSemaphoreCreateBinary();
	
	if(Semaphore_Uart_Rec == NULL)
	{
		printf("Semaphore creat failed!\r\n");
			/* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
	}
	
	SendATQueue = xQueueCreate(1, sizeof(uint8_t));
	if( SendATQueue == 0 )
	{
			printf("create failed\r\n");
	}
	
	/* 创建事件标志组 */
	xCreatedEventGroup = xEventGroupCreate();
	
	if(xCreatedEventGroup == NULL)
	{
			/* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
	}
}

//type:command,parameters,cookingSession,hardware,info,warning
static char * Sever_Address_GET(const Stru_Sever_Info_t* SeverInfo,char* type)
{
	uint8_t u8Lenth = 0;
	char* ptUrlInfo;
//	u8Lenth = strlen(type);

	u8Lenth = strlen(SeverInfo->Sendsever)+strlen(SeverInfo->SeverVer)+strlen(SeverInfo->MeterId)+strlen(SeverInfo->CardID)+strlen(type)+10;

	ptUrlInfo = (char *) malloc(u8Lenth);
	memset(ptUrlInfo,0,u8Lenth);
	printf("malloc end\r\n");
	u8Lenth = 0;
	u8Lenth = strlen(SeverInfo->Sendsever);
	if(u8Lenth != 0)
	{
		strcat(ptUrlInfo,SeverInfo->Sendsever);
	}
	//版本号拼接
	u8Lenth = 0;
	u8Lenth = strlen(SeverInfo->SeverVer);
	if(u8Lenth != 0)
	{
		strcat(ptUrlInfo,SeverInfo->SeverVer);
	}
	
	u8Lenth = 0;
	u8Lenth = strlen(SeverInfo->MeterId);
	if(u8Lenth != 0)
	{
		strcat(ptUrlInfo,SeverInfo->MeterId);
	}
	
	u8Lenth = 0;
	u8Lenth = strlen(SeverInfo->CardID);
	if(u8Lenth != 0)
	{
		strcat(ptUrlInfo,SeverInfo->CardID);
	}
	
	u8Lenth = 0;
	u8Lenth = strlen(type);
	if(u8Lenth != 0)
	{
		strcat(ptUrlInfo,type);
	}
	
	strcat(ptUrlInfo,"\r\n");
	
	return ptUrlInfo;
}
static char * Post_Data_Cmd(char *postdata)
{
	char* ptPostInfo;
	uint16_t u16Lenth = 0;
	u16Lenth = strlen(postdata)+10;
	ptPostInfo = (char *) malloc(u16Lenth);
	
	u16Lenth = 0;
	u16Lenth = strlen(postdata);
	if(u16Lenth != 0)
	{
		strcat(ptPostInfo,postdata);
	}
	strcat(ptPostInfo,"\r\n");
}

//该函数用于实现对发送URL（sever）长度的计算，并将计算结果添充到Send_AT_cmd[7].SendCommand
static void UrlLength(uint16_t length)
{
	char *ptUrlCharLength,chUrl[20]="AT+QHTTPURL=";
	char str;
	uint16_t i ;
	ptUrlCharLength = (char *) malloc(length+strlen(chUrl));
	myitoa(length,ptUrlCharLength,10); 
	strcat(chUrl,ptUrlCharLength);
	strcat(chUrl,",60\r\n");
	for(i =0;i<strlen(chUrl);i++)
	{
		str = chUrl[i];
		Send_AT_cmd[8].SendCommand[i] = str;
	}
}


//AT+QHTTPURL 9
//AT+QHTTPPOST  15
static void CmdLength(uint16_t urllength,uint8_t cmd_num)
{
	char ptUrlCharLength[4],chUrl[20];
	char str;
	uint16_t i ;
//	ptUrlCharLength = (char *) malloc(urllength+strlen(chUrl));
	myitoa(urllength,ptUrlCharLength,10); 
	if(cmd_num == Send_AT_cmd[8].u8CmdNum)  //如果需要填充AT+QHTTPURL指令
	{
		strcpy(chUrl,"AT+QHTTPURL=");
		strcat(chUrl,ptUrlCharLength);
//		strcat(chUrl,",60\r\n");
		strcat(chUrl,"\r\n");
		for(i =0;i<strlen(chUrl);i++)
		{
			str = chUrl[i];
			Send_AT_cmd[8].SendCommand[i] = str;
		}
	}

	if(cmd_num == Send_AT_cmd[14].u8CmdNum)  //如果需要填充AT+QHTTPPOST=272
	{
		strcpy(chUrl,"AT+QHTTPPOST=");
		strcat(chUrl,ptUrlCharLength);
		strcat(chUrl,"\r\n");
		for(i =0;i<strlen(chUrl);i++)
		{
			str = chUrl[i];
			Send_AT_cmd[14].SendCommand[i] = str;
		}
	}	
}

//向AWS发送GET请求
static void SendGetCommand()
{
	uint8_t i = 0;
	for(i=0;i< M26GETCOMMANDLEN;i++)
	{
		if((u8GetNum[i]!=5))
		{
			printf("send:%s\r\n\r\n",Send_AT_cmd[u8GetNum[i]].SendCommand);
			Sim80x.AtCommand.FindAnswer = 0;
			xQueueSend(SendATQueue,(void *) &Send_AT_cmd[u8GetNum[i]].u8CmdNum,(TickType_t)10);	 
			Sim80x_SendAtCommand(Send_AT_cmd[u8GetNum[i]].SendCommand,1000,1,"AT\r\r\nOK\r\n");
			osDelay(2000);
			while(!Sim80x.AtCommand.FindAnswer)
			{
				Sim80x_SendAtCommand(Send_AT_cmd[u8GetNum[i]].SendCommand,1000,1,"OK\r\n");
				osDelay(2000);
			}
		}		
	}
}

//向AWS发送POST请求
static ErrorStatus SendPostCommand()
{
	uint8_t i = 0;
	uint8_t u8Lenth = 0;
	EventBits_t event_value = 0;
	uint8_t u8ErrorFlag = 0;
//	for(i=0;i< sizeof(Send_AT_cmd)/sizeof(Send_AT_cmd[0]);i++)M26GETCOMMANDLEN
	for(i=0;i< M26POSTCOMMANDLEN;i++)
	{
		if((u8PostNum[i]!=5))
		{
			printf("send:%s\r\n\r\n",Send_AT_cmd[u8PostNum[i]].SendCommand);
			Sim80x.AtCommand.FindAnswer = 0;
			xQueueSend(SendATQueue,(void *) &Send_AT_cmd[u8PostNum[i]].u8CmdNum,(TickType_t)10);	 
//			Sim80x_SendAtCommand(Send_AT_cmd[u8PostNum[i]].SendCommand,1000,1,"AT\r\r\nOK\r\n");
//			osDelay(2000);
			while(!Sim80x.AtCommand.FindAnswer)
			{
				event_value = xEventGroupGetBits(xCreatedEventGroup);
				if(event_value != 0)
				{
					i = M26POSTCOMMANDLEN -1; //遇到AT指令的错误，则让使其在下一次循环当中让其断开
					xEventGroupClearBits( xCreatedEventGroup,0xffffff );
					event_value = 0;
					u8ErrorFlag = 1;
				}
				if(CONFIG_Meter.NotHaveDog == false && IsNeedRestart == false)
				{
						HAL_IWDG_Refresh(&hiwdg);
				}
				memset(Sim80x.UsartRxBuffer,0,_SIM80X_BUFFER_SIZE);
				Sim80x_SendAtCommand(Send_AT_cmd[u8PostNum[i]].SendCommand,1000,1,"OK\r\n");
				osDelay(2000);			
			}
		}
//		osDelay(5000);
	}
	if (u8ErrorFlag) 
	{
		return ERROR;
	}
	return SUCCESS;
}


//M26 SNI功能测试，在GET和POST前要打开SNI功能 对于模块来说只进行一次初始化即可。
static void M26_Sni_Init(void )
{
	uint8_t i = 0;
	for(i = 0;i < 3;i++)
	{
		{
			printf("send %s\r\n\r\n",Send_AT_cmd[u8SniNum[i]].SendCommand);
			Sim80x.AtCommand.FindAnswer = 0;
			xQueueSend(SendATQueue,(void *) &Send_AT_cmd[u8SniNum[i]].u8CmdNum,(TickType_t)10);	 
//			Sim80x_SendAtCommand(Send_AT_cmd[u8SniNum[i]].SendCommand,1000,1,"AT\r\r\nOK\r\n");
//			osDelay(2000);
			while(!Sim80x.AtCommand.FindAnswer)
			{
				if(CONFIG_Meter.NotHaveDog == false && IsNeedRestart == false)
					{
							HAL_IWDG_Refresh(&hiwdg);
					}
				{
					Sim80x_SendAtCommand(Send_AT_cmd[u8SniNum[i]].SendCommand,1000,1,"OK\r\n");
					osDelay(2000);
				}				
			}
		}		
	}
}
//######################################################################################################################
//AT 指令错误处理
//正确返回1，错误返回0。
uint8_t Analysis_AT_Cmd(char *pdata)
{
	char *ptStrStart ;
	char *ptFindResult ;
	static uint8_t u8ErrorCnt = 0;
	
	ptStrStart = (char*)Sim80x.UsartRxBuffer;
	ptFindResult = strstr(ptStrStart,"OK");
	if(ptFindResult != NULL)
	{
		u8ErrorCnt =0;
		return 1;
	}	
	
	u8ErrorCnt++;
	if(u8ErrorCnt == 10)
	{
		u8ErrorCnt = 0;
		xEventGroupSetBits(xCreatedEventGroup, ALL_AT_BIT | AT_BIT);
	}
	return 0;
}
//POST 指令携带的数据错误处理
//正确返回1，错误返回0。
uint8_t Analysis_POSTDATA_Cmd(char *pdata)
{
	char *ptStrStart ;
	char *ptFindResult ;
	ptStrStart = (char*)Sim80x.UsartRxBuffer;
	ptFindResult = strstr(ptStrStart,"OK");
	if(ptFindResult != NULL)
	{
		return 1;
	}	
	ptFindResult = strstr(ptStrStart,"ERROR");
	if(ptFindResult != NULL)
	{
		xEventGroupSetBits(xCreatedEventGroup, ALL_AT_BIT | POSTDATA_BIT);
		return 1;
	}
	return 0;
}

//AT+CSQ指令错误处理
//正确返回1，错误返回0。
uint8_t Analysis_CSQ_Cmd(char *pdata)
{
	char *ptStrStart ;
	char *ptFindResult ;
	static uint8_t u8ErrorCnt = 0;
	
	ptStrStart = (char*)Sim80x.UsartRxBuffer;
	ptFindResult = strstr(ptStrStart,"OK");
	if(ptFindResult != NULL)
	{
		ptFindResult = strstr(ptStrStart,"CSQ");
		if(ptFindResult != NULL)
		{
			if((strstr(ptStrStart,"99") || strstr(ptStrStart,"0,0")))
			{
				u8ErrorCnt++;
				if(u8ErrorCnt == 15)
				{
					u8ErrorCnt = 0;
					xEventGroupSetBits(xCreatedEventGroup, ALL_AT_BIT | CSQ_BIT);
				}				
			}
			else
			{
				u8ErrorCnt=0;
				return 1;
			}
		}
	}	
	return 0;
}

//AT+QIREGAPP指令错误处理
//正确返回1，错误返回0。
uint8_t Analysis_QIREGAPP_Cmd(char *pdata)
{
	char *ptStrStart ;
	char *ptFindResult ;
	static uint8_t u8ErrorCnt = 0;
	
	ptStrStart = (char*)Sim80x.UsartRxBuffer;
	ptFindResult = strstr(ptStrStart,"OK");
	if(ptFindResult != NULL)
	{
		u8ErrorCnt = 0;
		return 1;
	}	
	ptFindResult = strstr(ptStrStart,"ERROR");
	if(ptFindResult != NULL)
	{
		u8ErrorCnt++;
		if(u8ErrorCnt == 10)
		{
			u8ErrorCnt = 0;
			xEventGroupSetBits(xCreatedEventGroup, ALL_AT_BIT | QIREGAPP_BIT);
		}
		return 1;
	}	
	return 0;
}

//AT+QIACT指令错误处理
//正确返回1，错误返回0。
uint8_t Analysis_QIACT_Cmd(char *pdata)
{
	char *ptStrStart ;
	char *ptFindResult ;
	static uint8_t u8ErrorCnt = 0;
	
	ptStrStart = (char*)Sim80x.UsartRxBuffer;
	ptFindResult = strstr(ptStrStart,"OK");
	if(ptFindResult != NULL)
	{
		return 1;
	}	
//	
	ptFindResult = strstr(ptStrStart,"ERROR");
	if(ptFindResult == NULL)
	{
		return 1;
	}	
	return 0;
}

//AT+QIACT指令获取本地IP
//正确返回1，错误返回0。
uint8_t Analysis_QILOCIP_Cmd(char *pdata)
{
	char *ptStrStart ;
	char *ptFindResult ;
	static uint8_t u8ErrorCnt = 0;
	
	ptStrStart = (char*)Sim80x.UsartRxBuffer;
	ptFindResult = strstr(ptStrStart,".");
	if(ptFindResult != NULL)
	{
		return 1;
	}
	ptFindResult = strstr(ptStrStart,"ERROR");
	if(ptFindResult != NULL)
	{
		u8ErrorCnt++;
		if(u8ErrorCnt == 5)
		{
			u8ErrorCnt = 0;
			xEventGroupSetBits(xCreatedEventGroup, ALL_AT_BIT | QILOCIP_BIT);
		}
		return 0;
	}
	return 0;
}

//URL地址
//正确返回1，错误返回0。
uint8_t Analysis_SEVER_Addr_Cmd(char *pdata)
{
	char *ptStrStart ;
	char *ptFindResult ;
	
	ptStrStart = (char*)Sim80x.UsartRxBuffer;
	ptFindResult = strstr(ptStrStart,"OK");
	if(ptFindResult != NULL)
	{
		return 1;
	}
	
	ptFindResult = strstr(ptStrStart,"ERROR");
	if(ptFindResult != NULL)
	{
		xEventGroupSetBits(xCreatedEventGroup, ALL_AT_BIT | SEVERADDR_BIT);
		return 0;
	}		
	return 0;
}

uint8_t Analysis_SNI_Cmd(char *pdata)
{
	return 1;
}

//AT+QHTTPPOST指令
//正确返回1，错误返回0。
uint8_t Analysis_QHTTPPOST_Cmd(char *pdata)
{
	char *ptStrStart ;
	char *ptFindResult ;
	
	ptStrStart = (char*)Sim80x.UsartRxBuffer;
	ptFindResult = strstr(ptStrStart,"CONNECT");
	if(ptFindResult != NULL)
	{
		return 1;
	}	
	ptFindResult = strstr(ptStrStart,"ERROR");
	if(ptFindResult != NULL)
	{
		xEventGroupSetBits(xCreatedEventGroup, ALL_AT_BIT | QHTTPPOST_BIT);
		return 1;
	}	
	return 0;
}

//AT+QSSLCFG指令
//正确返回1，错误返回0。
uint8_t Analysis_QSSLCFG_Cmd(char *pdata)
{
	char *ptStrStart ;
	char *ptFindResult ;
	
	ptStrStart = (char*)Sim80x.UsartRxBuffer;
	ptFindResult = strstr(ptStrStart,"OK");
	if(ptFindResult != NULL)
	{
		return 1;
	}	
	
	ptFindResult = strstr(ptStrStart,"ERROR");
	if(ptFindResult != NULL)
	{
		xEventGroupSetBits(xCreatedEventGroup, ALL_AT_BIT | QSSLCFG_BIT);
		return 0;
	}
	return 0;
}

//AT+QHTTPURL指令
//正确返回1，错误返回0。
uint8_t Analysis_QHTTPURL_Cmd(char *pdata)
{
	char *ptStrStart ;
	char *ptFindResult ;
	
	ptStrStart = (char*)Sim80x.UsartRxBuffer;
	ptFindResult = strstr(ptStrStart,"CONNECT");
	if(ptFindResult != NULL)
	{
		return 1;
	}	
	ptFindResult = strstr(ptStrStart,"ERROR");
	if(ptFindResult != NULL)
	{
		xEventGroupSetBits(xCreatedEventGroup, ALL_AT_BIT | QHTTPURL_BIT);
		return 1;
	}	
	return 0;
}


uint8_t Analysis_QHTTPGET_Cmd(char *pdata)
{
	char *ptStrStart ;
	char *ptFindResult ;
	ptStrStart = (char*)Sim80x.UsartRxBuffer;
	ptFindResult = strstr(ptStrStart,"OK");
	if(ptFindResult != NULL)
	{
		return 1;
	}	
	ptFindResult = strstr(ptStrStart,"ERROR");
	if(ptFindResult != NULL)
	{
		return 0;
	}	
	return 0;
}

//AT+QHTTPURL指令
//正确返回1，错误返回0。
uint8_t Analysis_QHTTPREAD_Cmd(char *pdata)
{
	char *ptStrStart ;
	char *ptFindResult ;
	
	ptStrStart = (char*)Sim80x.UsartRxBuffer;
	ptFindResult = strstr(ptStrStart,"OK");
	if(ptFindResult != NULL)
	{
		return 1;
	}	
	ptFindResult = strstr(ptStrStart,"ERROR");
	if(ptFindResult != NULL)
	{
		xEventGroupSetBits(xCreatedEventGroup, ALL_AT_BIT | QHTTPREAD_BIT);
		return 1;
	}	
	return 0;
}

uint8_t Analysis_QIDEACT_Cmd(char *pdata)
{
	char *ptStrStart ;
	char *ptFindResult ;
	static uint8_t u8ErrorCnt = 0;
	ptStrStart = (char*)Sim80x.UsartRxBuffer;
	ptFindResult = strstr(ptStrStart,"OK");
	if((ptFindResult != NULL) || (u8ErrorCnt == 10))
	{
		u8ErrorCnt = 0;
		return 1;
	}
	ptFindResult = strstr(ptStrStart,"ERROR");
	if(ptFindResult != NULL)
	{
		u8ErrorCnt ++;
		if(u8ErrorCnt == 5)
		{
			u8ErrorCnt = 0;
			xEventGroupSetBits(xCreatedEventGroup, ALL_AT_BIT | QIDEACT_BIT);
		}
	}
	return 0;
}

uint8_t Analysis_QHTTPCFG_Cmd(char *pdata)
{
	char *ptStrStart ;
	char *ptFindResult ;
	static uint8_t u8ErrorCnt = 0;
	ptStrStart = (char*)Sim80x.UsartRxBuffer;
	ptFindResult = strstr(ptStrStart,"OK");
	if((ptFindResult != NULL) || (u8ErrorCnt == 10))
	{
		u8ErrorCnt = 0;
		return 1;
	}
	ptFindResult = strstr(ptStrStart,"ERROR");
	if(ptFindResult != NULL)
	{
		u8ErrorCnt ++;
		if(u8ErrorCnt == 5)
		{
			u8ErrorCnt = 0;
			xEventGroupSetBits(xCreatedEventGroup, ALL_AT_BIT | QIDEACT_BIT);
		}
	}
	return 0;
}


//postcookingsecsion 发送函数
void  PostCookingSecsion(void)  //SendReportDataPacket
{
	Stru_Sever_Info_t *struSeverInfo;
	uint8_t result = 0 , i = 0; //用于标识，是否响应了当前的指令
	char *ptUrl,*ptPost;
	char *ptPostData;
	ErrorStatus ErrorFlag;
//	char *cDataTime ;
	volatile uint16_t u8UrlLength = 0;
	
	if(REAL_DATA_Credit.CookingSessionSendNumber < REAL_DATA_Credit.CookingSessionEnd)
	{
		M26_Sni_Init();
	}
	

	while(REAL_DATA_Credit.CookingSessionSendNumber < REAL_DATA_Credit.CookingSessionEnd)
	{
		Send_AT_cmd[8].SendCommand =(char *)malloc(20);
		Send_AT_cmd[14].SendCommand =(char *)malloc(20);
		memset(Send_AT_cmd[8].SendCommand,0,20 *sizeof(char));
		memset(Send_AT_cmd[14].SendCommand,0,20 *sizeof(char));
		struSeverInfo = (struct SeverInfo *) malloc(sizeof(struct SeverInfo));
		ptPostData = (char *) malloc(350 *sizeof(char));
		memset(ptPostData,0,350 *sizeof(char));
		
		printf("\r\ncompare %d %d\r\n",REAL_DATA_Credit.CookingSessionSendNumber,REAL_DATA_Credit.CookingSessionEnd);
		printf("******PostCookingSecsion******");
		Cooking_Session_READ(REAL_DATA_Credit.CookingSessionSendNumber);//发送的时候从开始位置开始读取,发送成功,索引加一
		refreshCookingSessionReport(&CookingSessionReport);		
		
		encodeCookingPacket(&ptPostData,&CookingSessionReport); //组包 cookingsecsion POST的数据内容
		
		struSeverInfo->Sendsever = SEVER_URL;
		u8UrlLength = strlen(struSeverInfo->Sendsever);
		struSeverInfo->SeverVer = SEVER_VERSION;
		struSeverInfo->CardID = "/9088450934850394385";
		struSeverInfo->MeterId = "meter/cookingSession/TZ00000235";
		ptUrl = Sever_Address_GET( struSeverInfo,"");
		
		Send_AT_cmd[9].SendCommand = ptUrl; //URL地址
		u8UrlLength = strlen(ptUrl)-2;
		CmdLength(u8UrlLength,9);  //根据发送URL的长度		获取URL的长度添充AT+QHTTPURL=XX,60
		
		ptPost = Post_Data_Cmd( ptPostData);
		Send_AT_cmd[15].SendCommand = ptPost;
		u8UrlLength = strlen(ptPost)-2;
		CmdLength(u8UrlLength,15);  //根据发送POST的长度
		
		ErrorFlag =SendPostCommand();
		
		free(ptUrl);
		free(ptPost);
		free(Send_AT_cmd[8].SendCommand);
		free(Send_AT_cmd[14].SendCommand);		
		free(ptPostData);
		
		if(ErrorFlag != ERROR)   //发送成功，将序列号加1
		{
			REAL_DATA_Credit.CookingSessionSendNumber++;
			REAL_DATA_Credit_Write();//发送完cooking ,保存序号		
			printf("******end PostCookingSecsion******");
		}
		else  //发送失败不更新，下一周期重新上传
		{
			printf("******PostCookingSecsion fail!\r\n******");
			return ;			
		}
			
		
		
	}
}

//PostMeterStatus 发送函数
void  PostMeterStatus(void)  //SendReportStatePacket
{
	Stru_Sever_Info_t *struSeverInfo;
	uint8_t result = 0 , i = 0; //用于标识，是否响应了当前的指令
	char *ptUrl,*ptPost;
	char *ptPostData;
	char *cDataTime ;
	volatile uint16_t u8UrlLength = 0;	
	
	M26_Sni_Init();
	Send_AT_cmd[8].SendCommand =(char *)malloc(20);
	Send_AT_cmd[14].SendCommand =(char *)malloc(20);
	memset(Send_AT_cmd[8].SendCommand,0,20 *sizeof(char));
	memset(Send_AT_cmd[14].SendCommand,0,20 *sizeof(char));
	struSeverInfo = (struct SeverInfo *) malloc(sizeof(struct SeverInfo));
	ptPostData = (char *) malloc(350 *sizeof(char));
	memset(ptPostData,0,350 *sizeof(char));
	
	printf("******PostMeterStatus******");
	
	refreshReportStatePacket(&reportStatePacket);		
	encodeMeterStatusPacket(&ptPostData,&reportStatePacket); //组包 cookingsecsion POST的数据内容
	
	struSeverInfo->Sendsever = SEVER_URL;
	u8UrlLength = strlen(struSeverInfo->Sendsever);
	struSeverInfo->SeverVer = SEVER_VERSION;
	struSeverInfo->CardID = "";
	struSeverInfo->MeterId = "meter/status/TZ00000111";
	ptUrl = Sever_Address_GET( struSeverInfo,"");
	
	Send_AT_cmd[9].SendCommand = ptUrl; //URL地址
	u8UrlLength = strlen(ptUrl)-2;
	CmdLength(u8UrlLength,9);  //根据发送URL的长度		获取URL的长度添充AT+QHTTPURL=XX,60
	
	ptPost = Post_Data_Cmd( ptPostData);
	Send_AT_cmd[15].SendCommand = ptPost;
	u8UrlLength = strlen(ptPost)-2;
	CmdLength(u8UrlLength,15);  //根据发送POST的长度
	
	SendPostCommand();
	
	free(ptUrl);
	free(ptPost);
	free(Send_AT_cmd[8].SendCommand);
	free(Send_AT_cmd[14].SendCommand);		
	free(ptPostData);	
	
	printf("******end PostMeterStatus******");

}

//PostMeterWarning 发送函数
void  PostMeterWarning(void)  //SendWarnPacket();
{
	Stru_Sever_Info_t *struSeverInfo;
	uint8_t result = 0 , i = 0; //用于标识，是否响应了当前的指令
	char *ptUrl,*ptPost;
	char *ptPostData;
	char *cDataTime ;
	volatile uint16_t u8UrlLength = 0;	
	
	M26_Sni_Init();
	Send_AT_cmd[8].SendCommand =(char *)malloc(20);
	Send_AT_cmd[14].SendCommand =(char *)malloc(20);
	memset(Send_AT_cmd[8].SendCommand,0,20 *sizeof(char));
	memset(Send_AT_cmd[14].SendCommand,0,20 *sizeof(char));
	struSeverInfo = (struct SeverInfo *) malloc(sizeof(struct SeverInfo));
	ptPostData = (char *) malloc(500 *sizeof(char));
	memset(ptPostData,0,500 *sizeof(char));
	
	printf("******PostMeterWarning******");
	refreshWaringPacket(&waringPacket);
	encodeWarningsPacket(&ptPostData,&waringPacket); //组包 cookingsecsion POST的数据内容
	
	struSeverInfo->Sendsever = SEVER_URL;
	u8UrlLength = strlen(struSeverInfo->Sendsever);
	struSeverInfo->SeverVer = SEVER_VERSION;
	struSeverInfo->CardID = "";
	struSeverInfo->MeterId = "meter/warning/TZ00000111";
	ptUrl = Sever_Address_GET( struSeverInfo,"");
	
	Send_AT_cmd[9].SendCommand = ptUrl; //URL地址
	u8UrlLength = strlen(ptUrl)-2;
	CmdLength(u8UrlLength,9);  //根据发送URL的长度		获取URL的长度添充AT+QHTTPURL=XX,60
	
	ptPost = Post_Data_Cmd( ptPostData);
	Send_AT_cmd[15].SendCommand = ptPost;
	u8UrlLength = strlen(ptPost)-2;
	CmdLength(u8UrlLength,15);  //根据发送POST的长度
	
	SendPostCommand();
	
	free(ptUrl);
	free(ptPost);
	free(Send_AT_cmd[8].SendCommand);
	free(Send_AT_cmd[14].SendCommand);		
	free(ptPostData);	
	
	printf("******end PostMeterWarning******");
}

//PostMeterHardware 发送函数
void  PostMeterHardware(void)  //SendWarnPacket();
{
	Stru_Sever_Info_t *struSeverInfo;
	uint8_t result = 0 , i = 0; //用于标识，是否响应了当前的指令
	char *ptUrl,*ptPost;
	char *ptPostData;
	char *cDataTime ;
	volatile uint16_t u8UrlLength = 0;	
	
	M26_Sni_Init();
	Send_AT_cmd[8].SendCommand =(char *)malloc(20);
	Send_AT_cmd[14].SendCommand =(char *)malloc(20);
	memset(Send_AT_cmd[8].SendCommand,0,20 *sizeof(char));
	memset(Send_AT_cmd[14].SendCommand,0,20 *sizeof(char));
	struSeverInfo = (struct SeverInfo *) malloc(sizeof(struct SeverInfo));
	ptPostData = (char *) malloc(500 *sizeof(char));
	memset(ptPostData,0,400 *sizeof(char));
	
	printf("******PostMeterHardware******");
	refreshInformationPacket(&InformationPacket);
	encodeHardwarePacket(&ptPostData,&InformationPacket); 
	
	struSeverInfo->Sendsever = SEVER_URL;
	u8UrlLength = strlen(struSeverInfo->Sendsever);
	struSeverInfo->SeverVer = SEVER_VERSION;
	struSeverInfo->CardID = "";
	struSeverInfo->MeterId = "meter/hardware/TZ00000131";
	ptUrl = Sever_Address_GET( struSeverInfo,"");
	
	Send_AT_cmd[9].SendCommand = ptUrl; //URL地址
	u8UrlLength = strlen(ptUrl)-2;
	CmdLength(u8UrlLength,9);  //根据发送URL的长度		获取URL的长度添充AT+QHTTPURL=XX,60
	
	ptPost = Post_Data_Cmd( ptPostData);
	Send_AT_cmd[15].SendCommand = ptPost;
	u8UrlLength = strlen(ptPost)-2;
	CmdLength(u8UrlLength,15);  //根据发送POST的长度
	
	SendPostCommand();
	
	free(ptUrl);
	free(ptPost);
	free(Send_AT_cmd[8].SendCommand);
	free(Send_AT_cmd[14].SendCommand);		
	free(ptPostData);	
	
	printf("******end PostMeterHardware******");
}

//PostMeterSettings 发送函数
void  PostMeterSettings(void)  //
{
	Stru_Sever_Info_t *struSeverInfo;
	uint8_t result = 0 , i = 0; //用于标识，是否响应了当前的指令
	char *ptUrl,*ptPost;
	char *ptPostData;
	char *cDataTime ;
	volatile uint16_t u8UrlLength = 0;	
	
	M26_Sni_Init();
	Send_AT_cmd[8].SendCommand =(char *)malloc(20);
	Send_AT_cmd[14].SendCommand =(char *)malloc(20);
	memset(Send_AT_cmd[8].SendCommand,0,20 *sizeof(char));
	memset(Send_AT_cmd[14].SendCommand,0,20 *sizeof(char));
	
	struSeverInfo = (struct SeverInfo *) malloc(sizeof(struct SeverInfo));
	ptPostData = (char *) malloc(500 *sizeof(char));
	memset(ptPostData,0,500 *sizeof(char));
	
	printf("******PostMeterSettings******");
	refreshSetupPacket(&SetupPacket);
	encodeSettingsPacket(&ptPostData,&SetupPacket); 
	
	struSeverInfo->Sendsever = SEVER_URL;
	u8UrlLength = strlen(struSeverInfo->Sendsever);
	struSeverInfo->SeverVer = SEVER_VERSION;
	struSeverInfo->CardID = "";
	struSeverInfo->MeterId = "meter/settings/TZ00000525";
	ptUrl = Sever_Address_GET( struSeverInfo,"");
	
	Send_AT_cmd[9].SendCommand = ptUrl; //URL地址
	u8UrlLength = strlen(ptUrl)-2;
	CmdLength(u8UrlLength,9);  //根据发送URL的长度		获取URL的长度添充AT+QHTTPURL=XX,60
	
	ptPost = Post_Data_Cmd( ptPostData);
	Send_AT_cmd[15].SendCommand = ptPost;
	u8UrlLength = strlen(ptPost)-2;
	CmdLength(u8UrlLength,15);  //根据发送POST的长度
	
	SendPostCommand();
	
	free(ptUrl);
	free(ptPost);
	free(Send_AT_cmd[8].SendCommand);
	free(Send_AT_cmd[14].SendCommand);		
	free(ptPostData);	
	
	printf("******end PostMeterSettings******");
}

