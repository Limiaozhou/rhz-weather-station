#include"iostm8s103F3.h"
#include"typedef.h"
#include"mem.h"
#include "delay.h"
#include "check.h"
#include "config.h"
#include "uart.h"
//#include "485protocol.h"
#include "eeprom.h"
#include <math.h>
#include "stdio.h"
#define   RXBUFSIZE     20                   

//extern unsigned char open_pump[6];
static char Rxbuf[RXBUFSIZE];
char pos = 0;
short int uart1_clear_cunt=0;
unsigned char send[50];

//extern ST_Config config;
//unsigned char zeor_cmp[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//extern float SendDat;
//当前时钟是16M
void UART_Init(int baud)
{
  pos = 0;
  
  UART1_CR1=0x00;
  UART1_CR2=0x00;
  UART1_CR3=0x00;
  // 设置波特率，必须注意以下几点：
  // (1) 必须先写BRR2
  // (2) BRR1存放的是分频系数的第11位到第4位，
  // (3) BRR2存放的是分频系数的第15位到第12位，和第3位
  // 到第0位
  // 例如对于波特率位9600时，分频系数=16000000/9600=1666.66
  // 对应的十六进制数为0682（1666，1667为0x0683），BBR1=68,BBR2=02
  
  //UART1_BRR2=0x00;
  //UART1_BRR1=0x0d;
  switch(baud)
  {
  case 0://2400:
  UART1_BRR2=0x1A;
  UART1_BRR1=0xA0;
  break;
  case 1://9600:
  UART1_BRR2=0x02;
  UART1_BRR1=0x68;
  break;
  case 2://19200:
  UART1_BRR2=0x01;
  UART1_BRR1=0x34;
    break;
  case 3://38400:
  UART1_BRR2=0x00;
  UART1_BRR1=0x1A;
  break;
  case 4://57600:
  UART1_BRR2=0x05;
  UART1_BRR1=0x11;
  break;
  case 5://115200:
  UART1_BRR2=0x0A;
  UART1_BRR1=0x08;
  break;
  }

  UART1_CR2=0x2c;//允许接收，发送，开接收中断
  //UART1_CR2|=(0x01<<5);//允许接收，发送，开接收中断
  
}

void UART_Send(unsigned char *pbuf, unsigned char len)
{
  while(len--)
  {
    while((UART1_SR & 0x80)==0x00);  //发送寄存器空标志，发完和复位值为1，写UART1_DR会清0
    { 
      UART1_DR=*pbuf++;
    }
  }
}

void UART_Clear(void)
{
  for(char i=0;i< pos; i++)
  {
    Rxbuf[i] = 0;
  }
  pos = 0;
}

#pragma vector= UART1_R_OR_vector//0x19
__interrupt void UART1_R_OR_IRQHandler(void)
{
  char data;
  
 // if(UART1_SR_FE || UART1_SR_OR_LHE)  data = UART1_DR;
 // else if(UART1_SR_RXNE||pos<RXBUFSIZE)  
  
    data = UART1_DR;
    if(pos<RXBUFSIZE)  
      Rxbuf[pos++]=data;
//        UART_Send(Rxbuf, pos);
// UART_Send("hello", 5);
//     while((UART1_SR & 0x80)==0x00);
//    { 
//      UART1_DR=pos;
//    }
 // else data = UART1_DR;
  if(pos>=8)//485
  {
//    UART_Send(Rxbuf, pos);
//    pro485(Rxbuf);
    UART_Clear();
  }
  uart1_clear_cunt=0;
  return;
}
