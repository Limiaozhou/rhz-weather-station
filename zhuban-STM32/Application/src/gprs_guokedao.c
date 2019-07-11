#include "gprs.h"
#include "delay.h"
#include "usart.h"
//#include "led.h"
#include "sys.h"
#include "string.h"
//#include "ff.h"
#include "mem.h"
#include "data.h"
#include "dma.h"
#include "stdlib.h"
#include "stdio.h"
#include "malloc.h"
#include "timer.h"
#include "wdg.h"

//#define   GPRS_WLINK    3
////////////////////////////////////////////////////////////////////////////////// 
//果壳道GPRS相关处理
//创建：xj
//版本：V1.0
//修改日期:2015/8/31
//Copyright(C) 深圳市睿海智电子科技有限公司  	 
////////////////////////////////////////////////////////////////////////////////// 

#define   GPRS_Lindex  6
//unsigned char Uart_Send_Buffer[UART_RX_LEN];
//extern unsigned char Is_gprs_connect;	//判断GPRS模块是否第一次连上网，以此判断是否需要重启
//char csq;		//3G的信号强度
char wifi_c;	//100%-WIFI丢包率
char g3_c;		//100%-3G丢包率
unsigned char wifiSet;


static char GPRS_LINK=0;	//当前连接状态
static char cmdId = 0;		//编号

struct GPRS_SendInfo sendat;
unsigned int resend = 0;
extern int send_flag;//该标记表示采集时间
extern unsigned char wifiSet;
ST_BWConfig  *Gbwconfig;

//int USART1_RX_Port_0 = 0;
//int USART1_RX_Port_1 = 0;
const char reset[4]={0x55,0xaa,0x03,0xb0};

//9708,9712

char cmd1[]="AT+WMODE=APSTA\r\n";
//char cmd2[]="AT+NAME=            \r\n";
char cmd3[];//="AT+WAP=11BGN,            ,AUTO\r\n";
char cmd4[];//="AT+WSSSID=                    \r\n";
char cmd5[];//="AT+WSKEY=          ,          ,                    \r\n";
char cmd6[];//="AT+SOCKB=      ,      ,                    \r\n";
//char cmd7[]="AT+Z\r\n";

struct GPRS_Info GPRS_Cmd[]={
  "+++", "a",25,\
  "a", "ok",25,\
  cmd1 ,"ok",25,\
  //cmd2 , "ok",25,
  cmd3, "ok",25,\
  cmd4, "ok",25,\
	cmd5, "ok",25,\
	cmd6, "ok",25
	//cmd7, "",25
};

void GPRS_SendCMD(u8 port,u8 *str,u16 len)
{
	u8 cmd[]="AT+CIPSEND=0,20 \r\n";
	if(GPRS_LINK==1)
	{
		cmd[11]=port+0x30;
		if(len<100)
		{
			cmd[13]=len/10+0x30;
			cmd[14]=len%10+0x30;
		}
		else
		{
			cmd[13]=len/100+0x30;
			cmd[14]=len%100/10+0x30;
			cmd[15]=len%100%10+0x30;
		}
		
		USART_Puts(USART3,cmd,strlen(cmd));
		delay_ms(50);
		USART_Puts(USART3,str,len);
		delay_ms(50);
	}
}

/***************************************
*@DESCRIPTION: --GPRS重置
*
*@Input: --无
*
*@Return: --无
****************************************/	
void GPRS_Reset(void)
{
	resend=0;
  cmdId = 0;
  GPRS_LINK = GPRS_NLINK;
  memset(&sendat.pdat, 0 , sizeof(sendat));
}

