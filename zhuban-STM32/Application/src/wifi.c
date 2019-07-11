#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "gprs.h"
#include "common.h"
#include "usart.h"
#include "led.h"
#include "set_info.h"
#include "malloc.h"
#include "mem.h"

//extern char  bw_config;
//extern char  bw_rconfig;
char WIFI_cmdId;
static struct CONFIG_SendInfo sendat;
char *send_wifi_tmp;
extern INFO_MUL *Info_Sd;
extern char wifi_flag;

const struct Config_Info WIFI_Cmd[]={
 "+++","a",15,\
 "a","+ok",15,\
 "","+ok",15,\
 "AT+WMODE=APSTA\r","+ok",15,\
 "","+ok",15,\
 "","+ok",15,\
 "","+ok",15,\
};

void WIFI_Start()
{
  WIFI_cmdId=0;
  memset(&sendat.pdat,0,sizeof(sendat));
  sendat.status = CONFIG_NULL;
}

void WIFI_Reset()
{
  if(WIFI_cmdId<2)
  {
    WIFI_cmdId=0;
    memset(&sendat.pdat,0,sizeof(sendat));
    sendat.status = CONFIG_NULL;
  }
  if((WIFI_cmdId>2)&&(WIFI_cmdId<7))
  {
    WIFI_cmdId=2;
    memset(&sendat.pdat,0,sizeof(sendat));
    sendat.status = CONFIG_END;
    // HalUARTWrite(0,"+++\r\n", 5);
  }
}

void WIFI_Stat(void)
{ 
  switch (sendat.status)//status表示开始、等待、结束
  {
  case CONFIG_START:
    USART_Puts(USART1,sendat.pdat,strlen(sendat.pdat));
    sendat.status = CONFIG_WAIT;
    led_set(REDLIGHT);
    break;
  case CONFIG_WAIT:
    //检查结果
    if(sendat.check)
    {
      led_set(GREENLIGHT);
      sendat.status=CONFIG_END;
    }
    else if(sendat.time) //延时
    {
      sendat.time--;
    }
    else                 //超时处理
    {
      WIFI_Reset();
      led_set(REDLIGHT);//WIFI_Reset();
    }
    break;
  case CONFIG_END:
    if(WIFI_cmdId<6)
    {
      WIFI_cmdId++;
      sendat.status=CONFIG_NULL;
    }
    else
    {
      USART_Puts(USART1,"AT+Z\r",5);
      memset(&sendat.pdat, 0 , sizeof(sendat));
      led_set(YELLOWLIGHT);
      myfree(SRAMIN,send_wifi_tmp);
      send_wifi_tmp=NULL;
      wifi_flag=0;
    }
    break;
  }
}
void Config_WIFI_SendSetUp(unsigned char *pdat, unsigned char *pret,char time)
{
  sendat.pdat = pdat;
  sendat.pret = pret;
  sendat.time = time;
  sendat.check = 0;
  if(WIFI_cmdId ==2)
  {
 /*   char name[10];
    send_wifi_tmp=(unsigned char *)mymalloc(SRAMIN,50);
    sendat.pdat = send_wifi_tmp;
    mem_copy(name,"RHZ-",4);
    mem_copy(name+4,Info_Sd->Soft_Ver,2);
    name[6]=((Info_Sd->Addr[4]&0xf0)>>4)+0x30;
    name[7]=((Info_Sd->Addr[4]&0x0f))+0x30;
    name[8]=((Info_Sd->Addr[5]&0xf0)>>4)+0x30;
    name[9]=((Info_Sd->Addr[5]&0x0f))+0x30;
    sprintf(sendat.pdat,"AT+WAP=11BGN,%s,AUTO\r", name);*/
  char name[10];
    send_wifi_tmp=(unsigned char *)mymalloc(SRAMIN,50);
    sendat.pdat = send_wifi_tmp;
 //   mem_copy(name,"RHZ-",4);
//    mem_copy(name+4,Info_Sd->Soft_Ver,2);
//   mem_copy(name,"AGR",3);
   name[0]=(Info_Sd->Addr[0]);
   name[1]=(Info_Sd->Addr[1]);
   name[2]=(Info_Sd->Addr[2]);
    name[3]=((Info_Sd->Addr[3]&0xf0)>>4)+0x30;
    name[4]=((Info_Sd->Addr[3]&0x0f))+0x30;
    name[5]=((Info_Sd->Addr[4]&0xf0)>>4)+0x30;
    name[6]=((Info_Sd->Addr[4]&0x0f))+0x30;
    name[7]=((Info_Sd->Addr[5]&0xf0)>>4)+0x30;
    name[8]=((Info_Sd->Addr[5]&0x0f))+0x30;  
    sprintf(sendat.pdat,"AT+WAP=11BGN,%s,AUTO\r", name);
  }
  else if(WIFI_cmdId ==4)
  {
     sendat.pdat = send_wifi_tmp;
     sprintf(sendat.pdat,"AT+WSSSID=%s\r", Info_Sd->Apne_Name);
  }
  else if(WIFI_cmdId ==5)
  {
    sendat.pdat = send_wifi_tmp;
    sprintf(sendat.pdat,"AT+WSKEY=%s,%s,%s\r",Info_Sd->Encr_Auth, Info_Sd->Encr_Encry,Info_Sd->Apne_Passwd);
  }
  if(WIFI_cmdId ==6)
  {
     sendat.pdat = send_wifi_tmp;
     sprintf(sendat.pdat,"AT+SOCKB=UDP,%s,%s\r",Info_Sd->Server_Port,Info_Sd->Server_Ip);
  }
  sendat.status = CONFIG_START;
} 
void WIFI_Deal(void)
{
  if((WIFI_cmdId<7)&&(sendat.status==CONFIG_NULL))
  {
    Config_WIFI_SendSetUp(WIFI_Cmd[WIFI_cmdId].command, WIFI_Cmd[WIFI_cmdId].retComd,WIFI_Cmd[WIFI_cmdId].time);
  }
  if(WIFI_cmdId<7)
    WIFI_Stat();
}


void WIFI_RxDeal(char *pdat, char len)
{
  if(sendat.status == CONFIG_WAIT)
  {
    if(strstr((const char *)USART1_RX_BUF, (const char *)sendat.pret))
    {  
      sendat.check = 1;
      WIFI_Stat();
    }
  }
}


