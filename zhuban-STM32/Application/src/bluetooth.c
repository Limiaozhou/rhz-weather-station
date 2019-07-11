#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "sys.h"
#include "bluetooth.h"
#include "gprs.h"
#include "usart.h"
#include "malloc.h"
#include "mem.h"
#include "stmflash.h"
#include "check.h"
#include "dma.h"
#include "led.h"

char bw_config;
static struct CONFIG_SendInfo blue_sendat;
extern char BLUE_cmdId;
extern unsigned char blue_pos;
extern unsigned char Rxdata[80];
extern char Flash_code[8];
//extern void GenericApp_package_Deal(unsigned char *pdat,unsigned char len);
//int im=0;
//unsigned char *pdat;
#define BULECONFIGOPEN   1
#define BULECONFIGCOLSE  2
#define   BLUE_Lindex  3
extern char Flash_server[20];//flash中读出的服务器地址
extern char Flash_port[6];//flash中读出的服务器端口号

const struct Config_Info BLUE_Cmd[]={
 "AT\r\n","OK",5,\
 "","OK",5,\
 "AT+PSWD=123456\r\n","OK",5,\
 "AT+UART=115200,0,0\r\n","OK",5,\
};
char BLUE_cmdId=0;
ST_BWConfig *Gbwconfig;
char *config_tmp;
unsigned char BLUE_LINK=0;
void Bule_ConfigPint(unsigned char stat)
{
   switch(stat)
   {
      case BULECONFIGOPEN:
				BLUE_LINK=1;
			GPIO_SetBits(GPIOC,GPIO_Pin_4);
      break;
      case BULECONFIGCOLSE:
				BLUE_LINK=0;
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
			if(Gbwconfig!=NULL)
				mem_free(SRAMIN,sizeof(ST_BWConfig));
			if(config_tmp!=NULL)
				mem_free(SRAMIN,60);
			Gbwconfig=NULL;
      break;
   }
}
void BLUE_Reset(void)
{
  BLUE_cmdId=0;
  Bule_ConfigPint(BULECONFIGCOLSE);
  memset(&blue_sendat.pdat,0,sizeof(blue_sendat));
  blue_sendat.status = CONFIG_NULL;
}

void Config_BlueSendSetUp(char *pdat, char *pret,unsigned char time)
{
  blue_sendat.pdat = pdat;
  blue_sendat.pret = pret;
  blue_sendat.time = time;
  blue_sendat.check = 0;
  blue_sendat.status = CONFIG_START;
  if(BLUE_cmdId==1)
  {  
//		char *tmp=(char *)mymalloc(SRAMIN,20);
		blue_sendat.pdat=config_tmp;
    sprintf(blue_sendat.pdat,"AT+NAME=%s\r\n", Gbwconfig->name);
  }
}
void modify_address(char address[8])
{
	STMFLASH_Write(FLASH_VERSION_CODE,(u16*)address,4);
	mem_copy(Flash_code,address,8);
}
extern char Set_Gprs_server_address;

void Config_init(unsigned char WIFI_con[])
{
	int i=3;
	char j=0;
	Gbwconfig = (ST_BWConfig *)mymalloc(SRAMIN,sizeof(ST_BWConfig));
	config_tmp=(char *)mymalloc(SRAMIN,60);
  if(Gbwconfig !=NULL) 
		mem_clear(&Gbwconfig->addr[0], sizeof(ST_BWConfig));
  switch(WIFI_con[i])
	{
		case 0xf5:
		{
			mem_copy(Gbwconfig->addr,&WIFI_con[i+1],8);
      mem_copy(Gbwconfig->name, "MyAir",5);
      Gbwconfig->name[5]=((Gbwconfig->addr[3]&0xf0)>>4)+0x30;
      Gbwconfig->name[6]=((Gbwconfig->addr[3]&0x0f))+0x30;
      Gbwconfig->name[7]=((Gbwconfig->addr[4]&0xf0)>>4)+0x30;
      Gbwconfig->name[8]=((Gbwconfig->addr[4]&0x0f))+0x30;
      Gbwconfig->name[9]=((Gbwconfig->addr[5]&0xf0)>>4)+0x30;
      Gbwconfig->name[10]=((Gbwconfig->addr[5]&0x0f))+0x30;
      for(j=5;j<11;j++)
      {
        if(Gbwconfig->name[j]>'9')
          Gbwconfig->name[j]+=7;
      }
      Gbwconfig->name[11]='\0';
			modify_address(Gbwconfig->addr);
			i+=9;
		}
//		break;
		case 0xf6:
		{
//      mem_copy(Gbwconfig->user_name, &WIFI_con[i+2],WIFI_con[i+1]);
			i+=WIFI_con[i+1]+2;
//      mem_copy(Gbwconfig->user_psd, &WIFI_con[i+1],WIFI_con[i]);
			i+=WIFI_con[i]+1;
		}
//		break;
		case 0xf7:
		{
		  mem_copy(Gbwconfig->server_server, &WIFI_con[i+2],WIFI_con[i+1]);
			i+=WIFI_con[i+1]+2;
	 	  mem_copy(Gbwconfig->server_port, &WIFI_con[i+1],WIFI_con[i]);
			i+=WIFI_con[i]+1;
		STMFLASH_Write(FLASH_VERSION_SERVER,(u16*)Gbwconfig->server_server,10);
		STMFLASH_Write(FLASH_VERSION_PORT,(u16*)Gbwconfig->server_port,3);
		mem_copy(Flash_server,Gbwconfig->server_server,20);
		mem_copy(Flash_port,Gbwconfig->server_port,6);
// 		  mem_copy(Gbwconfig->server_protocol, &WIFI_con[i+1],WIFI_con[i]);
//			i+=WIFI_con[i]+1;
		}
//		break;
/*		case 0xf8:
		{
      mem_copy(Gbwconfig->encr_auth, &WIFI_con[i+2],WIFI_con[i+1]);
			i+=WIFI_con[i+1]+2;
      mem_copy(Gbwconfig->encr_encr, &WIFI_con[i+1],WIFI_con[i]);
			i+=WIFI_con[i]+1;
		}*/
		default:
			break;
	}
	
	BLUE_Deal();
	Bule_ConfigPint(BULECONFIGOPEN);
	Set_Gprs_server_address=1;
	GPRS_Reset();
	
}