/***************************************
*@DESCRIPTION: --GPRS初始化
*
*@Input: --无
*
*@Return: --无
****************************************/	
void GPRS_init(void)
{
	u8 p=13;
	
	Gbwconfig = (ST_BWConfig *)mymalloc(SRAMIN,sizeof(ST_BWConfig));
  if(Gbwconfig !=NULL) 
		mem_clear(&Gbwconfig->addr[0], sizeof(ST_BWConfig));

	memcpy(Gbwconfig->name,"trs0618",7);
	memcpy(Gbwconfig->user_name,&USART3_RX_BUF[p+1],USART3_RX_BUF[p]);
	p+=USART3_RX_BUF[p];p++;
	memcpy(Gbwconfig->user_psd,&USART3_RX_BUF[p+1],USART3_RX_BUF[p]);
	p+=USART3_RX_BUF[p];p+=2;
	memcpy(Gbwconfig->server_server,&USART3_RX_BUF[p+1],USART3_RX_BUF[p]);
	p+=USART3_RX_BUF[p];p++;
	memcpy(Gbwconfig->server_port,&USART3_RX_BUF[p+1],USART3_RX_BUF[p]);
	p+=USART3_RX_BUF[p];p++;
	memcpy(Gbwconfig->server_protocol,&USART3_RX_BUF[p+1],USART3_RX_BUF[p]);
	p+=USART3_RX_BUF[p];p+=2;
	memcpy(Gbwconfig->encr_auth,&USART3_RX_BUF[p+1],USART3_RX_BUF[p]);
	p+=USART3_RX_BUF[p];p++;
	memcpy(Gbwconfig->encr_encr,&USART3_RX_BUF[p+1],USART3_RX_BUF[p]);
	
	sprintf(cmd3,"AT+WAP=11BGN,%s,AUTO\r\n", Gbwconfig->name);
	sprintf(cmd4,"AT+WSSSID=%s\r\n", Gbwconfig->user_name);
	sprintf(cmd5,"AT+WSKEY=%s,%s,%s\r\n",Gbwconfig->encr_auth, Gbwconfig->encr_encr,Gbwconfig->user_psd);
	sprintf(cmd6,"AT+SOCKB=%s,%s,%s\r\n",Gbwconfig->server_protocol,Gbwconfig->server_port,Gbwconfig->server_server);
	
	if(Gbwconfig!=NULL)
    mem_free(SRAMIN,sizeof(ST_BWConfig));
	Gbwconfig=NULL;
	
  GPRS_Reset();
}

/***************************************
*@DESCRIPTION: --GPRS状态（状态机机制）
*
*@Input: --无
*
*@Return: --无
****************************************/	
void GPRS_Stat(void)
{ 
  switch (sendat.status)//status!Aao??ao??!e|I!ay?!e?o?
  {
  case GPRS_START:
    if(cmdId<(GPRS_Lindex+1))
			USART_Puts(USART3,sendat.pdat,strlen((const char *)sendat.pdat));
    sendat.status = GPRS_WAIT;
    break;
  case GPRS_WAIT:
    if(sendat.check)
    {
      resend=0;
      sendat.status=GPRS_END;
    }
    else if(sendat.time) //?Ro!A
    {
      sendat.time--;
    }
    else                 //3?o!A!a|?a
    {
	    GPRS_Reset();//timeover
			wifiSet=0;
    }
    break;
  case GPRS_END:
    break;
  }
}

/***************************************
*@DESCRIPTION: --查找字符
*
*@Input: --*pbuf：待查找字符的字符串
*           dest：待查找的字符
*            num：待查找字符串的长度
*
*@Return: --char：找到的位置
****************************************/	
char Find_Char(char *pbuf, char dest, char num)
{
  char i=0;
  while(num-- > 0)
  {
    if(*pbuf++ == dest) return i;
    i++;
  }
  return 0xFF;
}
char ipcall[30]={0};
void GPRS_SendSetUp(char *pdat, unsigned char *pret,unsigned char time)
{
  sendat.pdat = pdat;
  sendat.pret = pret;
  sendat.time = time;
  sendat.check = 0;
  sendat.status = GPRS_START;
}

void GPRS_SetUp(void)
{
  if(sendat.status == GPRS_END)
  {         
    if(cmdId < GPRS_Lindex)  
    {  
      cmdId++;
      sendat.status = GPRS_NULL;
    }
    else if(cmdId == GPRS_Lindex)        //cmdId = 10;
    {
      memset(&sendat.pdat, 0 , sizeof(sendat));
      GPRS_LINK = GPRS_INLINK;
			USART_Puts(USART3,"AT+Z\r\n",6);
			wifiSet=0;
      return ;
    }
  }
  if(sendat.status == GPRS_NULL)
  {
    GPRS_SendSetUp(GPRS_Cmd[cmdId].command, GPRS_Cmd[cmdId].retComd,GPRS_Cmd[cmdId].time);
  }
}
int Is_Connect(void)
{
  return (GPRS_LINK==GPRS_INLINK);
}
void GPRS_Deal(void)
{
  if(GPRS_LINK == GPRS_NLINK || GPRS_LINK == GPRS_GLINK)
  {
    GPRS_SetUp();
  }
  GPRS_Stat();
}
/*
void GPRS_RxDeal(void)
{
  if(DMA_Usart_Receive(3))
  {
      if(sendat.status == GPRS_WAIT)
      {
				if(strstr((const char *)USART3_RX_BUF, (const char *)sendat.pret))
        {  
          sendat.check = 1;
          GPRS_Stat();
        }
      }
	}
	mem_clear(USART3_RX_BUF,UART_RX_LEN);
	MYDMA_Enable(DMA1_Channel5);
}*/
