#include "gprs.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "sys.h"
#include "string.h"
//#include "ff.h"
#include "mem.h"
#include "data.h"
#include "dma.h"
#include "stdlib.h"
#include "stdio.h"
#include "malloc.h"
#include "update.h"

#define   GPRS_Lindex  5

/******************@DESCRIPTION: -- GPRS¨¢?¨ª???¨¢? *************/
struct Ipaddress
{
  char ipaddress[20];
  char ip_len;
};
struct Ipaddress ip;
/******************@DESCRIPTION: -- GPRS¨¢?¨ª??¨¹¨¢? *************/

static char GPRS_LINK=0;	//¦Ì¡À?¡ã¨¢??¨®¡Á¡ä¨¬?
static char cmdId = 0;		//¡À¨¤o?

static struct CONFIG_SendInfo sendat;
//unsigned int resend = 0;
extern INFO_MUL *Info_Sd;
char *send_gprs_tmp;

const struct Config_Info GPRS_Cmd[]={
  "AT+CGATT?\r\n", "+CGATT: 1",50,\
  "AT+CIPMODE=0\r\n","OK",50,\
  "AT+CSTT=\"CMNET\"\r\n", "OK",25,\
  "AT+CIICR\r\n","OK",25,\
  "AT+CIFSR\r\n", ".", 25,\
  "AT+CIPSTART=\"UDP\",\"120.25.67.66\",\"9718\"\r\n","CONNECT",50
};

/***************************************
*@DESCRIPTION: --GPRS????
*
*@Input: --?T
*
*@Return: --?T
****************************************/	
char Set_Gprs_server_address=0;
void GPRS_Reset(void)
{
  cmdId = 0;
  GPRS_LINK = CONFIG_NLINK;
  memset(&sendat.pdat, 0 , sizeof(sendat));
}

/***************************************
*@DESCRIPTION: --GPRS3?¨º??¡¥
*
*@Input: --?T
*
*@Return: --?T
****************************************/	
void GPRS_init(void)
{
//  Usart_init(2,9600);115200 usart2
  GPRS_Reset();
}

/*********************************************************************
* @fn      GenericApp_GPRS
*
* @brief   Send "People" message.
*
* @param   none
*
* @return  none
*/


/***************************************
*@DESCRIPTION: --GPRS¡Á¡ä¨¬?¡ê¡§¡Á¡ä¨¬??¨²?¨²??¡ê?
*
*@Input: --?T
*
*@Return: --?T
****************************************/	
void GPRS_Stat(void)
{ 
  switch (sendat.status)//status!A?La?Lo??a?Lo??!e|I?L?L!ay?!e??L¡éF?Lo?
  {
  case CONFIG_START:
    if(cmdId<7)
      USART_Puts(USART2,sendat.pdat,strlen((const char *)sendat.pdat));
    sendat.status = CONFIG_WAIT;
    led_set(REDLIGHT);
    break;
  case CONFIG_WAIT:
    if(sendat.check)
    {
      led_set(GREENLIGHT);
      
      sendat.status=CONFIG_END;
    }
    else if(sendat.time) //??LR?Lo!A
    {
      sendat.time--;
    }
    else                 //3??Lo!A!a|?L??La
    {
      GPRS_Reset();
      led_set(REDLIGHT);
    }
    break;
  case CONFIG_END:
    break;
  }
}

