//#include"iostm8s103K3.h"
#include"iostm8s103F3.h"
#include"typedef.h"

#define SCL   PB_IDR_IDR4
#define SDAI  PB_IDR_IDR5
#define SDAO  PB_ODR_ODR5
#include "uart.h"
#include "check.h"
//#define SlaveAddress 0x31

#define SlaveAddress 0x3C
//#define SlaveAddress2 0x3D

uchar iicstatus =0;  //0空闲模式  其它 工作模式
uchar bnums =0,addr=0;  //0空闲模式  其它 工作模式
uchar high,low;
float SendDat = 0,SendDat_pm10=0;
uchar sdata;
uchar sbuf[10]={0};

void SDA_SetOut()
{
    PB_DDR |= 0x20;
}

void SDA_SetIn()
{
    PB_DDR &= ~0x20;
}

void SDA_IRQSet()
{
   PB_CR2 =0x20;
   EXTI_CR1 = 0x0C; // PB口上升沿和下降沿触发
   SDA_SetIn();
   
   bnums =0;
   addr=0;
   iicstatus =0; 
   
   //SendDat +=1.01;
}

void SCL_IRQupSet()
{
    PB_CR2 =0x10;
    EXTI_CR1 = 0x04; // PB口上升沿
}

void SCL_IRQdownSet()
{
    PB_CR2=0x10;
    EXTI_CR1 = 0x08; // PB口下降沿触发
}

void IIC_Init(void)
{
	  PB_DDR &=~0x30; //
	  PB_CR1|=0x30;   // PB4 5 上拉输入
    EXTI_CR1 = 0x0C; // PB口上升沿和下降沿触发
    //开启端口中断
    PB_CR2|=0x20;
    
    iicstatus = 0;
    
    //SendDat = 1.054;
}
char switch_flag=1;//PM开关标志，为0时为关，为1时为开
//extern unsigned char open_pump[6];
//extern unsigned char close_pump[6];
//#pragma vector=EXTIB_vector
#pragma vector=EXTI1_vector
//0x06
__interrupt void EXTIB_IRQHandler(void)//对应IAP的中断地址：0x8060
{
   if(iicstatus==0)
   {
     //判断START
     if(!SDAI)  //SDA low 
     {  
         if(SCL) //CLK high
         {
            iicstatus = 0x55; //start
            SCL_IRQupSet();
         }
     }
   }
   else if(iicstatus==0x55)
   {
      bnums++;
      if(bnums<9)     //地址
      { 
        addr<<=1;
        if(SDAI) addr |= 0x01;          //数据的第一个CLK高电平来临  
        
      }
      else if(bnums==9) //ACK
      {
         SDA_SetOut();
         SDAO=0; 
         if((addr != SlaveAddress))  
         {
           if(addr==0x02)//开
           {
             switch_flag=1;
//             UART_Send(open_pump,6);
           }
           else //if(addr ==0x02)//开
           {
             switch_flag=0;
 //            UART_Send(close_pump,6);
           }
           SDA_IRQSet();
          // while(1);
         }
        
      }
      else
      {
         if(bnums>63 ) {SDA_IRQSet(); }    //stop  N*9
         if(bnums%9==1)
         {
           sdata = sbuf[bnums/9-1];
           
           if(sdata & 0x80) SDAO = 1;
           else            SDAO = 0;
         
           sdata <<=1; 
         }
         else
         {
           if(sdata & 0x80) SDAO = 1;
           else            SDAO = 0;
         
           sdata <<=1; 
         }
           
      }

      /*else if(bnums>9 && bnums<=17) //发送第一个数据
      {
         if(high & 0x80) SDAO = 1;
         else            SDAO = 0;
         
         high <<=1; 
      }
      else if(bnums>18 && bnums<=26) //发送第二个数据
      {
         if(low & 0x80) SDAO = 1;
         else           SDAO = 0;
         
         low <<=1; 
      }
      else  if(bnums>27 ) SDA_IRQSet();
      */
      
   }
}