void BLUE_Stat(void)
{     
  switch (blue_sendat.status)//status表示开始、等待、结束
  {
  case CONFIG_START:
    //HalUARTWrite(1,sendat.pdat,strlen(sendat.pdat));
	  USART_Puts(USART2,blue_sendat.pdat,strlen((const char *)blue_sendat.pdat));
    blue_sendat.status = CONFIG_WAIT;
	  GPIO_SetBits(GPIOB,GREENLED|REDLED);
    break;
  case CONFIG_WAIT:
    //检查结果
    if(blue_sendat.check)
    {
      blue_sendat.status=CONFIG_END;
			led_set(GREENLIGHT);
    }
    else if(blue_sendat.time) //延时
    {
      //HalUARTWrite(1,sendat.pdat,strlen(sendat.pdat));
			USART_Puts(USART2,blue_sendat.pdat,strlen((const char *)blue_sendat.pdat));
      blue_sendat.time--;
    }
    else                 //超时处理
    {
      BLUE_Reset();
			led_set(REDLIGHT);
//      bw_config=6;
    }
    break;
  case CONFIG_END:
/*    if(BLUE_cmdId<3)
    {
      BLUE_cmdId++;
      sendat.status=CONFIG_NULL;
    }
    else
    {
        //Uart_Send_String("AT+RESET\r\n",10);
        //HalUARTWrite(1,"AT+RESET\r\n",10);
			  USART_Puts(USART2,"AT+RESET\r\n",10);
			  Bule_ConfigPint(BULECONFIGCOLSE);

//        bw_config=3;
        //BLUE_Reset();
     }*/
    break;
  }
}
void BLUE_SetUp(void)
{
  if(blue_sendat.status == CONFIG_END)
  {         
    if(BLUE_cmdId < BLUE_Lindex)  
    {  
      BLUE_cmdId++;
      blue_sendat.status = CONFIG_NULL;
      //GPIO_SetBits(GPIOB,REDLED);
    }
    else if(BLUE_cmdId == BLUE_Lindex)        //cmdId = 10;
    {
      memset(&blue_sendat.pdat, 0 , sizeof(blue_sendat));
 //     GPRS_LINK = CONFIG_INLINK;
			USART_Puts(USART2,"AT+RESET\r\n",10);
			  Bule_ConfigPint(BULECONFIGCOLSE);
			Set_Gprs_server_address=0;
      led_set(YELLOWLIGHT);
      return ;
    }
  }
  if(blue_sendat.status == CONFIG_NULL)
  {
    Config_BlueSendSetUp(BLUE_Cmd[BLUE_cmdId].command, BLUE_Cmd[BLUE_cmdId].retComd,BLUE_Cmd[BLUE_cmdId].time);
  }
}
void BLUE_Deal(void)
{
  if((BLUE_cmdId<4)&&BLUE_LINK)
  {
    BLUE_SetUp();
  }
  
  BLUE_Stat();
}

void BLUE_RxDeal(void)
{
  if(DMA_Usart_Receive(2))
  {
		if(USART2_RX_BUF[2]>0x30&&Check_CheckCRC16(&USART2_RX_BUF[3],USART2_RX_BUF[2]-2))
	  {
			Config_init(USART2_RX_BUF);
			//			delay_ms(200);
		}
		if(blue_sendat.status == CONFIG_WAIT)
		{
			if(strstr((const char *)USART2_RX_BUF,(const char *)blue_sendat.pret))
  		{
				blue_sendat.check = 1;
				BLUE_Stat();
				BLUE_Deal();
			}
		}
		BLUE_Stat();
		mem_clear(USART2_RX_BUF,UART_RX_LEN);
	  MYDMA_Enable(DMA1_Channel6);
	}
}