/***************************************
*@DESCRIPTION: --2¨¦?¨°¡Á?¡¤?
*
*@Input: --*pbuf¡êo¡äy2¨¦?¨°¡Á?¡¤?¦Ì?¡Á?¡¤?¡ä?
*           dest¡êo¡äy2¨¦?¨°¦Ì?¡Á?¡¤?
*            num¡êo¡äy2¨¦?¨°¡Á?¡¤?¡ä?¦Ì?3¡è?¨¨
*
*@Return: --char¡êo?¨°¦Ì?¦Ì?????
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

void Config_SendSetUp(char *pdat, char *pret,unsigned char time)
{
  sendat.pdat = pdat;
  sendat.pret = pret;
  sendat.time = time;
  sendat.check = 0;
  sendat.status = CONFIG_START;
  
  if(cmdId==5)
  {
    send_gprs_tmp=(unsigned char *)mymalloc(SRAMIN,50);
    sendat.pdat=send_gprs_tmp;
    sprintf(sendat.pdat,"AT+CIPSTART=\"UDP\",\"%s\",\"%s\"\r\n", Info_Sd->Server_Ip,Info_Sd->Server_Port);
  }
}

void GPRS_SetUp(void)
{
  if(sendat.status == CONFIG_END)
  {         
    if(cmdId < GPRS_Lindex)  
    {  
      cmdId++;
      sendat.status = CONFIG_NULL;
    }
    else if(cmdId == GPRS_Lindex)        //cmdId = 10;
    {
      memset(&sendat.pdat, 0 , sizeof(sendat));
      GPRS_LINK = CONFIG_INLINK;
      USART_Puts(USART2,"ATE0\r\n",6);//?L?L¡éG?????
      led_set(YELLOWLIGHT);
      myfree(SRAMIN,send_gprs_tmp);
      send_gprs_tmp=NULL;
      return ;
    }
  }
  if(sendat.status == CONFIG_NULL)
  {
    Config_SendSetUp(GPRS_Cmd[cmdId].command, GPRS_Cmd[cmdId].retComd,GPRS_Cmd[cmdId].time);
  }
}
int Is_Connect(void)
{
  return (GPRS_LINK==CONFIG_INLINK);
}
void GPRS_Deal(void)
{
  if(GPRS_LINK == CONFIG_NLINK || GPRS_LINK == CONFIG_GLINK)
  {
    GPRS_SetUp();
  }
  GPRS_Stat();
}

unsigned char get_hex(unsigned char tmp)
{
  if(tmp>='0' && tmp<='9')
    return tmp-'0';
  else return tmp-'A'+10;
}


unsigned char over[3]={0x1A,0X0D,0X0A};
void get_CSQ()
{
  USART_Puts(USART2,"AT+CSQ\r\n",8);
}
void send_3gData(unsigned char *ptmp,unsigned char len)
{
  char tmp[20];
  sprintf(tmp,"AT+CIPSEND=%2d\r\n",len);
   USART_Puts(USART2,tmp,15);
  delay_ms(300);
 
  USART_Puts(USART2,ptmp,len);
  USART_Puts(USART2,over,3);
  delay_ms(300);
  get_CSQ();
}

int USART2_RX_cnt=0;
const char HEAD[2]={0X55, 0XAA};
float csq;
extern void WIFI_GPRS_INIT(void);
void SIM800_RxDeal(void)
{
  if(DMA_Usart_Receive(2))//uart2 gprs1
  {
    if(Is_Connect())
    {
      if(find_str((const char *)USART2_RX_BUF, HEAD,UART_RX_LEN,2))
        USART2_RX_cnt++;
      if(strstr((const char *)USART2_RX_BUF,"+CSQ: "))
      {
        char *buf=strstr((const char *)USART2_RX_BUF,"+CSQ: ");
        buf[0] = buf[6];
        buf[1] = buf[7];
        if(buf[1]==',')
          csq = buf[6]-0x30;
        else
          csq = ( (buf[6]-0x30)*10+(buf[7]-0x30));
      }
      Deal_Uart(USART2_RX_BUF,UART_RX_LEN-MYDMA_GetCurrDataCounter(DMA1_Channel6),1);
    }                                                                                   
    else
    {
      if(sendat.status == CONFIG_WAIT)
      {
        if(strstr((const char *)USART2_RX_BUF, (const char *)sendat.pret))
        {  
          sendat.check = 1;
          GPRS_Stat();
        }
      }
      
    }
    if(strstr((const char *)USART2_RX_BUF,"ERROR"))
    {
      WIFI_GPRS_INIT();
    }
    
    mem_clear(USART2_RX_BUF,UART_RX_LEN);
    MYDMA_Enable(DMA1_Channel6);
  }
}
