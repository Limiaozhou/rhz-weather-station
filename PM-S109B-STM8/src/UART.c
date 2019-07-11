#include"iostm8s103F3.h"
#include"typedef.h"
#include"mem.h"
#include "delay.h"
#include "check.h"
//#include "I2c_slave.h"
#define   RXBUFSIZE     64

//extern unsigned char open_pump[6];
//extern unsigned char close_pump[6];
static char Rxbuf[RXBUFSIZE];
char pos = 0;


void UART_Init(void)
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
      // 例如对于波特率位9600时，分频系数=2000000/9600=208
      // 对应的十六进制数为00D0，BBR1=0D,BBR2=00

    //UART1_BRR2=0x00;
    //UART1_BRR1=0x0d;
    UART1_BRR2=0x02;
    UART1_BRR1=0x68;

    UART1_CR2=0x2c;//允许接收，发送，开接收中断

}

void UART_Send(unsigned char *pbuf, unsigned char len)
{
   while(len--)
   {
//     UART1_DR = *pbuf++;
     while((UART1_SR & 0x80)==0x00);
//     { 
       UART1_DR=*pbuf++;
//     }
   }
}

void UART_Clear(void)
{
   for(char i=0;i< RXBUFSIZE; i++)
   {
     Rxbuf[i] = 0;
   }
   pos = 0;
}
float pm25;
float pm25_temp;
float pm10;
extern uchar sbuf[8];
extern char switch_flag;

typedef struct 
{
  int nol_dat;
  char flag;
  char error_flag;//如果有连续3个都异常大的温度数据，则此时温度赋值为异常大的情况
  float pre_dat;
  char first_flag;
}Pre_data;

Pre_data pre_pm25;
float pm250=0;

void Filter_data(float *act_data,Pre_data *pre,unsigned char range,int error_range)
{
  if(pre->first_flag)
  {
    float tmp=*act_data;
    if((*act_data-pre->pre_dat)>=range)// || (pre.pre_dat-*act_data>range))
    {
      if(pre->flag!=0xff)
        *act_data = pre->pre_dat;
      pre->flag=1;
    }
    else if((pre->pre_dat-*act_data)>=range)
    {
      if(pre->flag!=1)
        *act_data = pre->pre_dat;
      pre->flag=-1;    
    }
    else
      pre->flag=0;
    if((error_range>0 && *act_data>error_range) || (error_range<0 && *act_data<0-error_range))// && (pre->error_flag<3))
    {
      pre->error_flag++;
      if(pre->error_flag<3)
      {
        *act_data=(float)pre->nol_dat;
      }
    }
    else
    {
      pre->nol_dat=(int)*act_data;
      pre->error_flag=0;
    }
    pre->pre_dat = tmp;
  }
  else
  {
    pre->pre_dat=*act_data;
    pre->first_flag=1;
  }
  if(act_data==0) *act_data=pm250;
  else pm250=*act_data;
}
void UART_Receive()
{
//  unsigned char pdat[40]={0};
  unsigned char pdat[RXBUFSIZE]={0};
  unsigned char i=0;
    
   mem_copy(pdat,Rxbuf,pos);
   while(pos--)
   {
         //需添加判断，当前是开还是关，若switch_flag为1，则若收到关的回应，需再发送开的命令直到收到打开的回复；
         //                             若switch_flag为0， 若收到开的回应，需再发送关的命令直到收到关闭的回复
 /*      if(!switch_flag && ((pdat[0+i]==0x16)&&(pdat[2+i]==0x0c)&&(pdat[3+i]==0x02)&&(pdat[4+i]==0xDA)))
       {
            UART_Send(close_pump,6);	//UART_Send("000000",6);
            break;
       }
       if(switch_flag && ((pdat[0+i]==0x16)&&(pdat[2+i]==0x0c)&&(pdat[3+i]==0x01)&&(pdat[4+i]==0xDB)))
       {
            UART_Send(open_pump,6);	//UART_Send("000000",6);
            break;
       }*/
   //   if((pdat[0+i]==0x16)&&(pdat[2+i]==0x0c)) break;

//       if((pdat[0+i]==0x16)&&(pdat[1+i]==0x11)&&(pdat[2+i]==0x0B))//传感器回复读取结果
         if((pdat[0+i]==0x42)&&(pdat[1+i]==0x4d))
//       if((pdat[0+i]==0xff)&&(pdat[1+i]==0x17)&&(pdat[2+i]==0x04))//传感器回复读取结果  
       {
         //delay_ms(10);
  /*       pm25=((((long)(*(pdat+3+i)))<<24)+(((long)(*(pdat+4+i)))<<16)+(((long)(*(pdat+5+i)))<<8)+(int)(*(pdat+6+i)));
         //	delay_ms(10);
         pm10=((((long)(*(pdat+7+i)))<<24)+(((long)(*(pdat+8+i)))<<16)+(long)((*(pdat+9+i))<<8)+(int)(*(pdat+10+i)));
    //   pm25=(pm25*pm25/40)+0.5;
         
         if(pm25<=3)
         {
            pm25=pm25_temp;
         }
         else
           pm25_temp=pm25;
         
         
         if(pm25<5)
         {
           pm25+=5;
         }
         if(pm25<=10)
           pm25*=1.3;
         else if(pm25<=10)
           pm25*=1.1;
       */
//         pm25 = pdat[4+i]*256+pdat[5+i];
         pm25 = pdat[12+i]*256+pdat[13+i];
         pm10 = 0;
         if(pm25>=4)
             pm25+= 1;
         else
         {
            pm25+= 2;
            if(i%2==0)
              pm25+= 1;
         }
//         Filter_data(&pm25,&pre_pm25,10,500);
         sbuf[0]=(int)pm25/100;
         sbuf[1]=(int)pm25%100;
         sbuf[2]=(int)pm10/100;
         sbuf[3]=(int)pm10%100;
         Check_CalaCRC16(sbuf,4);
         break;
        }
       i++;
       if(pos<4) break;
   }
    UART_Clear();
}

void Deal()
{
  if(pos>=63)
  {
    delay_ms(100);
//   if(Rxbuf[1]==pos-3)
     UART_Receive();
//   else if(pos>15)
//     UART_Clear();
  }
}
#pragma vector= UART1_R_OR_vector//0x19
__interrupt void UART1_R_OR_IRQHandler(void)
{
      char data;
//      if(UART1_SR_FE || UART1_SR_OR_LHE)  data = UART1_DR;
//      else if(UART1_SR_RXNE||pos<RXBUFSIZE)  
//      {  
         data = UART1_DR;
         if(pos<RXBUFSIZE)  
           Rxbuf[pos++]=data;
//      }
//      else data = UART1_DR;
//  
//      return;
}
//将收到的数据再发送出去
/*
#pragma vector= UART1_R_OR_vector//0x19
__interrupt void UART1_R_OR_IRQHandler(void)
{
      Rxbuf[pos++]=UART1_DR;
      return;
}*/
